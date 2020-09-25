/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/**
 * FairMQTransportDevice.cxx
 *
 * @since 2017.10.24
 * @author R. Karabowicz
 */

#include "FairMQTransportDevice.h"

#include "FairGenericStack.h"         // for FairGenericStack
#include "FairMCApplication.h"        // for FairMCApplication
#include "FairMCEventHeader.h"        // for FairMCEventHeader
#include "FairMCSplitEventHeader.h"   // for FairMCSplitEventHe...
#include "FairModule.h"               // for FairModule
#include "FairParSet.h"               // for FairParSet
#include "FairRunSim.h"               // for FairRunSim
#include "FairRuntimeDb.h"            // for FairRuntimeDb
#include "FairTask.h"                 // for FairTask
#include "RootSerializer.h"           // for RootSerializer

#include <FairMQMessage.h>       // for FairMQMessagePtr
#include <FairMQParts.h>         // for FairMQParts
#include <TClonesArray.h>        // for TClonesArray
#include <TCollection.h>         // for TIter
#include <TList.h>               // for TList
#include <TObjArray.h>           // for TObjArray
#include <TObject.h>             // for TObject
#include <TVirtualMC.h>          // for TVirtualMC
#include <cstring>               // for strcmp
#include <dlfcn.h>               // for dlerror, dlclose
#include <fairlogger/Logger.h>   // for Logger, LOG
#include <functional>            // for __base
#include <iostream>              // for string, operator<<
#include <memory>                // for unique_ptr
#include <string>                // for allocator, operator+
#include <vector>                // for vector

using namespace std;

FairMQTransportDevice::FairMQTransportDevice()
    : FairMQRunDevice()
    , fRunId(0)
    , fTransportDeviceId(0)
    , fGeneratorChannelName("primariesChannel")
    , fRunConditional(false)
    , fVMC(nullptr)
    , fStack(nullptr)
    , fMCApplication(nullptr)
    , fRunSim(nullptr)
    , fNofEvents(1)
    , fTransportName("TGeant3")
    , fMaterialsFile("")
    , fMagneticField(nullptr)
    , fDetectorArray(nullptr)
    , fStoreTrajFlag(false)
    , fTaskArray(nullptr)
    , fFirstParameter(nullptr)
    , fSecondParameter(nullptr)
    , fSink(nullptr)
    , fMCSplitEventHeader(nullptr)
{}

void FairMQTransportDevice::Init()
{
    if (!fRunConditional)
        OnData(fGeneratorChannelName, &FairMQTransportDevice::TransportData);
}

void FairMQTransportDevice::InitTask()
{
    fRunSim = new FairRunSim();

    fMCSplitEventHeader = new FairMCSplitEventHeader(fRunId, 0, 0, 0);
    fRunSim->SetMCEventHeader(fMCSplitEventHeader);
    fRunSim->SetRunId(fRunSim->GetMCEventHeader()->GetRunID());

    fRunSim->SetSink(fSink);

    if (fFirstParameter || fSecondParameter) {
        FairRuntimeDb* rtdb = fRunSim->GetRuntimeDb();
        if (fFirstParameter)
            rtdb->setFirstInput(fFirstParameter);
        if (fSecondParameter)
            rtdb->setSecondInput(fSecondParameter);
    }

    fRunSim->SetName(fTransportName.data());

    if (fUserConfig.Length() > 0)
        fRunSim->SetUserConfig(fUserConfig);
    if (fUserCuts.Length() > 0)
        fRunSim->SetUserCuts(fUserCuts);

    // -----   Create media   -------------------------------------------------
    fRunSim->SetMaterials(fMaterialsFile.data());

    // -----   Magnetic field   -------------------------------------------
    if (fMagneticField)
        fRunSim->SetField(fMagneticField);

    // -----   Create geometry   ----------------------------------------------
    for (int idet = 0; idet < fDetectorArray->GetEntries(); idet++) {
        fRunSim->AddModule(dynamic_cast<FairModule*>(fDetectorArray->At(idet)));
    }

    std::vector<std::string> detectorLibraries = fConfig->GetValue<std::vector<std::string>>("detector-library");

    for (unsigned int ilib = 0; ilib < detectorLibraries.size(); ilib++) {
        LOG(info) << " -----> library \"" << detectorLibraries.at(ilib) << "\"";

        void* handle = dlopen(detectorLibraries.at(ilib).c_str(), RTLD_LAZY);

        if (!handle) {
            LOG(fatal) << "Cannot open library: " << dlerror();
            return;
        }

        // load the symbol
        LOG(info) << "Loading symbol ...";
        typedef void (*det_t)();

        // reset errors
        dlerror();
        det_t ExternCreateDetector = (det_t)dlsym(handle, "ExternCreateDetector");
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            LOG(fatal) << "Cannot load symbol 'ExternCreateDetector': " << dlsym_error;
            dlclose(handle);
            return;
        }

        // run the function, where detector should be added to FairRunSim
        ExternCreateDetector();

        // close the library
        cout << "NOT closing library...\n";
        //    dlclose(handle);
    }

    // -----   Negotiate the run number   -------------------------------------
    // -----      via the fUpdateChannelName   --------------------------------
    // -----      ask the fParamMQServer   ------------------------------------
    // -----      receive the run number and sampler id   ---------------------
    std::string* askForRunNumber = new string("ReportSimDevice");
    FairMQMessagePtr req(NewMessage(
        const_cast<char*>(askForRunNumber->c_str()),
        askForRunNumber->length(),
        [](void* /*data*/, void* object) { delete static_cast<string*>(object); },
        askForRunNumber));
    FairMQMessagePtr rep(NewMessage());

    if (Send(req, fUpdateChannelName) > 0) {
        if (Receive(rep, fUpdateChannelName) > 0) {
            std::string repString = string(static_cast<char*>(rep->GetData()), rep->GetSize());
            LOG(INFO) << " -> " << repString.data();
            fRunId = stoi(repString);
            fMCSplitEventHeader->SetRunID(fRunId);
            repString = repString.substr(repString.find_first_of('_') + 1, repString.length());
            fTransportDeviceId = stoi(repString);
            LOG(INFO) << "runId = " << fRunId << "  ///  fTransportDeviceId = " << fTransportDeviceId;
        }
    }

    fRunSim->SetStoreTraj(fStoreTrajFlag);

    // -----   Set tasks   ----------------------------------------------------
    if (fTaskArray) {
        for (int itask = 0; itask < fTaskArray->GetEntries(); itask++) {
            fRunSim->AddTask(dynamic_cast<FairTask*>(fTaskArray->At(itask)));
        }
    }

    // -----   Initialize simulation run   ------------------------------------
    //    fRunSim->SetRunId(fRunId); // run n simulations with same run id - offset the event number
    fRunSim->Init();

    fVMC = TVirtualMC::GetMC();
    fMCApplication = FairMCApplication::Instance();
    fStack = fMCApplication->GetStack();
    fStack->Register();
    //  fRunSim->Run(0);
    UpdateParameterServer();
    LOG(INFO) << "end of FairMQTransportDevice::InitTask() run id = " << fRunSim->GetMCEventHeader()->GetRunID();
    LOG(INFO) << " name/title/classname = " << fRunSim->GetMCEventHeader()->GetName() << "/"
              << fRunSim->GetMCEventHeader()->GetTitle() << "/" << fRunSim->GetMCEventHeader()->ClassName();
}

void FairMQTransportDevice::PreRun() {}

bool FairMQTransportDevice::ConditionalRun()
{
    if (!fRunConditional)
        return false;

    std::string* requestString = new string("RequestData");
    FairMQMessagePtr req(NewMessage(
        const_cast<char*>(requestString->c_str()),
        requestString->length(),
        [](void* /*data*/, void* object) { delete static_cast<string*>(object); },
        requestString));
    FairMQParts parts;
    //    FairMQMessagePtr rep(NewMessage());

    if (Send(req, fGeneratorChannelName) > 0) {
        if (Receive(parts, fGeneratorChannelName) > 0) {
            return TransportData(parts, 0);
        }
    }
    return false;
}

// bool FairMQTransportDevice::TransportData(FairMQMessagePtr& mPtr, int /*index*/)
// {
//     TClonesArray* chunk = nullptr;
//     Deserialize<RootSerializer>(*mPtr, chunk);
//     fStack->SetParticleArray(chunk);
//     fVMC->ProcessRun(1);

//     return true;
// }

bool FairMQTransportDevice::TransportData(FairMQParts& mParts, int /*index*/)
{
    TClonesArray* chunk = nullptr;
    FairMCSplitEventHeader* meh = nullptr;
    for (int ipart = 0; ipart < mParts.Size(); ipart++) {
        TObject* obj = nullptr;
        Deserialize<RootSerializer>(*mParts.At(ipart), obj);
        if (strcmp(obj->GetName(), "MCEvent") == 0)
            meh = dynamic_cast<FairMCSplitEventHeader*>(obj);
        else if (strcmp(obj->GetName(), "TParticles") == 0)
            chunk = dynamic_cast<TClonesArray*>(obj);
    }
    if (chunk != nullptr) {
        fStack->SetParticleArray(chunk, meh->GetChunkStart(), meh->GetNPrim());
        fMCSplitEventHeader->SetRECC(fRunId, meh->GetEventID(), meh->GetNofChunks(), meh->GetChunkStart());
        fVMC->ProcessRun(1);
    }

    return true;
}

void FairMQTransportDevice::UpdateParameterServer()
{
    FairRuntimeDb* rtdb = fRunSim->GetRuntimeDb();

    LOG(info) << "FairMQTransportDevice::UpdateParameterServer() (" << rtdb->getListOfContainers()->GetEntries()
              << " containers)";

    // send the parameters to be saved
    TIter next(rtdb->getListOfContainers());
    FairParSet* cont;
    while ((cont = static_cast<FairParSet*>(next()))) {
        std::string ridString = std::string("RUNID") + std::to_string(fRunSim->GetRunId()) + std::string("RUNID")
                                + std::string(cont->getDescription());
        cont->setDescription(ridString.data());
        SendObject(cont, fUpdateChannelName);
    }

    LOG(info) << "FairMQTransportDevice::UpdateParameterServer() finished";
}

void FairMQTransportDevice::PostRun() {}

FairMQTransportDevice::~FairMQTransportDevice() {}
