#ifndef CMSCAN_CMSCANLCIOWRITER_HH
#define CMSCAN_CMSCANLCIOWRITER_HH


#include "G4Event.hh"
#include <IO/LCWriter.h>
#include <IMPL/LCEventImpl.h>
#include "cmscanhit.hh"
#include "worldgeometry.hh"


class CMScanLcioWriter {

public:
    explicit CMScanLcioWriter(const std::string &output_name);

    virtual ~CMScanLcioWriter();

    void CreateEvent(const G4Event* event);

    void ClearEvent();

    void AddHits(std::vector<CMScanHit*> &hits);

private:
    IO::LCWriter* lcwriter_;

    IMPL::LCEventImpl* lcevent_;
    std::string output_name_;
    int event_ID_;
};


#endif //CMSCAN_CMSCANLCIOWRITER_HH
