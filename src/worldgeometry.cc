//
// Created by vallois on 04/07/18.
//
#include "worldgeometry.h"
#include "filereadstream.h"
#include "document.h"

#include "factory.h"
#include "rpc_sdhcal_g4impl.h"

//TODO changer detector size c'est codé avec le cul
WorldGeometry *WorldGeometry::Instance(const std::string &json_name) {

    if (nullptr == world_geometry_) {
        world_geometry_ = new WorldGeometry(json_name);
    }
    return world_geometry_;
}


WorldGeometry *WorldGeometry::Get() {

    if (world_geometry_ == nullptr)
        std::cerr<< "Warning : World Geometry Singleton is not instantiated" <<std::endl;

    return world_geometry_;
}

void WorldGeometry::Kill() {

    if (nullptr != world_geometry_) {

        delete world_geometry_;
        world_geometry_ = nullptr;
    }

}

WorldGeometry::WorldGeometry(const std::string &json_name) :
        world_size_{},
        detector_size_{},
        rpc_store_{},
        json_file_name_(json_name) {

    GeometryReader();
    PrintGeometry();

    double minimum_z = 1e9;
    double maximum_z = -1e9;
    double extremum_x = -1e9;
    double extremum_y = -1e9;
    for (auto &it : rpc_store_){

        G4ThreeVector rotation = it.second->GetRotation();
        G4ThreeVector translation = it.second->GetTranslation();
        G4ThreeVector chamber_size = it.second->GetChamberSize();

        G4RotationMatrix rotation_matrix(rotation.getX(), rotation.getY(), rotation.getZ());
        G4RotationMatrix inverse_matrix = rotation_matrix.inverse();

        G4ThreeVector corner_point[4];
        corner_point[0] = {-chamber_size[0] * 0.5, -chamber_size[1] * 0.5, 0};
        corner_point[1] = {-chamber_size[0] * 0.5, chamber_size[1] * 0.5, 0};
        corner_point[2] = {chamber_size[0] * 0.5, chamber_size[1] * 0.5, 0};
        corner_point[3] = {chamber_size[0] * 0.5, -chamber_size[1] * 0.5, 0};

        corner_point[0] = corner_point[0].transform(inverse_matrix) + translation;
        corner_point[1] = corner_point[1].transform(inverse_matrix) + translation;
        corner_point[2] = corner_point[2].transform(inverse_matrix) + translation;
        corner_point[3] = corner_point[3].transform(inverse_matrix) + translation;

        for (auto &i : corner_point) {
            if (i[2] > maximum_z)
                maximum_z = i[2];
            if (i[2] < minimum_z)
                minimum_z = i[2];
            if (fabs(i[0]) > extremum_x)
                extremum_x = fabs(i[0]);
            if (fabs(i[1]) > extremum_y)
                extremum_y = fabs(i[1]);
        }
    }
    detector_size_ = {extremum_x*2, extremum_y*2, maximum_z*2+30};
    world_size_ = detector_size_*1.2;
}

void WorldGeometry::GeometryReader() {

    FILE* file = fopen(json_file_name_.c_str(), "r");
    char readBuffer[65536];
    rapidjson::FileReadStream read_stream(file, readBuffer, sizeof(readBuffer));
    rapidjson::Document document;
    document.ParseStream(read_stream);
    fclose(file);

    Factory<Rpc_base, std::string>::Register("SDHCAL", &createInstance<Rpc_SDHCAL_G4impl>);
    Factory<Rpc_base> factory;

    assert(document.HasMember("chambers") && document["chambers"].IsArray());
    const rapidjson::Value& chamber = document["chambers"];
    for (rapidjson::SizeType i = 0; i < chamber.Size(); i++) {

        if(chamber[i].HasMember("chamber_id")){

            int chamber_id = chamber[i]["chamber_id"].GetInt();

            const rapidjson::Value& translation_array = chamber[i]["translation"];
            G4ThreeVector translation(translation_array[0]["pos_x"].GetDouble(),
                                      translation_array[1]["pos_y"].GetDouble(),
                                      translation_array[2]["pos_z"].GetDouble());

            const rapidjson::Value& rotation_array = chamber[i]["rotation"];
            G4ThreeVector rotation(rotation_array[0]["psi"].GetDouble()*CLHEP::pi/180,
                                   rotation_array[1]["theta"].GetDouble()*CLHEP::pi/180,
                                   rotation_array[2]["phi"].GetDouble()*CLHEP::pi/180);
            std::string type = chamber[i]["type"].GetString();
            int stack_id = chamber[i]["stack_id"].GetInt();

            auto it = rpc_store_.insert({chamber_id, factory.Create(type, chamber_id, stack_id)});
            if (it.second){

                it.first->second->SetTranslation(translation);
                it.first->second->SetRotation(rotation);
            }
            else{

                std::cout <<"ERROR : Chamber "<<chamber_id<<" already exists"<<std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else{
            std::cout <<"ERROR : json file has no chambers member"<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void WorldGeometry::PrintGeometry() {

    std::cout<<"-------------------------------------------------"<<std::endl;
    for (auto &item : rpc_store_){
        std::cout << "chamber : " << item.first << std::endl;
        item.second->PrintGeometry();
    }
    std::cout<<"-------------------------------------------------"<<std::endl;

}

WorldGeometry::~WorldGeometry() {

    for (auto &item : rpc_store_)
        delete item.second;
}
