#ifndef Constants_h
#define Constants_h

#include <map>

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

#include "rpc_base.h"

class WorldGeometry
{
public:

    static WorldGeometry *Instance(const std::string &json_name);

	static WorldGeometry *Get();

	static void Kill();

    inline G4ThreeVector const getWorldSize() { return world_size_; }

    inline G4ThreeVector const getDetectorSize() { return detector_size_; }

    inline std::map<int, Rpc_base *> const GetRpcs() { return rpc_store_; }

    inline unsigned long GetNumRpc() { return rpc_store_.size(); }

    inline Rpc_base *const GetRpc(int chamber_number) { return rpc_store_[chamber_number]; }

private:
	explicit WorldGeometry(const std::string &json_name);
	~WorldGeometry();
	void GeometryReader();
	void PrintGeometry();

private:
    G4ThreeVector world_size_;
    G4ThreeVector detector_size_;
    std::map<int, Rpc_base*> rpc_store_;
    static WorldGeometry *world_geometry_;
    std::string json_file_name_;
};
#endif
