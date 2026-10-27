#include "vila/world.hpp"

namespace vila {
namespace {

std::vector<Item> createItems() {
    return {
        {"notebook", "zápisník", "Poslední zápis majitelky: Správce má klíč od pracovny. "
            "Kód trezoru tvoří den a měsíc na rodinném portrétu.", "library", {"zapisnik", "sesit"},
            Clue{"notebook", "Správce má klíč od pracovny. Kód trezoru je den a měsíc z portrétu.", Flag::NotebookRead}},
        {"key", "mosazný klíč", "Na štítku je napsáno Pracovna.", "", {"klic", "klic od pracovny"}},
        {"wire", "drát", "Izolovaný drát pro připojení startovací baterie.", "workshop", {"vodic"}},
        {"battery", "baterie", "Nabitá startovací baterie. Chybí k ní přívodní drát.", "pantry", {"akumulator", "baterii"}},
        {"repair-kit", "opravná sada", "Baterie s připojeným drátem pro nastartování generátoru.", "",
            {"sada", "opravnou sadu", "baterie s dratem", "baterii s dratem"}},
        {"crowbar", "páčidlo", "Ocelové páčidlo. Uvolní zkřížené dveře.", "workshop", {"pacidlo"}}
    };
}

std::vector<Character> createCharacters() {
    return {
        {"caretaker", "správce", "kitchen", {"spravcem", "karel", "karlem"}},
        {"owner", "majitelka", "shelter", {"majitelkou", "eliska", "eliskou", "pani rubinova", "rubinovou"}}
    };
}

}

GameWorld WorldFactory::create() {
    GameWorld world;
    for (const auto& location : LocationCatalog::create())
        world.locations.emplace(location.id, location);
    for (const auto& item : createItems())
        world.items.emplace(item.id, item);
    for (const auto& character : createCharacters())
        world.characters.emplace(character.id, character);
    world.recipes.push_back({"wire", "battery", "repair-kit"});
    return world;
}

}
