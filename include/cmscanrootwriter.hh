#ifndef CMSCAN_CMSCANROOTWRITER_HH
#define CMSCAN_CMSCANROOTWRITER_HH

#include "TFile.h"
#include "TTree.h"
#include <string>
#include "cmscanhit.hh"

class CMScanRootWriter {

public:
    explicit CMScanRootWriter(const std::string &output_name);

    virtual ~CMScanRootWriter();

    void AddHits(std::vector<CMScanHit*> &hits);

private:
    TFile *tfile_;
    std::string output_name_;
    TTree *ttree_;
    double output_x_;
    double output_y_;
    double output_z_;
    int event_number_;
};


#endif //CMSCAN_CMSCANROOTWRITER_HH
