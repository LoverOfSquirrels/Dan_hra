#include "test.hpp"

namespace tests {

namespace {

void quitAndEof() {
    for (const auto* input : {"", "\nxyz\ninventar\nkonec\njdi sever\n"}) {
        GameSession session(WorldFactory::create());
        std::istringstream reader(input);
        std::ostringstream writer;
        ConsoleGame game(session, reader, writer);
        game.run();
        contains("Vila Rubín", writer.str());
        equal("driveway", session.state().locationId);
    }
}

void actionableHints() {
    TestGame game;
    contains("drát", game.send("napoveda"));
    game.play({"jdi sever", "jdi vychod", "jdi vychod", "vezmi drat"});
    contains("baterie", game.send("napoveda"));
    game.play({"jdi zapad", "jdi sever", "vezmi baterii"});
    contains("spoj drát s baterií", game.send("napoveda"));
    game.send("spoj drát s baterií");
    contains("opravnou sadu", game.send("napoveda"));
    require(game.state().owns("repair-kit"));
}

void registeredCommands() {
    TestGame game;
    for (const auto* command : {"rozhlédni se", "jdi sever", "vezmi nic", "prozkoumej nic",
             "pouzij nic na nic", "spoj nic s nic", "promluv se nikym", "odpovez 1",
             "inventar", "denik", "pomoc", "napoveda", "uloz", "nacti", "zprava", "konec", "?"})
        require(!game.send(command).empty());
}

}

void addConsoleTests(std::vector<Test>& tests) {
    tests.emplace_back("Konec a konec vstupního proudu", quitAndEof);
    tests.emplace_back("Nápovědu lze skutečně vykonat", actionableHints);
    tests.emplace_back("Všechny příkazy mají strategii", registeredCommands);
}

}
