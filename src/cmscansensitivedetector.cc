#include "cmscansensitivedetector.hh"
#include "cmscanprimarygeneratoraction.hh"

#include "G4SDManager.hh"
#include "g4root.hh"
#include "G4RunManager.hh"


CMScanSensitiveDetector::CMScanSensitiveDetector(const G4String &name,
                                                 const G4String &hits_collection_name,
                                                 int chamber_id) :
        G4VSensitiveDetector(name),
        hits_collection_(nullptr),
        current_hit_(nullptr),
        detector_id_(chamber_id) {

    collectionName.insert(hits_collection_name);
}


CMScanSensitiveDetector::~CMScanSensitiveDetector() = default;


void CMScanSensitiveDetector::Initialize(G4HCofThisEvent *event_hit_collection) {

    /// Create hits collection
    hits_collection_ = new HitsCollection(SensitiveDetectorName, collectionName[0]);

    /// Add this collection in event_hit_collection
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    event_hit_collection->AddHitsCollection(hcID, hits_collection_);
}


G4bool CMScanSensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *history) {

    if (!static_cast<bool>(step->GetTrack()->GetDynamicParticle()->GetCharge()) && step->GetTotalEnergyDeposit() == 0 )
        return false;

    if (current_hit_){

        if ( current_hit_->GetTrackID() == step->GetTrack()->GetTrackID() &&
             current_hit_->GetEndPositions() == step->GetPreStepPoint()->GetPosition() ) {

            current_hit_->AddStep(step);
        }
        else {

            current_hit_->Finalize();
            hits_collection_->insert(current_hit_);
            current_hit_ = new CMScanHit(step, detector_id_);
        }
    }
    else{

        current_hit_ = new CMScanHit(step, detector_id_);
    }

    if ( current_hit_->GetIsLeaving() ) {

        current_hit_->Finalize();
        hits_collection_->insert(current_hit_);
        current_hit_ = nullptr;
    }

    return true ;

}


void CMScanSensitiveDetector::EndOfEvent(G4HCofThisEvent *) {

    if (verboseLevel > 1) {

        G4int nofHits = hits_collection_->entries();
        G4cout << G4endl << "-------->Hits Collection: in this event they are " << nofHits
               << " hits in the tracker chambers: " << G4endl;
    }
}