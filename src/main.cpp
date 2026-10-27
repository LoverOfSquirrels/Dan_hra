#include "vila/console.hpp"
#include "vila/world.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
    vila::GameSession session(vila::WorldFactory::create());
    vila::ConsoleGame game(session, std::cin, std::cout);
    game.run();
}
