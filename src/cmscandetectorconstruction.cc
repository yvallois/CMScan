#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"

#include "cmscandetectorconstruction.hh"
#include "cmscansensitivedetector.hh"
#include "rpc_sdhcal_g4impl.h"

using namespace CLHEP;

CMScanDetectorConstruction::CMScanDetectorConstruction() : world_geometry_(WorldGeometry::Instance()) {}


G4VPhysicalVolume* CMScanDetectorConstruction::Construct() {

	DefineMaterials();
	return DefineVolumes();
}

void CMScanDetectorConstruction::DefineMaterials() {

	G4NistManager *nist_rpc_materials = G4NistManager::Instance();

	///elements of the materials used
	G4double a = 1.01 * g / mole;
	G4Element *elH = new G4Element("Hydrogen", "H", 1, a);

	a = 12.01 * g / mole;
	G4Element *elC = new G4Element("Carbon", "C", 6, a);

	a = 16.00 * g / mole;
	G4Element *elO = new G4Element("Oxygen", "O", 8, a);

	a = 32.06 * g / mole;
	G4Element *elS = new G4Element("Sulfur", "S", 16, a);

	a = 19.00 * g / mole;
	G4Element *elF = new G4Element("Fluor", "F", 9, a);

	a = 55.845 * g / mole;
	G4Element *elFe = new G4Element("Fer", "Fe", 26, a);

	a = 54.938 * g / mole;
	G4Element *elMn = new G4Element("Manganese", "Mn", 25, a);

	a = 28.086 * g / mole;
	G4Element *elSi = new G4Element("Silicium", "Si", 14, a);

	a = 30.974 * g / mole;
	G4Element *elP = new G4Element("Phosphore", "P", 15, a);

	a = 51.996 * g / mole;
	G4Element *elCr = new G4Element("Chrome", "Cr", 24, a);

	a = 58.693 * g / mole;
	G4Element *elNi = new G4Element("Nickel", "Ni", 28, a);

	a = 63.546 * g / mole;
	G4Element *elCu = new G4Element("Cuivre", "Cu", 29, a);

	a = 26.982 * g / mole;
	G4Element *elAl = new G4Element("Aluminium", "Al", 13, a);

	a = 92.906 * g / mole;
	G4Element *elNb = new G4Element("Niobium", "Nb", 41, a);

	///Hexafluorure de souffre (SF6)
	G4double SF6_Density = 6.16e-3 * g / cm3;
	G4Material *SF6 = new G4Material("SF6", SF6_Density, 2);
	SF6->AddElement(elS, 1);
	SF6->AddElement(elF, 6);

	///Nid d'abeille
	G4Material *alveolar_core = new G4Material("alveolar_core", 0.764*g/cm3, 1);
	alveolar_core->AddElement(elAl, 1);

	///Isobutane
	G4double Isobutane_Density = 2.51e-3 * g / cm3;
	G4Material *Isobutane = new G4Material("Isobutane", Isobutane_Density, 2);
	Isobutane->AddElement(elC, 4);
	Isobutane->AddElement(elH, 10);

	///CO2
	G4double CO2_Density = 1.87e-3 * g / cm3;
	G4Material *CO2 = new G4Material("CO2", CO2_Density, 2);
	CO2->AddElement(elC, 1);
	CO2->AddElement(elO, 2);

	///Tétrafluoroéthane (TFE)
	G4double TFE_Density = 4.28e-3 * g / cm3;
	G4Material *TFE = new G4Material("TFE", TFE_Density, 3);
	TFE->AddElement(elC, 2);
	TFE->AddElement(elH, 2);
	TFE->AddElement(elF, 4);

	///RPCGaz
	G4double RPC_Gaz_Density = (SF6_Density*0.02+CO2_Density*0.05+TFE_Density*0.93);
	G4Material *RPC_GasChamber_Material = new G4Material("RPC_Gaz", RPC_Gaz_Density, 3);
	RPC_GasChamber_Material->AddMaterial(TFE, 0.93);
	RPC_GasChamber_Material->AddMaterial(CO2, 0.05);
	RPC_GasChamber_Material->AddMaterial(SF6, 0.02);

	///g10
	G4double epoxydensity = 1.3 * g / cm3;
	G4Material *epoxy = new G4Material("epoxy", epoxydensity, 3);
	epoxy->AddElement(elC, 15);
	epoxy->AddElement(elO, 7);
	epoxy->AddElement(elH, 44);

	G4Material *Si02 = nist_rpc_materials->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
	G4Material *Cl = nist_rpc_materials->FindOrBuildMaterial("G4_Cl");

	G4double g10density = 1.7 * g / cm3;
	G4Material *g10 = new G4Material("g10", g10density, 3);
	g10->AddMaterial(epoxy, 14.7 * perCent);
	g10->AddMaterial(Cl, 8 * perCent);
	g10->AddMaterial(Si02, 77.3 * perCent);

	///Corten-A steel
	G4double cortenSteelDensity = 7.8 * g / cm3;
	G4Material *cortenSteel = new G4Material("cortenSteel", cortenSteelDensity, 11);
	cortenSteel -> AddElement(elFe, 95.88 * perCent);
	cortenSteel -> AddElement(elC, 0.12 * perCent);
	cortenSteel -> AddElement(elMn, 0.5 * perCent);
	cortenSteel -> AddElement(elSi, 0.75 * perCent);
	cortenSteel -> AddElement(elS, 0.03 * perCent);
	cortenSteel -> AddElement(elP, 0.15 * perCent);
	cortenSteel -> AddElement(elCr, 1.25 * perCent);
	cortenSteel -> AddElement(elNi, 0.65 * perCent);
	cortenSteel -> AddElement(elCu, 0.55 * perCent);
	cortenSteel -> AddElement(elAl, 0.06 * perCent);
	cortenSteel -> AddElement(elNb, 0.06 * perCent);
}

G4VPhysicalVolume* CMScanDetectorConstruction::DefineVolumes() {

	G4NistManager *nistManager = G4NistManager::Instance();

	/////////////////////////////////////////////////////
	/// World
	/////////////////////////////////////////////////////

    Hep3Vector world_size = world_geometry_->GetWorldSize();
    G4Box *world_solid = new G4Box("WorldSolid", world_size[0] * 0.5, world_size[1] * 0.5, world_size[2] * 0.5);
    G4LogicalVolume *world_logic = new G4LogicalVolume(world_solid, nistManager->FindOrBuildMaterial("G4_AIR"), "WorldLogic");
    G4VPhysicalVolume *world_physic = new G4PVPlacement(nullptr, G4ThreeVector(), world_logic, "WorldPhysic", nullptr, false, 0, true);

    ///Construction du detecteur et des filtres
    auto num_rpc = world_geometry_->GetNumRpc();
    for (int i = 0; i < num_rpc; ++i) {

        Rpc_base *rpc_base_ptr = world_geometry_->GetRpc(i);
        if (rpc_base_ptr->GetRpcType() == "SDHCALG4IMPL"){

            auto rpc_ptr = dynamic_cast<Rpc_SDHCAL_G4impl*>(rpc_base_ptr);
            rpc_ptr->Build();
            rpc_ptr->AddChamber(world_logic);
        }
    }

/*
    /////////////////////////////////////////////////////
    /// Container
    /////////////////////////////////////////////////////

    G4double steelThickness = 3 * mm;
    G4Box* outerContainer = new G4Box("OuterContainer", Detector_Size_X*0.5, Detector_Size_Y*0.5, Dist_Mid*0.5);
    G4Box* innerContainer = new G4Box("InnerContainer", Detector_Size_X*0.5 - steelThickness, Detector_Size_Y*0.5 - steelThickness, Dist_Mid*0.5 - steelThickness);
    G4SubtractionSolid* Container = new G4SubtractionSolid("Container", outerContainer, innerContainer);

    G4LogicalVolume* Container_Log = new G4LogicalVolume(Container, nistManager->FindOrBuildMaterial("cortenSteel"), "Container_Log");
    new G4PVPlacement (nullptr, G4ThreeVector(), Container_Log, "Container_Phy", world_logic, false, 0, true);
*/
    /////////////////////////////////////////////////////
    /// Target
    /////////////////////////////////////////////////////

    G4Box* Target = new G4Box("Target", 15*cm, 15*cm, 15*cm);
    G4LogicalVolume* Target_Log = new G4LogicalVolume(Target, nistManager->FindOrBuildMaterial("G4_Pb"), "Target_Log");
    new G4PVPlacement (nullptr, G4ThreeVector(-10*cm,-20*cm,-50*cm), Target_Log, "Target_Phy", world_logic, false, 0, true);

    return world_physic;
}

CMScanDetectorConstruction::~CMScanDetectorConstruction() {
    WorldGeometry::Kill();
}
