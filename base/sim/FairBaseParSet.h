/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef FAIRBASEPARSET_H
#define FAIRBASEPARSET_H

#include <Rtypes.h>             // for THashConsistencyHolder, ClassDef
#include <TGeoManager.h>   // IWYU pragma: keep needed by cint
#include <RtypesCore.h>         // for Double_t, UInt_t, Bool_t

/**
 * Parameter class for run
 * @author M. Al-Turany
 * @version 1
 * @since 12.10.04
 */
#include "FairParGenericSet.h"  // for FairParGenericSet

class FairParamList;
class FairPrimaryGenerator;
class TObjArray;
class TBuffer;
class TClass;
class TMemberInspector;

class FairBaseParSet : public FairParGenericSet
{
  public:
    /**
     * constructor
     * @param name :  Parameter set name
     * @param title:  Parameter set title
     * @param context:  Parameter set context
     */

    FairBaseParSet(const char* name = "FairBaseParSet",
                   const char* title = "Class for base parameter io",
                   const char* context = "BaseDefaultContext");
    /** dtor*/
    ~FairBaseParSet(void);
    /** clear*/
    void clear(void);
    /**
     * Fills all persistent data members into the list for write.
     * @param FairParamList : Parameter list to be filled
     */
    void putParams(FairParamList*);
    /**
     * Fills all persistent data members from the list after reading. The function
     * returns false, when a data member is not in the list.
     * @param FairParamList : Parameter list to be filled
     */

    Bool_t getParams(FairParamList*);
    /**
     * Set the detector list used in the simulation
     * @param array: TObjArray of detector
     */
    void SetDetList(TObjArray* array) { fDetList = array; }
    /**
     * Set the Generator used in the simulation
     * @param gen: Primary generator used in simulation
     */
    void SetGen(FairPrimaryGenerator* gen) { fPriGen = gen; }
    /**
     * Set the beam momentum (if any) used in the simulation
     * @param BMom: Beam Momentum in GeV/c
     */
    void SetBeamMom(Double_t BMom) { fBeamMom = BMom; }
    /**
     * Set the list of parameter containers used in a run
     * @param array: TObjArray of containers
     */
    void SetContListStr(TObjArray* list) { fContNameList = list; }
    /**
     * Set the random seed used in a run
     * @param RndSeed: Random Seed
     */
    void SetRndSeed(UInt_t RndSeed) { fRandomSeed = RndSeed; }
    /**
     *  Get the detector list used in the simulation
     */
    TObjArray* GetDetList() { return fDetList; }
    /**
     *  Get the Primery generator used in the simulation
     */
    FairPrimaryGenerator* GetPriGen() { return fPriGen; }
    /**
     *  Get the Beam Momentum used in the simulation (GeV/c)
     */
    Double_t GetBeamMom() { return fBeamMom; }
    /**
     *  Get the parameter container list used in this run
     */
    TObjArray* GetContList() { return fContNameList; }
    /**
     *  Get the Random Seed used in this run
     */
    UInt_t GetRndSeed() { return fRandomSeed; }

  protected:
    /// Detectors used in the simulation
    TObjArray* fDetList;
    /// Generator used for simulation
    FairPrimaryGenerator* fPriGen;
    /// Beam momentum (GeV/c)
    Double_t fBeamMom;
    /// List of parameter container names in the RUN
    TObjArray* fContNameList;
    /// Random Seed from gRandom
    UInt_t fRandomSeed;

    ClassDef(FairBaseParSet, 6);

  private:
    FairBaseParSet(const FairBaseParSet& L);
    FairBaseParSet& operator=(const FairBaseParSet&) { return *this; }
};

#endif /* !FAIRBASEPARSET_H */
