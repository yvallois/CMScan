//
// Created by vallois on 05/12/18.
//

#ifndef CMSCAN_RPC_SDHCAL_H
#define CMSCAN_RPC_SDHCAL_H

#include "G4ThreeVector.hh"

#include "rpc_base.h"

class Rpc_SDHCAL : public Rpc_base {

public:
    Rpc_SDHCAL(int chamber_id, int stack_id) :
            Rpc_base(chamber_id, stack_id),
            num_pad_x_(96),
            num_pad_y_(96),
            pad_size_(10.408),
            chamber_size_() {

        chamber_size_[0] = num_pad_x_ * pad_size_;
        chamber_size_[1] = num_pad_y_ * pad_size_;
        chamber_size_[2] = 16.13;
    }

    G4ThreeVector GetChamberSize() override { return chamber_size_;}

    void PrintGeometry() override = 0;

protected:
    int num_pad_x_;
    int num_pad_y_;
    double pad_size_;
    G4ThreeVector chamber_size_;
};

#endif //CMSCAN_RPC_SDHCAL_H
