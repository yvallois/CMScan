#include "cmscanevent.hh"
#include "cmscansensitivedetector.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"


CMTScanEventAction::CMTScanEventAction(CMTScanRunAction *run_action) :
        G4UserEventAction(),
        run_action_(run_action),
        world_geometry_(nullptr) {

    world_geometry_ = WorldGeometry::Get();
}


void CMTScanEventAction::BeginOfEventAction(const G4Event *event) {

    CMScanLcioWriter *lcio_writer = run_action_->GetLcioWriter();
    lcio_writer->ClearEvent();
    lcio_writer->CreateEvent(event);
}


void CMTScanEventAction::EndOfEventAction(const G4Event *event) {

    CMScanLcioWriter *lcio_writer = run_action_->GetLcioWriter();
    CMScanRootWriter *root_writer = run_action_->GetRootWriter();

    G4HCofThisEvent *col = event->GetHCofThisEvent();
    std::vector<CMScanDigit> digits;


    for (int i = 0; i < col->GetNumberOfCollections(); i++) {

        auto *hits_collection = dynamic_cast<HitsCollection*>(col->GetHC(i));
        std::vector<CMScanHit*> hits_per_chamber = *(hits_collection->GetVector());

        if (!hits_per_chamber.empty()){

            int chamber_id = hits_per_chamber.at(0)->GetChamberID();
            auto digits_per_chamber = world_geometry_->GetRpc(chamber_id)->Digitize(hits_per_chamber);
            digits.insert(digits.end(), digits_per_chamber.begin(), digits_per_chamber.end());
        }
    }

    if (!digits.empty()) {
        root_writer->AddHits(digits);
        lcio_writer->AddHits(digits);
    }
}


