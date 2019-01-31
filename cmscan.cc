#include <iostream>

#include <boost/program_options.hpp>

#include "G4UIterminal.hh"
#include "G4RunManager.hh"
#include "FTFP_BERT.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "cmscandetectorconstruction.hh"
#include "cmscanactioninitialization.hh"
#include "worldgeometry.h"

namespace po = boost::program_options;

void conflicting_options(const po::variables_map& vm, const char* opt1, const char* opt2);

WorldGeometry *WorldGeometry::world_geometry_ = nullptr;

int main(int argc,char** argv)
{
    try {
        /// Parse program options
        po::options_description description("Allowed options");
        description.add_options()
                ("help", "produce help message")
                ("version", "output the version number")
                ("config", po::value<std::string>(), "path/to/json/my_json.json")
                ("batch,B", po::value<std::string>(), "Launch in batch mode")
                ("interactive,I", "Launch in interactive mode")
                ("gui,G", "Launch in gui mode");

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
                options(description).run(), vm);
        po::notify(vm);

        conflicting_options(vm, "batch", "interactive");
        conflicting_options(vm, "batch", "gui");
        conflicting_options(vm, "interactive", "gui");

        if (vm.count("help")) {

            std::cout << "Usage: options_description [options]\n";
            std::cout << description;
            return 0;
        }

        if (vm.count("config"))
            WorldGeometry::Instance(vm["config"].as<std::string>());
        else
            throw std::logic_error(std::string("Error : configuration file is missing"));


        /// Choose the Random engine
        G4Random::setTheEngine(new CLHEP::RanecuEngine);

        /// Construct the default run manager
        auto *run_manager = new G4RunManager;

        /// Set mandatory initialization classes
        run_manager->SetUserInitialization(new CMScanDetectorConstruction());
        G4VModularPhysicsList *physicsList = new FTFP_BERT;
        physicsList->RegisterPhysics(new G4StepLimiterPhysics());
        run_manager->SetUserInitialization(physicsList);
        run_manager->SetUserInitialization(new CMScanActionInitialization("../data"));
        run_manager->Initialize();

        /// in case something is wrong
        if (G4RunManager::GetRunManager()->GetUserPhysicsList() == nullptr)
            throw std::logic_error(std::string("Error :  The PHYSICS LIST is not set"));

        if (G4RunManager::GetRunManager()->GetUserDetectorConstruction() == nullptr)
            throw std::logic_error(std::string("Error :  The DETECTOR is not built"));

        if (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction() == nullptr)
            throw std::logic_error(std::string("Error :  The Primary generator is not defined"));


        /// launcher
        G4UImanager *ui_manager = G4UImanager::GetUIpointer();
        if (vm.count("batch")){

            std::cout << "file of commands : " << vm["batch"].as<std::string>() << std::endl;
            std::string command = "/control/execute ";
            std::string macro_file_name = vm["batch"].as<std::string>();
            ui_manager->ApplyCommand(command + macro_file_name);
        }

        if (vm.count("interactive")){

            G4UIsession *session = new G4UIterminal;
            session->SessionStart();
            delete session;
        }

        if (vm.count("gui")){

            G4UIExecutive *ui_executive = new G4UIExecutive(argc, argv);
            G4VisManager *vis_manager = new G4VisExecutive;
            vis_manager->Initialize();
            ui_manager->ApplyCommand("/control/execute init_vis.mac");
            ui_executive->SessionStart();

            delete ui_executive;
            delete vis_manager;
        }

        delete run_manager;
    }
    catch(std::exception& e) {

            std::cout << e.what() << "\n";
            return 1;
    }

}

/**
 * @brief Function used to check that 'opt1' and 'opt2' are not specified at the same time.
 * @param vm map which store input variables
 * @param opt1 first input option
 * @param opt2 second input option
 */
void conflicting_options(const po::variables_map& vm, const char* opt1, const char* opt2) {

    if (vm.count(opt1) && !vm[opt1].defaulted() && vm.count(opt2) && !vm[opt2].defaulted())
        throw std::logic_error(std::string("Conflicting options '") + opt1 + "' and '" + opt2 + "'.");
}
