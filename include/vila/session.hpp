#pragma once

#include "vila/model.hpp"

namespace vila {

class GameSession {
public:
    explicit GameSession(GameWorld world);
    const GameWorld& world() const { return world_; }
    GameState& state() { return state_; }
    const GameState& state() const { return state_; }
    std::string save();
    std::string load();

private:
    const GameWorld world_;
    GameState state_;
    std::optional<GameState> savedState_;
};

}
