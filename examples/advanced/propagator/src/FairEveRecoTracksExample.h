/********************************************************************************
 *    Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/*
 * FairEveRecoTracksExample.h
 *
 *  Created on: 16 cze 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef FAIREVERECOTRACKS_H_
#define FAIREVERECOTRACKS_H_

#include <Rtypes.h>         // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>     // for Bool_t, Int_t

#include "FairEveTracks.h"  // for FairEveTracks
#include "FairTask.h"       // for InitStatus

class FairRKPropagator;
class FairTutPropTrack;
class TDatabasePDG;
class TBuffer;
class TClass;
class TClonesArray;
class TMemberInspector;

class FairEveRecoTracksExample : public FairEveTracks
{
    TClonesArray *fContainerReco;
    TClonesArray *fContainerSim;
    TClonesArray *fHits1, *fHits2;
    Bool_t fShowPrimary;
    Bool_t fShowSecondary;
    Bool_t fDrawMC;
    Bool_t fUsePdg;
    Int_t fPdgCut;
    FairRKPropagator *fRK;
    TDatabasePDG *fPDG;

  protected:
    Bool_t CheckCuts(FairTutPropTrack *tr);
    void DrawTrack(Int_t id);

  public:
    FairEveRecoTracksExample();
    void Repaint();
    void SetDrawMC(Bool_t draw);
    void SetPdgCut(Int_t pdg, Bool_t use)
    {
        fPdgCut = pdg;
        fUsePdg = use;
    };
    void SetShowPrimSec(Bool_t prim, Bool_t sec)
    {
        fShowPrimary = prim;
        fShowSecondary = sec;
    }
    virtual InitStatus Init();
    virtual ~FairEveRecoTracksExample();
    ClassDef(FairEveRecoTracksExample, 0)
};

#endif /* FAIREVERECOTRACKS_H_ */
