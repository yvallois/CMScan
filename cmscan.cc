#include "cmscandetectorconstruction.hh"
#include "cmscanactioninitialization.hh"

#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
//TODO fichier config
int main(int argc,char** argv)
{

	// Detect interactive mode (if no arguments) and define UI session
  	G4UIExecutive* ui_executive = nullptr;
  	if ( argc == 1 ) {

	    ui_executive = new G4UIExecutive(argc, argv );
  	}

  	/// Choose the Random engine
  	G4Random::setTheEngine(new CLHEP::RanecuEngine);

  	/// Construct the default run manager
    auto* runManager = new G4RunManager;

	/// Set mandatory initialization classes
	runManager->SetUserInitialization(new CMScanDetectorConstruction());

	G4VModularPhysicsList* physicsList = new FTFP_BERT;
  	physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  	runManager->SetUserInitialization(physicsList);

  	runManager->SetUserInitialization(new CMScanActionInitialization("../data"));

  	///////////////////////////////////////////////////////////////////////////////////////////

	G4cout<<"OK => declaration et initialisation\n"<<G4endl;
	// in case something is wrong
	if ( G4RunManager::GetRunManager()->GetUserPhysicsList() == nullptr) {

		G4cerr << G4endl << " ***** ERROR ***** The PHYSICS LIST is not set ***** ERROR ***** " << G4endl;
		exit (-1);
	}
	if ( G4RunManager::GetRunManager()->GetUserDetectorConstruction() == nullptr) {

		G4cerr << G4endl << " ***** ERROR ***** The DETECTOR is not built ***** ERROR ***** " << G4endl;
		exit (-1);
	}
	if ( G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction() == nullptr) {

		G4cerr << G4endl << " ***** ERROR ***** The Primary generator is not defined ***** ERROR ***** " << G4endl;
		exit (-1);
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	/// Initialize visualization
	///////////////////////////////////////////////////////////////////////////////////////////

	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();

	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	// Process macro or start UI session
	if (ui_executive != nullptr) {

		// interactive mode
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		ui_executive->SessionStart();
		delete ui_executive;
	}
	else {

		// batch mode
		G4String command = "/control/execute ";
		std::string fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	}

	delete visManager;
	delete runManager;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
