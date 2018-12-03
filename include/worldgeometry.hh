#ifndef Constants_h
#define Constants_h

#include <map>

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"


	const G4double RPC_Chip_Th = 1.6*mm;
	const G4double RPC_PCB_Th = 1.2*mm;
	const G4double RPC_MylarAnode_Th = 0.05*mm;
	const G4double RPC_CoatingAnode_Th = 0.05*mm; 
	const G4double RPC_GlassAnode_Th = 0.7*mm;
	const G4double RPC_GasChamber_Th = 1.2*mm;
	const G4double RPC_GlassCathode_Th = 1.1*mm;
	const G4double RPC_CoatingCathode_Th = 0.05*mm; 
	const G4double RPC_MylarCathode_Th = 0.18*mm;

	const G4double RPC_K7T_Th = 5*mm;
	const G4double RPC_K7B_Th = 5*mm;
	const G4double RPC_Inner_Th = 	RPC_Chip_Th +
									RPC_PCB_Th +
									RPC_MylarAnode_Th +
									RPC_CoatingAnode_Th +
									RPC_GlassAnode_Th +
									RPC_GasChamber_Th +
									RPC_GlassCathode_Th +
									RPC_CoatingCathode_Th +
									RPC_MylarCathode_Th;
	
	const G4double RPC_Th = RPC_K7T_Th + RPC_Inner_Th + RPC_K7B_Th;


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
