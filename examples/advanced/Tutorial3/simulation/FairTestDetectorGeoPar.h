/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef FAIRTESTDETECTORGEOPAR_H_
#define FAIRTESTDETECTORGEOPAR_H_

#include <Rtypes.h>             // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>         // for Bool_t

#include "FairParGenericSet.h"  // for FairParGenericSet

class TObjArray;
class FairParamList;
class TBuffer;
class TClass;
class TMemberInspector;

class FairTestDetectorGeoPar : public FairParGenericSet
{
  public:
    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray* fGeoSensNodes;

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray* fGeoPassNodes;

    FairTestDetectorGeoPar(const char* name = "FairTestDetectorGeoPar",
                           const char* title = "FairTestDetector Geometry Parameters",
                           const char* context = "TestDefaultContext");
    ~FairTestDetectorGeoPar(void);
    void clear(void);
    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);
    TObjArray* GetGeoSensitiveNodes() { return fGeoSensNodes; }
    TObjArray* GetGeoPassiveNodes() { return fGeoPassNodes; }

  private:
    FairTestDetectorGeoPar(const FairTestDetectorGeoPar&);
    FairTestDetectorGeoPar& operator=(const FairTestDetectorGeoPar&);

    ClassDef(FairTestDetectorGeoPar, 1);
};

#endif /* FAIRTESTDETECTORGEOPAR_H_ */
