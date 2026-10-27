#include "vila/services.hpp"
#include <algorithm>

namespace vila {

std::string ItemService::take(const std::string& name) {
    if (finder_.inventoryItem(name))
        return "Tento předmět už máte.";
    const auto* item = finder_.roomItem(name);
    if (!item)
        return finder_.feature(name) ? "Tento objekt nelze odnést." : "Takový předmět tu není.";
    session_.state().itemLocations.erase(item->id);
    session_.state().inventory.insert(item->id);
    return "Berete: " + item->name + ".";
}

std::string ItemService::examine(const std::string& name) {
    auto* item = finder_.inventoryItem(name);
    if (!item)
        item = finder_.roomItem(name);
    if (item)
        return describe(item->description, item->clue);
    if (const auto* feature = finder_.feature(name))
        return describe(featureDescription(*feature), feature->clue);
    if (const auto* character = finder_.character(name))
        return "Můžete promluvit s postavou: " + character->name + ".";
    return "Takový předmět ani objekt tu není a nemáte ho u sebe.";
}

std::string ItemService::combine(const std::string& firstName, const std::string& secondName) {
    const auto* first = finder_.inventoryItem(firstName);
    const auto* second = finder_.inventoryItem(secondName);
    if (!first || !second)
        return "Ke spojení musíte mít oba předměty v inventáři.";
    const auto& recipes = session_.world().recipes;
    const auto recipe = std::find_if(recipes.begin(), recipes.end(), [first, second](const auto& candidate) {
        return (candidate.first == first->id && candidate.second == second->id)
            || (candidate.first == second->id && candidate.second == first->id);
    });
    if (recipe == recipes.end())
        return "Tyto předměty nelze spojit.";
    session_.state().inventory.erase(first->id);
    session_.state().inventory.erase(second->id);
    session_.state().inventory.insert(recipe->result);
    return "Vytváříte: " + session_.world().items.at(recipe->result).name + ".";
}

std::string ItemService::describe(const std::string& description, const std::optional<Clue>& clue) {
    return clue && session_.state().record(*clue)
        ? description + "\nDo deníku přibyl záznam." : description;
}

std::string ItemService::featureDescription(const Feature& feature) const {
    const auto& state = session_.state();
    if (feature.id == "study-door" && state.has(Flag::StudyUnlocked))
        return "Dveře pracovny jsou odemčené.";
    if (feature.id == "generator" && state.has(Flag::PowerOn))
        return "Generátor běží a napájí elektrický zámek krytu.";
    if (feature.id == "shelter-door" && state.has(Flag::ShelterOpened))
        return "Dveře krytu jsou otevřené.";
    if (feature.id == "shelter-door" && state.has(Flag::PowerOn))
        return "Zámek je napájený, ale dveře jsou zkřížené. Potřebujete páčidlo.";
    if (feature.id == "safe" && state.has(Flag::RepairFaultProven))
        return "Trezor je otevřený. Kopii servisního protokolu máte v deníku.";
    return feature.description;
}

}
