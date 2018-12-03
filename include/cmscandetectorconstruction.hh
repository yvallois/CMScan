#ifndef CMTScanDetectorConstruction_h
#define CMTScanDetectorConstruction_h 1

#include <map>

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "worldgeometry.hh"
#include "cmscanrpc.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.
class CMScanDetectorConstruction : public G4VUserDetectorConstruction {
public:
    CMScanDetectorConstruction();
    ~CMScanDetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

private:
    std::vector<CMScanRPC*> rpc_list_;
    std::string geometry_file_;
    WorldGeometry *world_geometry_;
};
#endif

