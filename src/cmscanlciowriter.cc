#include <IOIMPL/LCFactory.h>
#include <EVENT/LCIO.h>
#include <IMPL/LCFlagImpl.h>

#include <IMPL/LCEventImpl.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/CalorimeterHitImpl.h>
#include "cmscanlciowriter.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "cmscanprimarygeneratoraction.hh"

CMScanLcioWriter::CMScanLcioWriter(const std::string &output_name) : lcwriter_(nullptr),
                                                                     lcevent_(nullptr),
                                                                     output_name_(output_name),
                                                                     event_ID_(0) {

    lcwriter_ = IOIMPL::LCFactory::getInstance()->createLCWriter();
    lcwriter_->setCompressionLevel(0);
    lcwriter_->open(output_name_, EVENT::LCIO::WRITE_NEW);
}

CMScanLcioWriter::~CMScanLcioWriter() {

    lcwriter_->close();
}


void CMScanLcioWriter::CreateEvent(const G4Event *event) {

    lcevent_ = new IMPL::LCEventImpl();
    lcevent_->setRunNumber(G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID());
    lcevent_->setEventNumber(event->GetEventID());
    lcevent_->setTimeStamp(static_cast<EVENT::long64>(CMTScanPrimaryGeneratorAction::GetDuration() / 200e-9));
}


void CMScanLcioWriter::ClearEvent() {

    delete lcevent_;
}

void CMScanLcioWriter::AddHits(std::vector<CMScanDigit> &digits) {

    IMPL::LCCollectionVec *collection = new IMPL::LCCollectionVec(EVENT::LCIO::CALORIMETERHIT);
    IMPL::LCFlagImpl flag;
    flag.setBit(EVENT::LCIO::RCHBIT_LONG);
    flag.setBit(EVENT::LCIO::RCHBIT_ID1);
    flag.setBit(EVENT::LCIO::RCHBIT_TIME);
    collection->setFlag(flag.getFlag());

    for (auto &hit : digits) {

        auto* calorimeter_hit = new IMPL::CalorimeterHitImpl();
        float position[3];
        position[0] = hit.position_[0];
        position[1] = hit.position_[1];
        position[2] = hit.position_[2];
        calorimeter_hit->setPosition(position);
        calorimeter_hit->setTime(static_cast<float>(hit.time_));
        collection->addElement(calorimeter_hit);
    }
    lcevent_->addCollection(collection, "CMScan");
    lcevent_->setEventNumber(event_ID_);
    lcevent_->setDetectorName("CMScan");
    lcwriter_->writeEvent(lcevent_);
    event_ID_++;
}



