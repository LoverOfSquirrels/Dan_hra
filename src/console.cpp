#include "vila/console.hpp"
#include <istream>
#include <ostream>

namespace vila {

void ConsoleGame::run() {
    output_ << "Vila Rubín\n"
        << "Jste pojišťovací vyšetřovatel. Zjistěte, co se stalo s obyvateli vily, "
           "a u auta uzavřete zprávu pro pojišťovnu.\n"
        << "Seznam příkazů: pomoc.\n" << GameView(session_).look() << '\n';
    std::string line;
    while (true) {
        output_ << "\n> ";
        if (!std::getline(input_, line))
            break;
        const auto command = parser_.parse(line);
        output_ << dispatcher_.dispatch(command) << '\n';
        if (command.kind == CommandKind::Quit)
            break;
    }
}

}
