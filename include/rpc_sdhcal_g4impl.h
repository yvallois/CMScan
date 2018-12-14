//
// Created by vallois on 05/12/18.
//

#ifndef CMSCAN_RPCSDHCAL_G4IMPL_H
#define CMSCAN_RPCSDHCAL_G4IMPL_H

#include <iostream>
#include <random>

#include <G4AffineTransform.hh>

#include "rpc_sdhcal.h"
#include "cmscanhit.hh"

class Rpc_SDHCAL_G4impl : public Rpc_SDHCAL {
public:

    Rpc_SDHCAL_G4impl(int chamber_id, int stack_id);

    virtual ~Rpc_SDHCAL_G4impl() {delete rpc_logic_;}

    void PrintGeometry() override;

    void Build() override;

    std::vector<CMScanDigit> Digitize(std::vector<CMScanHit*> &hits) override;

    void AddChamber(G4LogicalVolume *mother_logic) override;

    void GlobalToRPCCoordinate(G4ThreeVector &coordinate) override;

private:

    G4LogicalVolume *rpc_logic_;
    G4AffineTransform affine_transform_;
    std::string name_;

    int noise_rate_; ///< noise/pad/sec

    std::default_random_engine generator_;
    std::uniform_real_distribution<> real_distribution;
    std::binomial_distribution<int> noise_distribution_;
    std::uniform_int_distribution<> gen_noise_distribution_;
};


#endif //CMSCAN_RPCSDHCAL_G4IMPL_H
