#ifndef CMTScanTrackerHit_h
#define CMTScanTrackerHit_h

#include "G4Step.hh"
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
#include "cmscanrpc.hh"


class CMScanHit : public G4VHit {

public:
    explicit CMScanHit(G4Step *step, CMScanRPC *rpc);

    ~CMScanHit() override = default;

    void *operator new(size_t);

    void operator delete(void *);

    void AddStep (const G4Step *step);

    void Finalize();

    void Print() override;

    // Get methods
    inline G4double GetEdep() const { return deposited_energy_; };

    inline G4ThreeVector GetPos() const { return position_; };

    inline G4double GetTime() const { return time_; };

    inline G4int GetTrackID() const { return track_ID_; };

    inline G4ThreeVector GetEndPositions() const { return end_position_; };

    inline G4bool GetIsLeaving() const { return is_leaving_step_; };

    inline G4int GetPDGID() const { return pdg_ID_; }

    inline G4int GetChamberID() const { return chamber_ID_; };

private:
	G4double deposited_energy_;
	G4ThreeVector position_;
	G4double time_;

    G4ThreeVector end_position_;
    G4ThreeVector begin_position_;
    G4int track_ID_;
    G4int pdg_ID_;
    G4double energy_deposited_;
    G4bool is_leaving_step_;
    CMScanRPC *rpc_;
    G4int chamber_ID_;
};

typedef G4THitsCollection<CMScanHit> HitsCollection;

extern G4ThreadLocal G4Allocator<CMScanHit> *CMTScanTrackerHitAllocator;

#endif