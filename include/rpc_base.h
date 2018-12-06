//
// Created by vallois on 04/12/18.
//

#ifndef CMSCAN_RPC_BASE_H
#define CMSCAN_RPC_BASE_H

#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"

class Rpc_base{
public:
    Rpc_base(int chamber_id, int stack_id) : translation_(), rotation_(), chamber_id_(chamber_id), stack_id_(stack_id) {}

    virtual void Build() = 0;

    virtual void AddChamber(G4LogicalVolume *mother_logic) = 0;

    virtual void GlobalToRPCCoordinate(G4ThreeVector &coordinate) = 0;

    virtual G4ThreeVector GetChamberSize() = 0;

    virtual void PrintGeometry() = 0;

    virtual G4ThreeVector GetTranslation(){ return translation_; }

    virtual G4ThreeVector GetRotation(){ return rotation_; }

    virtual int GetChamberId() { return chamber_id_; }

    virtual void SetTranslation(G4ThreeVector &translation) { translation_ = translation; }

    virtual void SetRotation(G4ThreeVector &rotation) { rotation_ = rotation; }

protected:
    G4ThreeVector translation_;
    G4ThreeVector rotation_;
    int chamber_id_;
    int stack_id_;
};


#endif //CMSCAN_RPC_BASE_H
