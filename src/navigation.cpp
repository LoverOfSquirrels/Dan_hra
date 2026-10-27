#include "vila/services.hpp"
#include "vila/text.hpp"
#include <algorithm>

namespace vila {

std::string NavigationService::move(const std::string& direction) {
    const auto& exits = session_.world().locations.at(session_.state().locationId).exits;
    const auto exit = std::find_if(exits.begin(), exits.end(), [&direction](const auto& candidate) {
        return Text::matches(direction, candidate.name, candidate.aliases);
    });
    if (exit == exits.end())
        return "Tudy cesta nevede. Dostupné východy zjistíte příkazem rozhlédni se.";
    if (!exit->isOpen(session_.state()))
        return exit->blockedMessage.empty() ? "Tato cesta je uzavřená." : exit->blockedMessage;
    session_.state().locationId = exit->destination;
    return view_.look();
}

}
