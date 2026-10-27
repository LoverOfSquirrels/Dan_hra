#include "test.hpp"

namespace tests {
namespace {

void equivalent(const GameState& expected, const GameState& actual) {
    equal(expected.locationId, actual.locationId);
    equal(expected.ending, actual.ending);
    equal(expected.activeCharacter, actual.activeCharacter);
    equal(expected.inventory, actual.inventory);
    equal(expected.flags, actual.flags);
    equal(expected.itemLocations, actual.itemLocations);
    equal(expected.characterLocations, actual.characterLocations);
    equal(expected.journal, actual.journal);
}

}

namespace {

void loadWithoutSave() {
    TestGame game;
    game.play({"jdi sever", "jdi zapad", "vezmi zapisnik"});
    contains("není uložená", game.send("nacti"));
    equal("library", game.state().locationId);
    require(game.state().owns("notebook"));
}

void independentSnapshot() {
    TestGame game;
    game.play({"jdi sever", "jdi zapad", "vezmi zapisnik", "prozkoumej zapisnik",
        "jdi vychod", "jdi vychod", "promluv se spravcem"});
    const auto saved = game.state();
    game.send("uloz");
    for (int attempt = 0; attempt < 2; ++attempt) {
        game.state().locationId = "bedroom";
        game.state().inventory.clear();
        game.state().itemLocations.clear();
        game.state().characterLocations.at("owner") = "veranda";
        game.state().journal.clear();
        game.state().flags.insert(Flag::PowerOn);
        game.state().ending = Ending::FullTruth;
        game.state().activeCharacter.reset();
        game.send("nacti");
        equivalent(saved, game.state());
    }
    contains("mosazný klíč", game.send("odpovez 2"));
}

void rescueAndEndingSnapshot() {
    TestGame game;
    game.repairPower();
    game.rescue();
    const auto rescued = game.state();
    game.send("uloz");
    game.returnFromVeranda();
    game.send("zprava");
    equal(Ending::Rescue, game.state().ending.value());
    game.send("nacti");
    equivalent(rescued, game.state());
    game.returnFromVeranda();
    game.play({"zprava", "uloz", "nacti"});
    equal(Ending::Rescue, game.state().ending.value());
    equal("veranda", game.state().characterLocations.at("owner"));
}

void latestSaveWins() {
    TestGame game;
    game.play({"uloz", "jdi sever", "uloz", "jdi zapad", "nacti"});
    equal("foyer", game.state().locationId);
}

void newSessionHasNoSave() {
    TestGame first;
    first.play({"jdi sever", "uloz"});
    TestGame second;
    contains("není uložená", second.send("nacti"));
    equal("driveway", second.state().locationId);
}

}

void addSaveTests(std::vector<Test>& tests) {
    tests.emplace_back("Načtení bez uložení", loadWithoutSave);
    tests.emplace_back("Úplný snapshot a opakované načtení", independentSnapshot);
    tests.emplace_back("Snapshot záchrany a ukončené hry", rescueAndEndingSnapshot);
    tests.emplace_back("Poslední uložení nahradí předchozí", latestSaveWins);
    tests.emplace_back("Nový běh nepřevezme uloženou hru", newSessionHasNoSave);
}

}
