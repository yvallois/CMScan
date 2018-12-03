//
// Created by vallois on 05/07/18.
//

#ifndef CMTSCAN_CMSCANRPC_H
#define CMTSCAN_CMSCANRPC_H

#include "G4AffineTransform.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class CMScanRPC {
public:
    CMScanRPC(int i, int pad_number_x, int pad_number_y, int pad_size);

    virtual ~CMScanRPC();

    void AddChamber(G4RotationMatrix *rot, G4ThreeVector &trans, G4LogicalVolume *mother_logic);

    inline int GetChamberId() const { return chamber_id_; }

    inline int GetPadSize() const { return pad_size_; }

    inline int GetPadNumberX() const { return pad_number_x_; }

    inline int GetPadNumberY() const { return pad_number_y_; }

    inline G4LogicalVolume *GetLogicalVolume() const { return rpc_logic_; }

    void GlobalToRPCCoordinate(G4ThreeVector &coordinate);

private:
    void Build();

    G4String name_;
    int chamber_id_;
    int pad_number_x_;
    int pad_number_y_;
    int pad_size_;
    G4ThreeVector chamber_size_;
    G4LogicalVolume *rpc_logic_;
    G4AffineTransform affine_transform_;
};


#endif //CMTSCAN_CMSCANRPC_H
