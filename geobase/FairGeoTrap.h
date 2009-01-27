#ifndef CBMGEOTRAP_H
#define CBMGEOTRAP_H

#include "FairGeoBasicShape.h"
#include "FairGeoRotation.h"

class FairGeoTransform;
class FairGeoVolume;

class FairGeoTrap : public FairGeoBasicShape {
private:
  FairGeoRotation intrinsicRot;
public:
  FairGeoTrap();
  ~FairGeoTrap();
  TArrayD* calcVoluParam(FairGeoVolume*);
  void calcVoluPosition(FairGeoVolume*,
            const FairGeoTransform&,const FairGeoTransform&);
  ClassDef(FairGeoTrap,0) // class for geometry shape TRAP
};

#endif  /* !CBMGEOTRAP_H */
