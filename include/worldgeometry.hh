#ifndef Constants_h
#define Constants_h

#include <map>

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

struct ChamberGeometry{
    int chamber_id;
    G4ThreeVector translation;
    G4ThreeVector rotation;
    int nb_pad_x;
    int nb_pad_y;
    int pad_size;
};

class WorldGeometry
{
public:

    static WorldGeometry *Instance();

    static void Kill();

    inline G4ThreeVector const getWorldSize(){return world_size_;}
    inline G4ThreeVector const getDetectorSize(){return detector_size_;}
    inline unsigned long GetChamberNumber(){return chambers_geometry_.size();}
    inline ChamberGeometry const GetChamberGeometry(int chamber_number){return chambers_geometry_[chamber_number];}

private:
	WorldGeometry();
	~WorldGeometry() = default;
	void GeometryReader();
	void PrintGeometry();

private:
    G4ThreeVector world_size_;
    G4ThreeVector detector_size_{};
    std::map<int, ChamberGeometry> chambers_geometry_;
    static WorldGeometry *world_geometry_;
};
#endif
