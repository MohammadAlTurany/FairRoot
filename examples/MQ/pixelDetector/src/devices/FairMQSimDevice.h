/********************************************************************************
 *    Copyright (C) 2017 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/**
 * FairMQSimDevice.h
 *
 * @since 2018-02-01
 * @author R. Karabowicz
 */

#ifndef FAIRMQSIMDEVICE_H_
#define FAIRMQSIMDEVICE_H_

#include <Rtypes.h>
#include <TString.h>          // for TString
#include <RtypesCore.h>       // for UInt_t
#include <cstdint>            // for int64_t
#include <string>
#include <iosfwd>             // for string

#include "FairMQRunDevice.h"  // for FairMQRunDevice

class FairRunSim;
class FairField;
class FairParIo;
class FairPrimaryGenerator;
class TObjArray;
class FairSink;

class FairMQSimDevice : public FairMQRunDevice
{
  public:
    FairMQSimDevice();
    virtual ~FairMQSimDevice() {}

    virtual void SetParamUpdateChannelName(const TString& tString) { fUpdateChannelName = tString; }

    // ------ FairRunSim setters ------
    void SetNofEvents(int64_t nofev) { fNofEvents = nofev; }
    void SetTransportName(const std::string& str) { fTransportName = str; }
    void SetMaterials(const std::string& str) { fMaterialsFile = str; }
    void SetMagneticField(FairField* field) { fMagneticField = field; }
    void SetDetectorArray(TObjArray* array) { fDetectorArray = array; }
    void SetGenerator(FairPrimaryGenerator* primGen) { fPrimaryGenerator = primGen; }
    void SetStoreTraj(bool flag = true) { fStoreTrajFlag = flag; }
    void SetTaskArray(TObjArray* array) { fTaskArray = array; }
    void SetFirstParameter(FairParIo* par) { fFirstParameter = par; }
    void SetSecondParameter(FairParIo* par) { fSecondParameter = par; }
    void SetUserConfig(const TString& Config) { fUserConfig = Config; }
    void SetUserCuts(const TString& Cuts) { fUserCuts = Cuts; }
    void SetSink(FairSink* sink) { fSink = sink; }
    // ------ ---------- -------- ------

    virtual void SendBranches();

  protected:
    virtual void InitTask();
    virtual void PreRun() {}
    virtual void PostRun() {}
    virtual bool ConditionalRun();

  private:
    UInt_t fSimDeviceId;
    std::string fUpdateChannelName;

    FairRunSim* fRunSim;
    // ------ FairRunSim settings ------
    int64_t fNofEvents;
    std::string fTransportName;
    std::string fMaterialsFile;
    FairField* fMagneticField;
    TObjArray* fDetectorArray;
    FairPrimaryGenerator* fPrimaryGenerator;
    bool fStoreTrajFlag;
    TObjArray* fTaskArray;
    FairParIo* fFirstParameter;    // first (prefered) input for parameters
    FairParIo* fSecondParameter;   // second input (used if not found in first input)
    TString fUserConfig;           //!                  /** Macro for geant configuration*/
    TString fUserCuts;             //!                  /** Macro for geant cuts*/
    FairSink* fSink;
    // ------ ---------- -------- ------

    void UpdateParameterServer();

    FairMQSimDevice(const FairMQSimDevice&);
    FairMQSimDevice& operator=(const FairMQSimDevice&);
};

#endif /* FAIRMQSIMDEVICE_H_ */
