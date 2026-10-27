#include "test.hpp"
#include <queue>

namespace tests {

namespace {

void graphIntegrity() {
    const auto world = WorldFactory::create();
    std::set<std::string> reached{"driveway"};
    std::queue<std::string> pending;
    pending.push("driveway");
    while (!pending.empty()) {
        const auto id = pending.front();
        pending.pop();
        for (const auto& exit : world.locations.at(id).exits) {
            require(world.locations.count(exit.destination) != 0, "Východ vede do neexistující lokace.");
            const auto& back = world.locations.at(exit.destination).exits;
            require(std::any_of(back.begin(), back.end(), [&id](const auto& e) { return e.destination == id; }));
            if (reached.insert(exit.destination).second)
                pending.push(exit.destination);
        }
    }
    equal(world.locations.size(), reached.size());
}

void objectReferences() {
    const auto world = WorldFactory::create();
    for (const auto& pair : world.items)
        require(pair.second.initialLocation.empty() || world.locations.count(pair.second.initialLocation) != 0);
    for (const auto& pair : world.characters)
        require(world.locations.count(pair.second.initialLocation) != 0);
    for (const auto& recipe : world.recipes)
        for (const auto& id : {recipe.first, recipe.second, recipe.result})
            require(world.items.count(id) != 0);
    for (const auto& pair : world.locations) {
        std::set<std::string> known;
        for (const auto& exit : pair.second.exits) {
            std::set<std::string> aliases{Text::normalize(exit.name)};
            for (const auto& alias : exit.aliases)
                aliases.insert(Text::normalize(alias));
            for (const auto& alias : aliases)
                require(known.insert(alias).second, "Dva východy mají stejný alias.");
        }
    }
}

void itemsAndJournal() {
    TestGame game;
    game.play({"jdi sever", "jdi zapad"});
    contains("zápisník", game.send("rozhlédni se"));
    game.send("prozkoumej zápisník");
    equal(std::size_t{1}, game.state().journal.size());
    require(game.state().inventory.empty());
    game.send("prozkoumej zápisník");
    equal(std::size_t{1}, game.state().journal.size());
    game.send("vezmi zápisník");
    require(game.state().owns("notebook"));
    require(game.state().itemLocations.count("notebook") == 0);
    require(game.send("rozhlédni se").find("Předměty:") == std::string::npos);
}

void blockedMovement() {
    TestGame game;
    game.play({"jdi sever", "jdi zapad"});
    contains("zamčená", game.send("jdi sever"));
    equal("library", game.state().locationId);
    game.send("jdi teleport");
    equal("library", game.state().locationId);
    game.play({"jdi vychod", "jdi dolu", "jdi sever"});
    equal("cellar", game.state().locationId);
}

void noRemoteInteractions() {
    TestGame game;
    game.play({"prozkoumej zapisnik", "pouzij 2410 na trezor", "vezmi drat", "promluv se majitelkou"});
    require(game.state().flags.empty());
    require(game.state().inventory.empty());
    require(game.state().journal.empty());
    require(!game.state().activeCharacter);
}

}

void addWorldTests(std::vector<Test>& tests) {
    tests.emplace_back("Dosažitelnost a návratnost všech lokací", graphIntegrity);
    tests.emplace_back("Reference a jednoznačnost východů", objectReferences);
    tests.emplace_back("Předměty a nezávislý deník", itemsAndJournal);
    tests.emplace_back("Zamčené a neexistující cesty", blockedMovement);
    tests.emplace_back("Zákaz interakcí na dálku", noRemoteInteractions);
}

}
