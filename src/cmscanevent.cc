#include "cmscanevent.hh"
#include "cmscansensitivedetector.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"


CMTScanEventAction::CMTScanEventAction(CMTScanRunAction *run_action) :
        G4UserEventAction(),
        run_action_(run_action) {}


void CMTScanEventAction::BeginOfEventAction(const G4Event *event) {

    CMScanLcioWriter *lcwriter = run_action_->GetLcioWriter();
    lcwriter->ClearEvent();
    lcwriter->CreateEvent(event);
}


void CMTScanEventAction::EndOfEventAction(const G4Event *event) {

    CMScanLcioWriter *lcwriter = run_action_->GetLcioWriter();
    CMScanRootWriter *root_writer = run_action_->GetRootWriter();

    G4HCofThisEvent *col = event->GetHCofThisEvent();
    std::vector<CMScanHit*> hits;
    for (int i = 0; i < col->GetNumberOfCollections(); i++) {

        auto *hits_collection = dynamic_cast<HitsCollection*>(col->GetHC(i));
        std::vector<CMScanHit*> hits_per_chamber = *(hits_collection->GetVector());
        hits.insert(hits.end(), hits_per_chamber.begin(), hits_per_chamber.end());
    }

    if (!hits.empty()) {

        root_writer->AddHits(hits);
        lcwriter->AddHits(hits);
    }
}


