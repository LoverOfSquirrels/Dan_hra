#pragma once

#include "vila/model.hpp"

namespace vila {

class WorldFactory {
public:
    static GameWorld create();
};

class LocationCatalog {
public:
    static std::vector<Location> create();
};

}
