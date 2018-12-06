//
// Created by vallois on 05/12/18.
//

#ifndef CMSCAN_FACTORY_H
#define CMSCAN_FACTORY_H

#include <map>

#include "rpc_base.h"

template<typename T>
Rpc_base *createInstance(int chamber_id, int stack_id) { return new T(chamber_id, stack_id); }

template<class Object, class Key = std::string>
class Factory {

    static std::map<std::string, Rpc_base*(*)(int chamber_id, int stack_id)> register_;

public:

    static void Register(Key key, Rpc_base*(*obj)(int chamber_id, int stack_id)){

        if (register_.find(key) == register_.end())
            register_[key] = obj;
    };

    Object *Create(const Key &key, int chamber_id, int stack_id){

        Object *tmp = nullptr;
        auto it = register_.find(key);
        if (it != register_.end())
            tmp = ((*it).second)(chamber_id, stack_id);
        return tmp;
    };
};

template<class Object, class Key>
std::map<std::string, Rpc_base*(*)(int chamber_id, int stack_id)> Factory<Object, Key>::register_{};

#endif //CMSCAN_FACTORY_H
