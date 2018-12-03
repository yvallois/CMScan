#include "cmscanactioninitialization.hh"
#include "cmscanprimarygeneratoraction.hh"
#include "cmscanrunaction.hh"
#include "cmscanevent.hh"
#include "cmscanstackingaction.hh"


CMScanActionInitialization::CMScanActionInitialization(std::string output_name) : G4VUserActionInitialization(),
                                                                                  output_name_(output_name) {
}

CMScanActionInitialization::~CMScanActionInitialization() = default;


void CMScanActionInitialization::BuildForMaster() const {

    auto *runAction = new CMTScanRunAction(output_name_);
    SetUserAction(runAction);
}


void CMScanActionInitialization::Build() const {

    SetUserAction(new CMTScanPrimaryGeneratorAction("../cmake-build-debug/setup.file"));

    auto *run_action = new CMTScanRunAction(output_name_);
    SetUserAction(run_action);

    auto *eventAction = new CMTScanEventAction(run_action);
    SetUserAction(eventAction);

    auto *stackingAction = new CMTScanStackingAction;
    SetUserAction(stackingAction);
}