#pragma once

#include "vila/session.hpp"

namespace vila {

class ObjectFinder {
public:
    explicit ObjectFinder(const GameSession& session) : session_(session) {}
    const Item* inventoryItem(const std::string& name) const;
    const Item* roomItem(const std::string& name) const;
    const Feature* feature(const std::string& name) const;
    const Character* character(const std::string& name) const;

private:
    const GameSession& session_;
};

}
