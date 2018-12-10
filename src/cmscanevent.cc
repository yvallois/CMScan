#include "cmscanevent.hh"
#include "cmscansensitivedetector.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"


CMTScanEventAction::CMTScanEventAction(CMTScanRunAction *run_action) :
        G4UserEventAction(),
        run_action_(run_action) {}


void CMTScanEventAction::BeginOfEventAction(const G4Event *event) {

    CMScanLcioWriter *lcio_writer = run_action_->GetLcioWriter();
    lcio_writer->ClearEvent();
    lcio_writer->CreateEvent(event);
}


void CMTScanEventAction::EndOfEventAction(const G4Event *event) {

    CMScanLcioWriter *lcio_writer = run_action_->GetLcioWriter();
    CMScanRootWriter *root_writer = run_action_->GetRootWriter();

    G4HCofThisEvent *col = event->GetHCofThisEvent();
    std::vector<CMScanHit*> hits;
    std::cout << "event number : " << event->GetEventID() << std::endl;
    for (int i = 0; i < col->GetNumberOfCollections(); i++) {

        auto *hits_collection = dynamic_cast<HitsCollection*>(col->GetHC(i));
        std::cout << "hit collection name : " << hits_collection->GetName() << std::endl;
        std::vector<CMScanHit*> hits_per_chamber = *(hits_collection->GetVector());
        hits.insert(hits.end(), hits_per_chamber.begin(), hits_per_chamber.end());
    }

    if (!hits.empty()) {

        root_writer->AddHits(hits);
        lcio_writer->AddHits(hits);
    }
}


