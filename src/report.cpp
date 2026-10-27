#include "vila/services.hpp"
#include "vila/text.hpp"

namespace vila {

std::string ReportService::finish() {
    auto& state = session_.state();
    if (state.locationId != "driveway")
        return "Zprávu uzavřete u auta na příjezdové cestě. Potom vyšetřování skončí.";
    state.ending = selectEnding();
    std::vector<std::string> lines{"Zpráva pro pojišťovnu", outcome(*state.ending)};
    lines.emplace_back(state.has(Flag::CauseKnown)
        ? "Příčina: bouře přerušila telefon a napájení; zámek krytu se při výpadku zablokoval."
        : "Příčina přerušení komunikace nebyla ověřena.");
    if (state.has(Flag::RepairFaultProven))
        lines.emplace_back("Důkaz: servis vyúčtoval nouzové odjištění, které v krytu chybí. Protokol je v deníku.");
    if (!state.has(Flag::OwnerRescued))
        lines.emplace_back("Majitelka nebyla vyvedena do bezpečí. Zpráva vyžaduje další pátrání.");
    const auto staff = state.journal.find("staff");
    if (staff != state.journal.end())
        lines.push_back("Personál: " + staff->second);
    return Text::join(lines, "\n");
}

Ending ReportService::selectEnding() const {
    if (session_.state().has(Flag::OwnerRescued))
        return session_.state().has(Flag::RepairFaultProven) ? Ending::FullTruth : Ending::Rescue;
    return session_.state().has(Flag::CauseKnown) ? Ending::AccidentExplained : Ending::Unresolved;
}

std::string ReportService::outcome(Ending ending) {
    switch (ending) {
    case Ending::FullTruth: return "Výsledek: Majitelka zachráněna, příčina i závada opravy doloženy.";
    case Ending::Rescue: return "Výsledek: Majitelka zachráněna. Odpovědnost servisu není doložena protokolem.";
    case Ending::AccidentExplained: return "Výsledek: Příčina zjištěna, záchrana nedokončena.";
    case Ending::Unresolved: return "Výsledek: Případ zůstal nevyřešený.";
    }
    return "";
}

}
