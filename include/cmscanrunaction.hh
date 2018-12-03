#ifndef CMTScanRunAction_h
#define CMTScanRunAction_h

#include "cmscanlciowriter.hh"
#include "cmscanrootwriter.hh"
#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class CMTScanRunAction : public G4UserRunAction {
public:
    explicit CMTScanRunAction(std::string output_name);

	~CMTScanRunAction() override;

	void BeginOfRunAction(const G4Run *run) override;

	void EndOfRunAction(const G4Run *run) override;

    CMScanLcioWriter *GetLcioWriter() const;

    CMScanRootWriter *GetRootWriter() const;

private:
    CMScanLcioWriter *lcio_writer_;
    CMScanRootWriter *root_writer_;
};

#endif
