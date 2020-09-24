/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// --------------------------------------------------------------------------
// -----          Header for the FairTutorialDet2Digizier              ------
// -----              Created 25.09.17 by S.Wenzel                     ------
// --------------------------------------------------------------------------

#ifndef FAIRTUTORIALDET2CUSTOMTASK_H
#define FAIRTUTORIALDET2CUSTOMTASK_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Option_t
#include <vector>        // for vector

#include "FairTask.h"    // for FairTask, InitStatus

class CustomClass;
class TBuffer;
class TClass;
class TMemberInspector;

class FairTutorialDet2CustomTask : public FairTask
{
  public:
    /** Default constructor **/
    FairTutorialDet2CustomTask();

    /** Standard constructor **/
    FairTutorialDet2CustomTask(const char* name, const char* title = "FAIR Task");

    /** Destructor **/
    virtual ~FairTutorialDet2CustomTask();

    /** Initialisation **/
    virtual InitStatus Init();

    /** Executed task **/
    virtual void Exec(Option_t* option);

  private:
    // This task just has input data to be retrieved with InitObjectAs function
    // note that we are forced to put const on data that we are consuming
    std::vector<CustomClass> const* fCustomData = nullptr;    //!
    std::vector<CustomClass> const* fCustomData2 = nullptr;   //!
    ClassDef(FairTutorialDet2CustomTask, 1);
};
#endif   // FAIRTUTORIALDETCUSTOMTASK_H
