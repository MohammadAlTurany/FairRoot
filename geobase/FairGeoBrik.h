/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef FAIRGEOBRIK_H
#define FAIRGEOBRIK_H

#include <Rtypes.h>             // for THashConsistencyHolder, ClassDef

#include "FairGeoBasicShape.h"  // for FairGeoBasicShape

class FairGeoTransform;
class FairGeoVolume;
class TArrayD;
class TBuffer;
class TClass;
class TMemberInspector;

class FairGeoBrik : public FairGeoBasicShape
{
  public:
    FairGeoBrik();
    ~FairGeoBrik();
    TArrayD* calcVoluParam(FairGeoVolume*);
    void calcVoluPosition(FairGeoVolume*, const FairGeoTransform&, const FairGeoTransform&);
    ClassDef(FairGeoBrik, 0);   // class for geometry shape BOX or BRIK
};

#endif /* !FAIRGEOBRIK_H */
