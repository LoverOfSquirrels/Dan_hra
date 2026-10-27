#include "test.hpp"
#include <iostream>

int main() {
    std::vector<tests::Test> cases;
    tests::addParserTests(cases);
    tests::addWorldTests(cases);
    tests::addStoryTests(cases);
    tests::addPuzzleTests(cases);
    tests::addSaveTests(cases);
    tests::addConsoleTests(cases);
    std::size_t failures = 0;
    for (const auto& test : cases) {
        try {
            test.second();
            std::cout << "OK  " << test.first << '\n';
        } catch (const std::exception& error) {
            ++failures;
            std::cerr << "CHYBA  " << test.first << ": " << error.what() << '\n';
        }
    }
    std::cout << "Výsledek: " << cases.size() - failures << '/' << cases.size() << " testů prošlo.\n";
    return failures == 0 ? 0 : 1;
}
