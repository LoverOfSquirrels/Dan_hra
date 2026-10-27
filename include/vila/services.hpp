#pragma once

#include "vila/finder.hpp"
#include <functional>

namespace vila {

class GameView {
public:
    explicit GameView(const GameSession& session) : session_(session) {}
    std::string look() const;
    std::string inventory() const;
    std::string journal() const;

private:
    const GameSession& session_;
};

class NavigationService {
public:
    NavigationService(GameSession& session, const GameView& view) : session_(session), view_(view) {}
    std::string move(const std::string& direction);

private:
    GameSession& session_;
    const GameView& view_;
};

class ItemService {
public:
    ItemService(GameSession& session, const ObjectFinder& finder) : session_(session), finder_(finder) {}
    std::string take(const std::string& name);
    std::string examine(const std::string& name);
    std::string combine(const std::string& firstName, const std::string& secondName);

private:
    GameSession& session_;
    const ObjectFinder& finder_;
    std::string describe(const std::string& description, const std::optional<Clue>& clue);
    std::string featureDescription(const Feature& feature) const;
};

class PuzzleService {
public:
    PuzzleService(GameSession& session, const ObjectFinder& finder) : session_(session), finder_(finder) {}
    std::string use(const std::string& itemName, const std::string& targetName);

private:
    GameSession& session_;
    const ObjectFinder& finder_;
    std::string unlockStudy(const std::string& item);
    std::string repairGenerator(const std::string& item);
    std::string openShelter(const std::string& item);
    std::string openSafe(const std::string& code);
};

class DialogueService {
public:
    DialogueService(GameSession& session, const ObjectFinder& finder, const GameView& view)
        : session_(session), finder_(finder), view_(view) {}
    std::string talk(const std::string& name);
    std::string choose(const std::string& input);

private:
    struct Option {
        std::string label;
        std::function<std::string()> execute;
    };
    GameSession& session_;
    const ObjectFinder& finder_;
    const GameView& view_;
    std::vector<Option> options(const std::string& character);
    std::vector<Option> caretakerOptions();
    std::vector<Option> ownerOptions();
    std::string giveKey();
    std::string explainAccident();
    std::string rescueOwner();
    std::string recordStaff(const std::string& response);
    static std::string showOptions(const std::vector<Option>& options);
};

class ReportService {
public:
    explicit ReportService(GameSession& session) : session_(session) {}
    std::string finish();

private:
    GameSession& session_;
    Ending selectEnding() const;
    static std::string outcome(Ending ending);
};

class HintService {
public:
    explicit HintService(const GameSession& session) : session_(session) {}
    std::string hint() const;

private:
    const GameSession& session_;
    std::string powerHint() const;
    std::string evidenceHint() const;
};

}
