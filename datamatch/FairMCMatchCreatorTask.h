/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                 FAIRMCMATCHCREATORTASK header file             -----
// -----                  Created 20/03/07  by R.Kliemt               -----
// -------------------------------------------------------------------------

/** FAIRMCMATCHCREATORTASK.h
 *@author T.Stockmanns <t.stockmanns@fz-juelich.de>
 **
 ** Displays all available informations for a given event
 **/

#ifndef FAIRMCMATCHCREATORTASK_H
#define FAIRMCMATCHCREATORTASK_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Bool_t, Int_t, Option_t
#include <map>           // for map
#include <string>     // for string
#include <iosfwd>        // for string

#include "FairTask.h"    // for InitStatus, FairTask

class FairMCMatch;
class TClonesArray;
class TBuffer;
class TClass;
class TMemberInspector;

class FairMCMatchCreatorTask : public FairTask
{
  public:
    /** Default constructor **/
    FairMCMatchCreatorTask();

    /** Destructor **/
    virtual ~FairMCMatchCreatorTask();

    /** Virtual method Init **/
    virtual void SetParContainers();
    void SetPersistance(Bool_t pers) { fPersistance = pers; }
    Bool_t GetPersistance() { return fPersistance; }

    virtual InitStatus Init();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* opt);

    virtual void Finish();

    virtual void InitDataStages() = 0;

  protected:
    FairMCMatch* fMCMatch;

  private:
    std::map<std::string, TClonesArray*> fBranches;
    Bool_t fPersistance;
    TClonesArray* fMCLink;   //->
    Int_t fEventNr;

    InitStatus InitBranches();

    void Register();

    void Reset();

    void ProduceHits();

    FairMCMatchCreatorTask(const FairMCMatchCreatorTask&);
    FairMCMatchCreatorTask& operator=(const FairMCMatchCreatorTask&);

    ClassDef(FairMCMatchCreatorTask, 1);
};

#endif
