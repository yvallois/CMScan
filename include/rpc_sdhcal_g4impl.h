//
// Created by vallois on 05/12/18.
//

#ifndef CMSCAN_RPCSDHCAL_G4IMPL_H
#define CMSCAN_RPCSDHCAL_G4IMPL_H

#include <iostream>

#include <G4AffineTransform.hh>

#include "rpc_sdhcal.h"

class Rpc_SDHCAL_G4impl : public Rpc_SDHCAL {
public:

    Rpc_SDHCAL_G4impl(int chamber_id, int stack_id);

    virtual ~Rpc_SDHCAL_G4impl() {delete rpc_logic_;}

    void PrintGeometry() override;

    void Build() override;

    void AddChamber(G4LogicalVolume *mother_logic) override;

    void GlobalToRPCCoordinate(G4ThreeVector &coordinate) override;

private:

    G4LogicalVolume *rpc_logic_;
    G4AffineTransform affine_transform_;
    std::string name_;
};


#endif //CMSCAN_RPCSDHCAL_G4IMPL_H
