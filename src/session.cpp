#include "vila/session.hpp"
#include <utility>

namespace vila {

GameSession::GameSession(GameWorld world) : world_(std::move(world)), state_(GameState::create(world_)) {}

std::string GameSession::save() {
    savedState_ = state_;
    return "Hra uložena do paměti. Uložení platí do ukončení aplikace.";
}

std::string GameSession::load() {
    if (!savedState_)
        return "V tomto běhu aplikace zatím není uložená hra.";
    state_ = *savedState_;
    return "Hra načtena.";
}

}
