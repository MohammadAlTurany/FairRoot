/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef PIXELPOINT_H
#define PIXELPOINT_H 1

#include <Rtypes.h>       // for THashConsistencyHolder, ClassDef
#include <TVector3.h>     // for TVector3
#include <RtypesCore.h>   // for Double_t, Int_t, Option_t

#include "FairMCPoint.h"  // for FairMCPoint

class TBuffer;
class TClass;
class TMemberInspector;

class PixelPoint : public FairMCPoint
{
  public:
    /** Default constructor **/
    PixelPoint();

    /** Constructor with arguments
     *@param trackID  Index of MCTrack
     *@param detID    Detector ID
     *@param pos      Ccoordinates at entrance to active volume [cm]
     *@param mom      Momentum of track at entrance [GeV]
     *@param tof      Time since event start [ns]
     *@param length   Track length since creation [cm]
     *@param eLoss    Energy deposit [GeV]
     **/
    PixelPoint(Int_t trackID, Int_t detID, TVector3 pos, TVector3 mom, Double_t tof, Double_t length, Double_t eLoss);

    /** Destructor **/
    virtual ~PixelPoint();

    /** Output to screen **/
    virtual void Print(const Option_t* opt) const;

  private:
    /** Copy constructor **/
    PixelPoint(const PixelPoint& point);
    PixelPoint operator=(const PixelPoint& point);

    ClassDef(PixelPoint, 1);
};

#endif
