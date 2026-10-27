#include "vila/services.hpp"
#include "vila/text.hpp"

namespace vila {
namespace {

void addList(std::vector<std::string>& lines, const std::string& label, const std::vector<std::string>& values) {
    if (!values.empty())
        lines.push_back(label + ": " + Text::join(values, ", "));
}

void addStateDescription(std::vector<std::string>& lines, const std::string& room, const GameState& state) {
    if (room == "library" && state.has(Flag::StudyUnlocked))
        lines.emplace_back("Dveře do pracovny jsou odemčené.");
    if (room == "cellar") {
        lines.emplace_back(state.has(Flag::PowerOn) ? "Generátor běží. Kontrolka krytu svítí."
            : "Generátor je vypnutý. Elektrický zámek krytu nemá napájení.");
        if (state.has(Flag::ShelterOpened))
            lines.emplace_back("Dveře krytu jsou otevřené.");
    }
    if (room == "shelter" && state.has(Flag::OwnerRescued))
        lines.emplace_back("Majitelka je v bezpečí na verandě.");
}

}

std::string GameView::look() const {
    const auto& state = session_.state();
    const auto& world = session_.world();
    const auto& room = world.locations.at(state.locationId);
    std::vector<std::string> lines{room.name, room.description};
    std::vector<std::string> items, features, characters, exits;
    for (const auto& [id, location] : state.itemLocations)
        if (location == room.id)
            items.push_back(world.items.at(id).name);
    for (const auto& feature : room.features)
        features.push_back(feature.name);
    for (const auto& [id, location] : state.characterLocations)
        if (location == room.id)
            characters.push_back(world.characters.at(id).name);
    for (const auto& exit : room.exits)
        exits.push_back(exit.name + " (" + world.locations.at(exit.destination).name
            + (exit.isOpen(state) ? "" : ", zamčeno") + ")");
    addList(lines, "Předměty", items);
    addList(lines, "K prozkoumání", features);
    addList(lines, "Postavy", characters);
    addStateDescription(lines, room.id, state);
    addList(lines, "Východy", exits);
    return Text::join(lines, "\n");
}

std::string GameView::inventory() const {
    std::vector<std::string> names;
    for (const auto& id : session_.state().inventory)
        names.push_back(session_.world().items.at(id).name);
    return names.empty() ? "Inventář je prázdný." : "Inventář: " + Text::join(names, ", ");
}

std::string GameView::journal() const {
    if (session_.state().journal.empty())
        return "Deník je prázdný.";
    std::vector<std::string> lines{"Deník"};
    for (const auto& entry : session_.state().journal)
        lines.push_back("- " + entry.second);
    return Text::join(lines, "\n");
}

}
