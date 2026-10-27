#pragma once

#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace vila {

enum class Flag {
    NotebookRead, StudyUnlocked, PowerOn, ShelterOpened,
    OwnerRescued, CauseKnown, RepairFaultProven
};

enum class Ending { Unresolved, AccidentExplained, Rescue, FullTruth };

struct Clue {
    std::string id;
    std::string text;
    std::optional<Flag> flag = std::nullopt;
};

struct Item {
    std::string id;
    std::string name;
    std::string description;
    std::string initialLocation;
    std::vector<std::string> aliases;
    std::optional<Clue> clue = std::nullopt;
};

struct Feature {
    std::string id;
    std::string name;
    std::string description;
    std::vector<std::string> aliases;
    std::optional<Clue> clue = std::nullopt;
};

struct GameState;

struct Exit {
    std::string name;
    std::string destination;
    std::vector<std::string> aliases;
    std::optional<Flag> requiredFlag = std::nullopt;
    std::string blockedMessage = "";

    bool isOpen(const GameState& state) const;
};

struct Character {
    std::string id;
    std::string name;
    std::string initialLocation;
    std::vector<std::string> aliases;
};

struct Recipe {
    std::string first;
    std::string second;
    std::string result;
};

struct Location {
    std::string id;
    std::string name;
    std::string description;
    std::vector<Exit> exits;
    std::vector<Feature> features;
};

struct GameWorld {
    std::map<std::string, Location> locations;
    std::map<std::string, Item> items;
    std::map<std::string, Character> characters;
    std::vector<Recipe> recipes;
};

struct GameState {
    std::string locationId = "driveway";
    std::set<std::string> inventory;
    std::map<std::string, std::string> itemLocations;
    std::map<std::string, std::string> characterLocations;
    std::map<std::string, std::string> journal;
    std::set<Flag> flags;
    std::optional<Ending> ending;
    std::optional<std::string> activeCharacter;

    bool has(Flag flag) const;
    bool owns(const std::string& item) const;
    bool record(const Clue& clue);
    static GameState create(const GameWorld& world);
};

}
