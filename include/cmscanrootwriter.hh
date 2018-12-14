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

    void AddHits(std::vector<CMScanDigit> &digits);

private:
    TFile *tfile_;
    std::string output_name_;
    TTree *ttree_;
    int output_x_;
    int output_y_;
    int output_z_;
    int time_;
    int event_number_;
};


#endif //CMSCAN_CMSCANROOTWRITER_HH
