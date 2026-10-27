#include "vila/services.hpp"
#include "vila/text.hpp"
#include <charconv>

namespace vila {

std::string DialogueService::talk(const std::string& name) {
    const auto* character = finder_.character(name);
    if (!character)
        return "Tato postava tu není.";
    session_.state().activeCharacter = character->id;
    return character->name + "\n" + showOptions(options(character->id));
}

std::string DialogueService::choose(const std::string& input) {
    const auto character = session_.state().activeCharacter;
    if (!character || session_.state().characterLocations.at(*character) != session_.state().locationId)
        return "Nejprve zahajte rozhovor příkazem promluv se <postavou>.";
    const auto choices = options(*character);
    unsigned int choice = 0;
    const auto parsed = std::from_chars(input.data(), input.data() + input.size(), choice);
    if (parsed.ec != std::errc{} || parsed.ptr != input.data() + input.size() || choice == 0 || choice > choices.size())
        return "Zvolte číslo dostupné odpovědi.\n" + showOptions(choices);
    const auto result = choices[choice - 1].execute();
    return session_.state().activeCharacter ? result + "\n" + showOptions(options(*character)) : result;
}

std::vector<DialogueService::Option> DialogueService::options(const std::string& character) {
    auto choices = character == "caretaker" ? caretakerOptions() : ownerOptions();
    choices.push_back({"Ukončit rozhovor", [this] {
        session_.state().activeCharacter.reset();
        return "Rozhovor ukončen.";
    }});
    return choices;
}

std::vector<DialogueService::Option> DialogueService::caretakerOptions() {
    std::vector<Option> choices{{"Co se stalo s obyvateli vily?", [this] {
        return recordStaff("Správce: Majitelka šla před bouří do krytu. Ostatní personál odjel. "
            "Poranil jsem si nohu a nedostal ji ven. V dílně je návod ke generátoru.");
    }}};
    if (session_.state().has(Flag::NotebookRead) && !session_.state().owns("key"))
        choices.push_back({"Požádat o klíč od pracovny", [this] { return giveKey(); }});
    return choices;
}

std::vector<DialogueService::Option> DialogueService::ownerOptions() {
    std::vector<Option> choices{{"Co způsobilo uzavření krytu?", [this] { return explainAccident(); }}};
    if (!session_.state().has(Flag::OwnerRescued))
        choices.push_back({"Doprovodit majitelku na verandu", [this] { return rescueOwner(); }});
    return choices;
}

std::string DialogueService::giveKey() {
    session_.state().inventory.insert("key");
    return "Správce: Tady je klíč od pracovny. Předává vám mosazný klíč.";
}

std::string DialogueService::explainAccident() {
    session_.state().record({"testimony", "Majitelka potvrzuje, že při bouři vypadl proud "
        "a dveře krytu se zablokovaly. Nouzové odjištění uvnitř chybí.", Flag::CauseKnown});
    return recordStaff("Majitelka: Po výpadku se zámek zablokoval. Uvnitř chybí nouzové odjištění, "
        "přestože ho servis slíbil. Přežila jsem díky zásobě vody.");
}

std::string DialogueService::rescueOwner() {
    explainAccident();
    session_.state().flags.insert(Flag::OwnerRescued);
    session_.state().record({"rescue", "Majitelka byla vyvedena z krytu na verandu. Je naživu."});
    session_.state().characterLocations.at("owner") = "veranda";
    session_.state().locationId = "veranda";
    session_.state().activeCharacter.reset();
    return "Doprovázíte majitelku na verandu. Je v bezpečí.\n" + view_.look();
}

std::string DialogueService::recordStaff(const std::string& response) {
    session_.state().record({"staff", "Kuchařka a zahradník odjeli před bouří. "
        "Při výpadku byl správce v kuchyni a majitelka v krytu."});
    return response;
}

std::string DialogueService::showOptions(const std::vector<Option>& choices) {
    std::vector<std::string> lines;
    for (std::size_t i = 0; i < choices.size(); ++i)
        lines.push_back(std::to_string(i + 1) + ". " + choices[i].label);
    lines.emplace_back("Volba: odpověz <číslo>.");
    return Text::join(lines, "\n");
}

}
