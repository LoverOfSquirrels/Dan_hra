#include "vila/services.hpp"

namespace vila {

std::string PuzzleService::use(const std::string& itemName, const std::string& targetName) {
    const auto* target = finder_.feature(targetName);
    if (!target)
        return "Takový cíl tu není.";
    const auto* item = finder_.inventoryItem(itemName);
    const auto& id = item ? item->id : itemName;
    if (target->id == "study-door")
        return unlockStudy(id);
    if (target->id == "generator")
        return repairGenerator(id);
    if (target->id == "shelter-door")
        return openShelter(id);
    if (target->id == "safe")
        return openSafe(itemName);
    return "Na tento objekt nelze předmět použít.";
}

std::string PuzzleService::unlockStudy(const std::string& item) {
    if (session_.state().has(Flag::StudyUnlocked))
        return "Pracovna už je odemčená.";
    if (item != "key" || !session_.state().owns(item))
        return "K odemčení potřebujete mosazný klíč v inventáři.";
    session_.state().flags.insert(Flag::StudyUnlocked);
    return "Odemykáte dveře pracovny.";
}

std::string PuzzleService::repairGenerator(const std::string& item) {
    if (session_.state().has(Flag::PowerOn))
        return "Generátor už běží.";
    if (item != "repair-kit" || !session_.state().owns(item))
        return "Startér potřebuje opravnou sadu z baterie a drátu.";
    session_.state().inventory.erase(item);
    session_.state().flags.insert(Flag::PowerOn);
    return "Připojujete baterii s drátem. Generátor naskočil a napájí zámek krytu.";
}

std::string PuzzleService::openShelter(const std::string& item) {
    if (session_.state().has(Flag::ShelterOpened))
        return "Kryt už je otevřený.";
    if (!session_.state().has(Flag::PowerOn))
        return "Elektrický zámek je bez proudu. Nejprve opravte generátor.";
    if (item != "crowbar" || !session_.state().owns(item))
        return "Zkřížené dveře uvolníte páčidlem, které musíte mít u sebe.";
    session_.state().flags.insert(Flag::ShelterOpened);
    return "Uvolňujete zárubeň páčidlem. Dveře krytu jsou otevřené.";
}

std::string PuzzleService::openSafe(const std::string& code) {
    if (session_.state().has(Flag::RepairFaultProven))
        return "Trezor už je otevřený. Protokol je zaznamenaný v deníku.";
    if (code != "2410")
        return "Trezor se neotevřel. Potřebujete správný čtyřmístný kód.";
    session_.state().record({"protocol", "Podepsaný servisní protokol dokládá, že firma "
        "vyúčtovala nouzové odjištění krytu, ale nenamontovala je. Jde o doloženou nesrovnalost v opravě.",
        Flag::RepairFaultProven});
    return "Trezor je otevřený. Servisní protokol dokládá chybějící nouzové odjištění. Záznam je v deníku.";
}

}
