#include "test.hpp"

namespace tests {

namespace {

void ownershipRequired() {
    TestGame game;
    game.play({"jdi sever", "jdi zapad", "pouzij key na dvere", "pouzij klic na dvere"});
    require(!game.state().has(Flag::StudyUnlocked));
    game.play({"jdi vychod", "jdi dolu", "pouzij repair-kit na generator"});
    require(!game.state().has(Flag::PowerOn));
}

void recipeInputs() {
    TestGame game;
    game.play({"jdi sever", "jdi vychod", "jdi vychod", "vezmi drat", "spoj drat s baterii"});
    equal(std::set<std::string>{"wire"}, game.state().inventory);
    game.send("spoj drat s drat");
    equal(std::set<std::string>{"wire"}, game.state().inventory);
    game.play({"jdi zapad", "jdi sever", "vezmi baterii", "spoj baterii s drat"});
    equal(std::set<std::string>{"repair-kit"}, game.state().inventory);
}

void shelterRequirements() {
    TestGame game;
    game.play({"jdi sever", "jdi vychod", "jdi vychod", "vezmi pacidlo", "jdi zapad", "jdi zapad", "jdi dolu"});
    contains("bez proudu", game.send("pouzij pacidlo na dvere"));
    require(!game.state().has(Flag::ShelterOpened));
    TestGame powered;
    powered.repairPower();
    powered.state().inventory.erase("crowbar");
    contains("musíte mít", powered.send("pouzij crowbar na dvere"));
    require(!powered.state().has(Flag::ShelterOpened));
}

void repeatedUse() {
    TestGame game;
    game.repairPower();
    contains("už běží", game.send("pouzij opravnou sadu na generator"));
    game.play({"pouzij pacidlo na dvere", "pouzij pacidlo na dvere"});
    require(game.state().has(Flag::ShelterOpened));
    require(game.state().owns("crowbar"));
}

void safeCodes() {
    TestGame game;
    game.gatherEvidence();
    contains("už je otevřený", game.send("pouzij 2410 na trezor"));
    equal(std::size_t{3}, game.state().journal.size());
    game.state().flags.erase(Flag::RepairFaultProven);
    game.state().journal.erase("protocol");
    for (const auto* code : {"0000", "241", "24100", "klíč"})
        game.send(std::string("pouzij ") + code + " na trezor");
    require(!game.state().has(Flag::RepairFaultProven));
}

void dynamicDescriptions() {
    TestGame game;
    game.repairPower();
    contains("Generátor běží", game.send("rozhlédni se"));
    contains("Generátor běží", game.send("prozkoumej generator"));
    contains("zkřížené", game.send("prozkoumej dvere"));
    game.send("pouzij pacidlo na dvere");
    contains("otevřené", game.send("prozkoumej dvere"));
    require(game.send("rozhlédni se").find("zamčeno") == std::string::npos);
}

}

void addPuzzleTests(std::vector<Test>& tests) {
    tests.emplace_back("Žádné použití předmětů bez vlastnictví", ownershipRequired);
    tests.emplace_back("Recept vyžaduje oba předměty", recipeInputs);
    tests.emplace_back("Kryt vyžaduje proud i páčidlo", shelterRequirements);
    tests.emplace_back("Opakované použití předmětů", repeatedUse);
    tests.emplace_back("Správný a chybný kód trezoru", safeCodes);
    tests.emplace_back("Popisy odpovídají aktuálnímu stavu", dynamicDescriptions);
}

}
