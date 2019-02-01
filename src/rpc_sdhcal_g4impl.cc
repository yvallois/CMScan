//
// Created by vallois on 05/12/18.
//

#include <set>

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SDChargedFilter.hh"

#include "rpc_sdhcal_g4impl.h"
#include "cmscansensitivedetector.hh"


Rpc_SDHCAL_G4impl::Rpc_SDHCAL_G4impl(int chamber_id, int stack_id, const std::string &rpc_type) :
        Rpc_SDHCAL(chamber_id, stack_id, rpc_type),
        rpc_logic_(nullptr),
        name_(""),
        noise_rate_(2),
        real_distribution(std::uniform_real_distribution<>(0, 1)) {


    name_="RPC_";
    name_.append(std::to_string(chamber_id_));

    G4LogicalVolumeStore* store = G4LogicalVolumeStore::GetInstance() ;
    if (store->GetVolume(name_ , false)) {

        std::cout << "Error : RPC with the same name" << std::endl;
        exit(EXIT_FAILURE);
    }

    noise_distribution_ = std::binomial_distribution<int>(9, num_pad_x_ * num_pad_y_ * noise_rate_ * 600e-9);
    gen_noise_distribution_ = std::uniform_int_distribution<>(0, num_pad_x_ - 1);
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
    G4SDChargedFilter *filter = new G4SDChargedFilter("ChargedFilter");
    sensitive_detector->SetFilter(filter);

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


std::vector<CMScanDigit> Rpc_SDHCAL_G4impl::Digitize(std::vector<CMScanHit *> &hits) {

    std::vector<CMScanDigit> digits{};
    std::vector<int> time_processed_noise;
    for (auto &hit : hits){

        ///Add noise
        auto time = static_cast<int>(hit->GetTime()/200);
        std::set<std::pair<int, int>> temp;
        int K = hit->GetChamberID();
        if (std::none_of(time_processed_noise.begin(), time_processed_noise.end(), [&](int i){return i == time;})) {

            time_processed_noise.push_back(time);
            int num_noise = noise_distribution_(generator_);
            for (int i = 0; i < num_noise; ++i)
                temp.insert({gen_noise_distribution_(generator_), gen_noise_distribution_(generator_)});
        }

        ///Efficiency
        if (real_distribution(generator_) < 0.95) {

            ///Multiplicity
            double position_x = affine_transform_.TransformPoint(hit->GetPos())[0];
            double position_y = affine_transform_.TransformPoint(hit->GetPos())[1];
            double pos_on_pad_x = position_x - static_cast<int>(position_x);
            double pos_on_pad_y = position_y - static_cast<int>(position_y);

            auto I = static_cast<int>((position_x + chamber_size_[0] * 0.5) / pad_size_);
            auto J = static_cast<int>((position_y + chamber_size_[1] * 0.5) / pad_size_);

            temp.insert({I, J});

            double rand_number = real_distribution(generator_);
            if (rand_number > 0.99) {
                // Multiplicity 4
                if (pos_on_pad_x - pos_on_pad_x / 2 <= 0 and pos_on_pad_y - pos_on_pad_y / 2 <= 0) {
                    temp.insert({I - 1, J});
                    temp.insert({I, J - 1});
                    temp.insert({I - 1, J - 1});
                } else {
                    if (pos_on_pad_x - pos_on_pad_x / 2 > 0 and pos_on_pad_y - pos_on_pad_y / 2 > 0) {
                        temp.insert({I + 1, J});
                        temp.insert({I, J + 1});
                        temp.insert({I + 1, J + 1});
                    } else {
                        if (pos_on_pad_x - pos_on_pad_x / 2 > 0 and pos_on_pad_y - pos_on_pad_y / 2 <= 0) {
                            temp.insert({I + 1, J});
                            temp.insert({I, J - 1});
                            temp.insert({I + 1, J - 1});
                        } else {
                            temp.insert({I - 1, J});
                            temp.insert({I, J + 1});
                            temp.insert({I - 1, J + 1});
                        }
                    }
                }
            } else {
                if (rand_number > 0.95) {
                    // Multiplicity 3
                    if (pos_on_pad_x - pos_on_pad_x / 2 <= 0 and pos_on_pad_y - pos_on_pad_y / 2 <= 0) {
                        temp.insert({I - 1, J});
                        temp.insert({I, J - 1});
                    } else {
                        if (pos_on_pad_x - pos_on_pad_x / 2 > 0 and pos_on_pad_y - pos_on_pad_y / 2 > 0) {
                            temp.insert({I + 1, J});
                            temp.insert({I, J + 1});
                        } else {
                            if (pos_on_pad_x - pos_on_pad_x / 2 > 0 and pos_on_pad_y - pos_on_pad_y / 2 <= 0) {
                                temp.insert({I + 1, J});
                                temp.insert({I, J - 1});
                            } else {
                                temp.insert({I - 1, J});
                                temp.insert({I, J + 1});
                            }
                        }
                    }
                } else {
                    if (rand_number > 0.63) {
                        // Multiplicity 2
                        double min_distance = std::min(std::initializer_list<double>{pos_on_pad_x,
                                                                                     pos_on_pad_y,
                                                                                     pad_size_ - pos_on_pad_x,
                                                                                     pad_size_ - pos_on_pad_y});
                        if (pos_on_pad_x == min_distance)
                            temp.insert({I - 1, J});
                        else {
                            if (pos_on_pad_y == min_distance)
                                temp.insert({I, J - 1});
                            else {
                                if (pad_size_ - pos_on_pad_x == min_distance)
                                    temp.insert({I + 1, J});
                                else
                                    temp.insert({I, J + 1});
                            }
                        }
                    }
                }
            }
        }
        //TODO supprimer predigit
        std::vector<std::pair<int, int>> predigit (temp.size());
        auto new_end = std::copy_if(temp.begin(), temp.end(), predigit.begin(), [](std::pair<int, int> ij)->bool{return ij.first>=0 and ij.first<=95 and ij.second>=0 and ij.second<=95;});
        predigit.resize(static_cast<unsigned long>(std::distance(predigit.begin(), new_end)));

        int position[3]{};
        for (auto &it : predigit){

            position[0] = it.first;
            position[1] = it.second;
            position[2] = K;
            digits.emplace_back(position, time);
        }
    }
    return digits;
}
