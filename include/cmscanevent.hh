#ifndef CMTScanEventAction_h
#define CMTScanEventAction_h 1


#include "cmscanrunaction.hh"
#include "G4UserEventAction.hh"
#include "cmscanhit.hh"
#include "globals.hh"


class CMTScanEventAction : public G4UserEventAction {

public:
    explicit CMTScanEventAction(CMTScanRunAction *run_action);

    ~CMTScanEventAction() override = default;

    void BeginOfEventAction(const G4Event *) override;

    void EndOfEventAction(const G4Event *) override;

private:
    CMTScanRunAction *run_action_;
};

#endif
