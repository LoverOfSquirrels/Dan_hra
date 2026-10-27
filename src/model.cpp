#include "vila/model.hpp"

namespace vila {

bool Exit::isOpen(const GameState& state) const {
    return !requiredFlag || state.has(*requiredFlag);
}

bool GameState::has(Flag flag) const {
    return flags.count(flag) != 0;
}

bool GameState::owns(const std::string& item) const {
    return inventory.count(item) != 0;
}

bool GameState::record(const Clue& clue) {
    if (clue.flag)
        flags.insert(*clue.flag);
    return journal.emplace(clue.id, clue.text).second;
}

GameState GameState::create(const GameWorld& world) {
    GameState state;
    for (const auto& [id, item] : world.items)
        if (!item.initialLocation.empty())
            state.itemLocations.emplace(id, item.initialLocation);
    for (const auto& [id, character] : world.characters)
        state.characterLocations.emplace(id, character.initialLocation);
    return state;
}

}
