#include "cmscanrunaction.hh"
#include "cmscanprimarygeneratoraction.hh"


#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"


CMTScanRunAction::CMTScanRunAction(std::string output_name) :
        G4UserRunAction() {

	G4RunManager::GetRunManager()->SetPrintProgress(1000);
    lcio_writer_ = new CMScanLcioWriter(output_name + ".slcio");
    root_writer_ = new CMScanRootWriter(output_name + ".root");
}


CMTScanRunAction::~CMTScanRunAction() {

    delete lcio_writer_;
    delete root_writer_;
}


void CMTScanRunAction::BeginOfRunAction(const G4Run* run) {
	G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

    //inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(true);
}


void CMTScanRunAction::EndOfRunAction(const G4Run* ) {

	G4cout << "RunTime = " <<CMTScanPrimaryGeneratorAction::GetDuration()<< " seconds" << G4endl;
}


CMScanLcioWriter *CMTScanRunAction::GetLcioWriter() const {

    return lcio_writer_;
}

CMScanRootWriter *CMTScanRunAction::GetRootWriter() const {

    return root_writer_;
}
