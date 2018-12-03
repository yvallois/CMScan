#include "G4AffineTransform.hh"
#include "cmscanhit.hh"
#include "G4VVisManager.hh"
#include <G4TouchableHistory.hh>


G4ThreadLocal G4Allocator<CMScanHit> *CMTScanTrackerHitAllocator = nullptr;


CMScanHit::CMScanHit(G4Step *step, CMScanRPC *rpc)
        : G4VHit(),
          position_(G4ThreeVector()),
          rpc_(rpc),
          chamber_ID_(-1){

    track_ID_ = step->GetTrack()->GetTrackID();
    begin_position_ = step->GetPreStepPoint()->GetPosition();
    end_position_ = step->GetPostStepPoint()->GetPosition();
    pdg_ID_ = step->GetTrack()->GetDynamicParticle()->GetPDGcode();
    energy_deposited_ = step->GetTotalEnergyDeposit();
    time_ = step->GetPreStepPoint()->GetGlobalTime();
    is_leaving_step_ = (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary);
}


void *CMScanHit::operator new(size_t) {
    if (!CMTScanTrackerHitAllocator)
        CMTScanTrackerHitAllocator = new G4Allocator<CMScanHit>;
    return (void *) CMTScanTrackerHitAllocator->MallocSingle();
}


void CMScanHit::operator delete(void *hit) {
    CMTScanTrackerHitAllocator->FreeSingle((CMScanHit *) hit);
}

void CMScanHit::Print() {

    std::cout<<"*******************************************************************************************"<<std::endl;
    std::cout << "posX : " << position_[0] << "\t\tposY : " << position_[1] << "\t\tposZ : " << position_[2] << std::endl;
    std::cout << "time : " << time_ << std::endl;
}

void CMScanHit::AddStep(const G4Step *step) {

    end_position_ = step->GetPostStepPoint()->GetPosition();
    energy_deposited_ += step->GetTotalEnergyDeposit();
    is_leaving_step_ = (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary);
}

void CMScanHit::Finalize() {

    position_ = 0.5 * (end_position_ + begin_position_);
    rpc_->GlobalToRPCCoordinate(position_);
    chamber_ID_ = rpc_->GetChamberId();
}
