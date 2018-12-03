#include "cmscanstackingaction.hh"

#include "G4Track.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CMTScanStackingAction::CMTScanStackingAction()
= default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CMTScanStackingAction::~CMTScanStackingAction()
= default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack CMTScanStackingAction::ClassifyNewTrack(const G4Track* track)
{
	//keep primary particle
	if (track->GetParentID() == 0) return fUrgent;
	else return fKill;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
