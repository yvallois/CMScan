#include "cmscanrootwriter.hh"

CMScanRootWriter::CMScanRootWriter(const std::string &output_name) : tfile_(nullptr),
                                                                     ttree_(nullptr),
                                                                     output_name_(output_name),
                                                                     event_number_(0),
                                                                     output_x_(0),
                                                                     output_y_(0),
                                                                     output_z_(0) {

    tfile_ = new TFile(output_name_.c_str(),"RECREATE");
    if(!(tfile_->IsOpen()) || tfile_->IsZombie())
        std::cout<<"Error : opening file"<<std::endl;
    else
        std::cout<<output_name_<<" is open"<<std::endl;

    ttree_ = new TTree("CMScan","CMScan");
    ttree_->SetAutoSave(32*1024);
    ttree_->Branch("event_number_", &event_number_, "event_number_/I");
    ttree_->Branch("output_i_",&output_x_,"output_i_/D");
    ttree_->Branch("output_j_",&output_y_,"output_j_/D");
    ttree_->Branch("output_k_",&output_z_,"output_k_/D");
}

CMScanRootWriter::~CMScanRootWriter() {

    ttree_->Write();
    tfile_->Close();
    delete tfile_;
}

void CMScanRootWriter::AddHits(std::vector<CMScanHit *> &hits) {

    for (auto &hit : hits) {

        output_x_ = hit->GetPos()[0];
        output_y_ = hit->GetPos()[1];
        output_z_ = hit->GetPos()[2];
        ttree_->Fill();
        output_x_ = output_y_ = output_z_ = 0;
    }
    event_number_++;
}
