#include "vila/finder.hpp"
#include "vila/text.hpp"

namespace vila {

const Item* ObjectFinder::inventoryItem(const std::string& name) const {
    for (const auto& [id, item] : session_.world().items)
        if (session_.state().owns(id) && Text::matches(name, item.name, item.aliases))
            return &item;
    return nullptr;
}

const Item* ObjectFinder::roomItem(const std::string& name) const {
    for (const auto& [id, item] : session_.world().items) {
        const auto found = session_.state().itemLocations.find(id);
        if (found != session_.state().itemLocations.end() && found->second == session_.state().locationId
            && Text::matches(name, item.name, item.aliases))
            return &item;
    }
    return nullptr;
}

const Feature* ObjectFinder::feature(const std::string& name) const {
    for (const auto& feature : session_.world().locations.at(session_.state().locationId).features)
        if (Text::matches(name, feature.name, feature.aliases))
            return &feature;
    return nullptr;
}

const Character* ObjectFinder::character(const std::string& name) const {
    for (const auto& [id, character] : session_.world().characters)
        if (session_.state().characterLocations.at(id) == session_.state().locationId
            && Text::matches(name, character.name, character.aliases))
            return &character;
    return nullptr;
}

}
