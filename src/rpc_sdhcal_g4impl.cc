//
// Created by vallois on 05/12/18.
//

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SDParticleFilter.hh"

#include "rpc_sdhcal_g4impl.h"
#include "cmscansensitivedetector.hh"


Rpc_SDHCAL_G4impl::Rpc_SDHCAL_G4impl(int chamber_id, int stack_id) :
        Rpc_SDHCAL(chamber_id, stack_id),
        rpc_logic_(nullptr),
        name_("") {


    name_="RPC_";
    name_.append(std::to_string(chamber_id_));

    G4LogicalVolumeStore* store = G4LogicalVolumeStore::GetInstance() ;
    if (store->GetVolume(name_ , false)) {

        std::cout << "Error : RPC with the same name" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Rpc_SDHCAL_G4impl::Rpc_SDHCAL_G4impl::PrintGeometry() {

    std::cout << "*** chamber id : " << chamber_id_ << std::endl;
    std::cout << "*** stack id : " << stack_id_ << std::endl;
    std::cout << "*** translation" << translation_ << std::endl;
    std::cout << "*** rotation" << rotation_ << std::endl;
    std::cout << "*** nb pad x : " << num_pad_x_ << "\t nb pad y : " << num_pad_y_ << std::endl;
    std::cout << "*** pad size : " << pad_size_ << std::endl << std::endl;
}

void Rpc_SDHCAL_G4impl::Rpc_SDHCAL_G4impl::Build() {

    const G4double chip_thickness = 1.6*mm;
    const G4double pcb_thickness = 1.2*mm;
    const G4double mylar_anode_thickness = 0.05*mm;
    const G4double coating_anode_thickness = 0.05*mm;
    const G4double glass_anode_thickness = 0.7*mm;
    const G4double gas_chamber_thickness = 1.2*mm;
    const G4double glass_cathode_thickness = 1.1*mm;
    const G4double coating_cathode_thickness = 0.05*mm;
    const G4double mylar_cathode_thickness = 0.18*mm;
    const G4double cassette_thickness = 5*mm;

    G4NistManager *nist_manager = G4NistManager::Instance();

    auto *rpc_solid = new G4Box(name_, chamber_size_[0]*0.5, chamber_size_[1]*0.5, chamber_size_[2]*0.5);
    auto *rpc_logic = new G4LogicalVolume(rpc_solid, nist_manager->FindOrBuildMaterial("G4_AIR"), name_);

    G4Box *cassette_top_solid = new G4Box("CassetteTopSolid"+std::to_string(chamber_id_), chamber_size_[0]*0.5, chamber_size_[1]*0.5, cassette_thickness*0.5);
    G4Box *chip_solid = new G4Box("ChipSolid"+std::to_string(chamber_id_), chamber_size_[0]*0.5, chamber_size_[1]*0.5, chip_thickness*0.5);
    G4Box *pcb_solid = new G4Box("PCBSolid"+std::to_string(chamber_id_),chamber_size_[0]*0.5, chamber_size_[1]*0.5, pcb_thickness*0.5);
    G4Box *mylar_anode_solid = new G4Box("MylarAnodeSolid"+std::to_string(chamber_id_), chamber_size_[0]*0.5, chamber_size_[1]*0.5, mylar_anode_thickness*0.5);
    G4Box *coating_anode_solid = new G4Box("CoatingAnodeSolid"+std::to_string(chamber_id_), chamber_size_[0]*0.5, chamber_size_[1]*0.5, coating_anode_thickness*0.5);
    G4Box *glass_anode_solid = new G4Box("GlassAnodeSolid"+std::to_string(chamber_id_), chamber_size_[0]*0.5, chamber_size_[1]*0.5, glass_anode_thickness*0.5);
    G4Box *gas_chamber_solid = new G4Box("GasSolid"+std::to_string(chamber_id_), chamber_size_[0]*0.5, chamber_size_[1]*0.5, gas_chamber_thickness*0.5);
    G4Box *glass_cathode_solid = new G4Box("GlassCathodeSolid"+std::to_string(chamber_id_), chamber_size_[0]*0.5, chamber_size_[1]*0.5, glass_cathode_thickness*0.5);
    G4Box *coating_cathode_solid = new G4Box("CoatingCathodeSolid"+std::to_string(chamber_id_), chamber_size_[0]*0.5, chamber_size_[1]*0.5, coating_cathode_thickness*0.5);
    G4Box *mylar_cathode_solid = new G4Box("MylarCathodeSolid"+std::to_string(chamber_id_), chamber_size_[0]*0.5, chamber_size_[1]*0.5, mylar_cathode_thickness*0.5);
    G4Box *cassette_bot_solid = new G4Box("CassetteBotSolid"+std::to_string(chamber_id_), chamber_size_[0]*0.5, chamber_size_[1]*0.5, cassette_thickness*0.5);

    G4LogicalVolume *cassette_top_logic = new G4LogicalVolume(cassette_top_solid, nist_manager->FindOrBuildMaterial("alveolar_core"), "CassetteTopLogic"+std::to_string(chamber_id_));
    G4LogicalVolume *chip_logic = new G4LogicalVolume(chip_solid, nist_manager->FindOrBuildMaterial("g10"), "ChipLogic"+std::to_string(chamber_id_));
    G4LogicalVolume *pcb_logic = new G4LogicalVolume(pcb_solid, nist_manager->FindOrBuildMaterial("g10"), "PCBLogic"+std::to_string(chamber_id_));
    G4LogicalVolume *mylar_anode_logic = new G4LogicalVolume(mylar_anode_solid, nist_manager->FindOrBuildMaterial("G4_MYLAR"), "MylarAnodeLogic"+std::to_string(chamber_id_));
    G4LogicalVolume *coating_anode_logic= new G4LogicalVolume(coating_anode_solid, nist_manager->FindOrBuildMaterial("G4_GRAPHITE"), "CoatingAnodeLogic"+std::to_string(chamber_id_));
    G4LogicalVolume *glass_anode_logic = new G4LogicalVolume(glass_anode_solid, nist_manager->FindOrBuildMaterial("G4_Pyrex_Glass"), "GlassAnodeLogic"+std::to_string(chamber_id_));
    G4LogicalVolume *gas_chamber_logic = new G4LogicalVolume(gas_chamber_solid, nist_manager->FindOrBuildMaterial("RPC_Gaz"), "GasChamberLogic"+std::to_string(chamber_id_));
    G4LogicalVolume *glass_cathode_logic = new G4LogicalVolume(glass_cathode_solid, nist_manager->FindOrBuildMaterial("G4_Pyrex_Glass"), "GlassCathodeLogic"+std::to_string(chamber_id_));
    G4LogicalVolume *coating_cathode_logic = new G4LogicalVolume(coating_cathode_solid, nist_manager->FindOrBuildMaterial("G4_GRAPHITE"), "CoatingCathodeLogic"+std::to_string(chamber_id_));
    G4LogicalVolume *mylar_cathode_logic = new G4LogicalVolume(mylar_cathode_solid, nist_manager->FindOrBuildMaterial("G4_MYLAR"), "MylarCathodeLogic"+std::to_string(chamber_id_));
    G4LogicalVolume *cassette_bot_logic = new G4LogicalVolume(cassette_bot_solid, nist_manager->FindOrBuildMaterial("alveolar_core"), "CassetteBotLogic"+std::to_string(chamber_id_));

    G4double position_z = chamber_size_[2]*0.5;
    position_z = position_z - cassette_thickness*0.5;
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, position_z), cassette_top_logic, "CassetteTopPhysic"+std::to_string(chamber_id_), rpc_logic, false, 0, true);
    position_z = position_z - cassette_thickness*0.5 - chip_thickness*0.5;
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, position_z), chip_logic, "ChipPhysic"+std::to_string(chamber_id_), rpc_logic, false, 0, true);
    position_z = position_z-chip_thickness*0.5-pcb_thickness*0.5;
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, position_z), pcb_logic, "PCBPhysic"+std::to_string(chamber_id_), rpc_logic, false, 0, true);
    position_z = position_z-pcb_thickness*0.5-mylar_anode_thickness*0.5;
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, position_z), mylar_anode_logic, "MylarAnodePhysic"+std::to_string(chamber_id_), rpc_logic, false, 0, true);
    position_z = position_z-mylar_anode_thickness*0.5-coating_anode_thickness*0.5;
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, position_z), coating_anode_logic, "CoatingAnodePhysic"+std::to_string(chamber_id_), rpc_logic, false, 0, true);
    position_z = position_z-coating_anode_thickness*0.5-glass_anode_thickness*0.5;
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, position_z), glass_anode_logic, "GlassAnodePhysic"+std::to_string(chamber_id_), rpc_logic, false, 0, true);
    position_z = position_z-glass_anode_thickness*0.5-gas_chamber_thickness*0.5;
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, position_z), gas_chamber_logic, "GazChamberPhysic"+std::to_string(chamber_id_), rpc_logic, false, 0, true);
    position_z = position_z-gas_chamber_thickness*0.5-glass_cathode_thickness*0.5;
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, position_z), glass_cathode_logic, "GlassCathodePhysic"+std::to_string(chamber_id_), rpc_logic, false, 0, true);
    position_z = position_z-glass_cathode_thickness*0.5-coating_cathode_thickness*0.5;
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, position_z), coating_cathode_logic, "CoatingCathodePhysic"+std::to_string(chamber_id_), rpc_logic, false, 0, true);
    position_z = position_z-coating_cathode_thickness*0.5-mylar_cathode_thickness*0.5;
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, position_z), mylar_cathode_logic, "MylarCathodePhysic"+std::to_string(chamber_id_), rpc_logic, false, 0, true);
    position_z = position_z-mylar_cathode_thickness*0.5-cassette_thickness*0.5;
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, position_z), cassette_bot_logic, "CassetteBotPhysic"+std::to_string(chamber_id_), rpc_logic, false, 0, true);

    rpc_logic_ = rpc_logic;

    auto *sensitive_detector = new CMScanSensitiveDetector("SensitiveDetector_" + std::to_string(chamber_id_),
                                                           "Hit_" + std::to_string(chamber_id_), chamber_id_);
    G4SDParticleFilter *muon_filter = new G4SDParticleFilter("MuonFilter");
    muon_filter->add("mu+");
    muon_filter->add("mu-");
    sensitive_detector->SetFilter(muon_filter);

    G4SDManager::GetSDMpointer()->AddNewDetector(sensitive_detector);
    gas_chamber_logic->SetSensitiveDetector(sensitive_detector);
}

void Rpc_SDHCAL_G4impl::AddChamber(G4LogicalVolume *mother_logic) {

    auto *rotation_matrix = new G4RotationMatrix(rotation_[0], rotation_[1], rotation_[2]);
    affine_transform_ = G4AffineTransform(rotation_matrix, translation_);
    new G4PVPlacement(rotation_matrix, translation_, rpc_logic_, name_, mother_logic, false, 0, true);
}

void Rpc_SDHCAL_G4impl::GlobalToRPCCoordinate(G4ThreeVector &coordinate) {

    affine_transform_.TransformPoint(coordinate);

    coordinate[0] = static_cast<int>((coordinate[0]) / pad_size_) * pad_size_ + chamber_size_[0] * 0.5;
    coordinate[1] = static_cast<int>((coordinate[1]) / pad_size_) * pad_size_ + chamber_size_[1] * 0.5;

    affine_transform_.Invert().TransformPoint(coordinate);
}
