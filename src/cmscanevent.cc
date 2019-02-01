#include "cmscanevent.hh"
#include "cmscansensitivedetector.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

#include "rpc_sdhcal_g4impl.h"

CMTScanEventAction::CMTScanEventAction(CMTScanRunAction *run_action) :
        G4UserEventAction(),
        run_action_(run_action),
        world_geometry_(WorldGeometry::Instance()) {}


void CMTScanEventAction::BeginOfEventAction(const G4Event *event) {

    lcio_writer_ = run_action_->GetLcioWriter();
    root_writer_ = run_action_->GetRootWriter();

    lcio_writer_->ClearEvent();
    lcio_writer_->CreateEvent(event);
}


void CMTScanEventAction::EndOfEventAction(const G4Event *event) {

    G4HCofThisEvent *col = event->GetHCofThisEvent();
    std::vector<CMScanDigit> digits;


    for (int i = 0; i < col->GetNumberOfCollections(); i++) {

        auto *hits_collection = dynamic_cast<HitsCollection*>(col->GetHC(i));
        std::vector<CMScanHit*> hits_per_chamber = *(hits_collection->GetVector());

        if (!hits_per_chamber.empty()){

            int chamber_id = hits_per_chamber.at(0)->GetChamberID();
            Rpc_base *rpc_base_ptr = world_geometry_->GetRpc(chamber_id);
            if (rpc_base_ptr->GetRpcType() == "SDHCALG4IMPL"){

                auto rpc_ptr = dynamic_cast<Rpc_SDHCAL_G4impl*>(rpc_base_ptr);
                auto digits_per_chamber = rpc_ptr->Digitize(hits_per_chamber);
                digits.insert(digits.end(), digits_per_chamber.begin(), digits_per_chamber.end());
            }
        }
    }

    if (!digits.empty()) {
        root_writer_->AddHits(digits);
        lcio_writer_->AddHits(digits);
    }
}


