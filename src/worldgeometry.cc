//
// Created by vallois on 04/07/18.
//
#include "worldgeometry.hh"
#include "filereadstream.h"
#include "document.h"
#include "G4RotationMatrix.hh"
//TODO check detector size
WorldGeometry *WorldGeometry::Instance(const std::string &json_name) {

    if (nullptr == world_geometry_) {
        world_geometry_ = new WorldGeometry(json_name);
    }
    return world_geometry_;
}


WorldGeometry *WorldGeometry::Instance() {

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
        chambers_geometry_{},
        json_file_name_(json_name) {

    GeometryReader();
    PrintGeometry();

    double minimum_z = 1e9;
    double maximum_z = -1e9;
    double extremum_x = -1e9;
    double extremum_y = -1e9;
    for (auto &it : chambers_geometry_){
        G4RotationMatrix rotation_matrix(it.second.rotation.getX(), it.second.rotation.getY(), it.second.rotation.getZ());
        G4RotationMatrix inverse_matrix = rotation_matrix.inverse();

        double size_x = it.second.pad_size * it.second.nb_pad_x;
        double size_y = it.second.pad_size * it.second.nb_pad_y;

        G4ThreeVector corner_point[4];
        corner_point[0] = {-size_x*0.5,-size_y*0.5,0};
        corner_point[1] = {-size_x*0.5,size_y*0.5,0};
        corner_point[2] = {size_x*0.5,size_y*0.5,0};
        corner_point[3] = {size_x*0.5,-size_y*0.5,0};

        corner_point[0] = corner_point[0].transform(inverse_matrix)+it.second.translation;
        corner_point[1] = corner_point[1].transform(inverse_matrix)+it.second.translation;
        corner_point[2] = corner_point[2].transform(inverse_matrix)+it.second.translation;
        corner_point[3] = corner_point[3].transform(inverse_matrix)+it.second.translation;

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

    chambers_geometry_.clear();
    FILE* file = fopen(json_file_name_.c_str(), "r");
    char readBuffer[65536];
    rapidjson::FileReadStream read_stream(file, readBuffer, sizeof(readBuffer));
    rapidjson::Document document;
    document.ParseStream(read_stream);
    fclose(file);

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

            int nb_pad_x = chamber[i]["nb_pad_x"].GetInt();
            int nb_pad_y = chamber[i]["nb_pad_y"].GetInt();
            int pad_size = chamber[i]["pad_size"].GetInt();

            auto it = chambers_geometry_.insert({chamber_id, ChamberGeometry{chamber_id, translation, rotation, nb_pad_x, nb_pad_y, pad_size}});

            if(!it.second){
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
    for (auto &it : chambers_geometry_){

        std::cout<<"Chamber : "<<it.second.chamber_id<<std::endl;
        std::cout<<"*** translation"<<it.second.translation<<std::endl;
        std::cout<<"*** rotation"<<it.second.rotation<<std::endl;
        std::cout<<"*** nb pad x : "<<it.second.nb_pad_x<<"\t nb pad y : "<<it.second.nb_pad_y<<std::endl;
        std::cout<<"*** pad size : "<<it.second.pad_size<<std::endl<<std::endl;
    }
    std::cout<<"-------------------------------------------------"<<std::endl;

}

