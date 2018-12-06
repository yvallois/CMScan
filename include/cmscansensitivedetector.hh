#ifndef CMTScanTrackerSD_h
#define CMTScanTrackerSD_h

#include <vector>

#include "G4VSensitiveDetector.hh"
#include "cmscanhit.hh"
#include "rpc_base.h"

class G4Step;
class G4HCofThisEvent;

class CMScanSensitiveDetector : public G4VSensitiveDetector {

public:
    CMScanSensitiveDetector(const G4String &name, const G4String &hits_collection_name, Rpc_base *rpc);

    ~CMScanSensitiveDetector() override;

    void Initialize(G4HCofThisEvent *event_hit_collection) override;

    G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;

    void EndOfEvent(G4HCofThisEvent *event_hit_collection) override;

private:
    HitsCollection *hits_collection_;
    Rpc_base *rpc_;
    CMScanHit *current_hit_;
};

#endif