#include "test.hpp"

namespace tests {

namespace {

void unresolvedEnding() {
    TestGame game;
    contains("nevyřešený", game.send("zpráva"));
    equal(Ending::Unresolved, game.state().ending.value());
}

void accidentEnding() {
    TestGame game;
    game.play({"jdi sever", "jdi dolu", "prozkoumej rozvadec", "jdi nahoru", "jdi jih", "zprava"});
    equal(Ending::AccidentExplained, game.state().ending.value());
    require(!game.state().has(Flag::OwnerRescued));
}

void rescueEnding() {
    TestGame game;
    game.repairPower();
    require(game.state().has(Flag::PowerOn));
    require(!game.state().owns("repair-kit"));
    game.rescue();
    equal("veranda", game.state().locationId);
    equal("veranda", game.state().characterLocations.at("owner"));
    require(game.state().has(Flag::CauseKnown));
    game.returnFromVeranda();
    game.send("zprava");
    equal(Ending::Rescue, game.state().ending.value());
}

void evidenceBeforeRescue() {
    TestGame game;
    game.gatherEvidence();
    require(game.state().has(Flag::RepairFaultProven));
    game.play({"jdi jih", "jdi vychod", "jdi jih"});
    game.repairPower();
    game.rescue();
    game.returnFromVeranda();
    contains("závada opravy doloženy", game.send("zprava"));
    equal(Ending::FullTruth, game.state().ending.value());
}

void evidenceAfterRescue() {
    TestGame game;
    game.repairPower();
    game.rescue();
    game.returnFromVeranda();
    game.gatherEvidence();
    game.play({"jdi jih", "jdi vychod", "jdi jih", "zprava"});
    equal(Ending::FullTruth, game.state().ending.value());
}

void reportAtCarOnly() {
    TestGame game;
    game.play({"jdi sever", "zprava"});
    require(!game.state().ending);
    game.play({"jdi jih", "zprava", "jdi sever"});
    equal("driveway", game.state().locationId);
    contains("Inventář", game.send("inventar"));
}

void dialogueConditions() {
    TestGame game;
    game.play({"jdi sever", "jdi vychod"});
    require(game.send("promluv se spravcem").find("Požádat") == std::string::npos);
    for (const auto* choice : {"0", "-1", "100000000000000000000", "abc", "3", "1abc"})
        contains("Zvolte číslo", game.send(std::string("odpovez ") + choice));
    game.play({"odpovez 2", "jdi zapad", "jdi zapad", "prozkoumej zapisnik", "jdi vychod", "jdi vychod"});
    contains("Požádat", game.send("promluv se spravcem"));
    game.send("odpovez 2");
    require(game.state().owns("key"));
    require(game.send("promluv se spravcem").find("Požádat") == std::string::npos);
}

void dialogueInvalidation() {
    TestGame game;
    game.play({"jdi sever", "jdi vychod", "promluv se spravcem", "jdi zapad"});
    require(!game.state().activeCharacter);
    contains("Nejprve zahajte", game.send("odpovez 1"));
}

void consistentClues() {
    TestGame game;
    game.play({"jdi sever", "jdi vychod", "promluv se spravcem", "odpovez 1"});
    require(game.state().journal.count("staff") != 0);
    game.play({"jdi zapad", "jdi nahoru", "jdi vychod", "prozkoumej seznam"});
    equal(std::size_t{1}, game.state().journal.size());
}

}

void addStoryTests(std::vector<Test>& tests) {
    tests.emplace_back("Konec bez vyřešení", unresolvedEnding);
    tests.emplace_back("Konec s příčinou bez záchrany", accidentEnding);
    tests.emplace_back("Záchranný konec", rescueEnding);
    tests.emplace_back("Úplný konec s důkazy před záchranou", evidenceBeforeRescue);
    tests.emplace_back("Úplný konec s důkazy po záchraně", evidenceAfterRescue);
    tests.emplace_back("Zpráva pouze u auta a uzavření stavu", reportAtCarOnly);
    tests.emplace_back("Podmíněné dialogy a chybné volby", dialogueConditions);
    tests.emplace_back("Zrušení rozhovoru při jiné akci", dialogueInvalidation);
    tests.emplace_back("Shodné indicie z různých zdrojů", consistentClues);
}

}
