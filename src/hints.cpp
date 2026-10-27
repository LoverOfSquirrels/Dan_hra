#include "vila/services.hpp"

namespace vila {

std::string HintService::hint() const {
    const auto& state = session_.state();
    if (!state.has(Flag::PowerOn))
        return powerHint();
    if (!state.has(Flag::ShelterOpened))
        return state.owns("crowbar") ? "Ve sklepě použijte páčidlo na dveře krytu."
            : "V dílně je páčidlo pro uvolnění dveří krytu.";
    if (!state.has(Flag::OwnerRescued))
        return "Vstupte do krytu, promluvte s majitelkou a nabídněte doprovod na verandu.";
    if (!state.has(Flag::RepairFaultProven))
        return evidenceHint();
    return "Vraťte se k autu na příjezdové cestě a uzavřete případ příkazem zpráva.";
}

std::string HintService::powerHint() const {
    const auto& state = session_.state();
    if (state.owns("repair-kit"))
        return "Ve sklepě použijte opravnou sadu na generátor.";
    if (!state.owns("wire"))
        return "V dílně najdete drát a návod k opravě generátoru.";
    if (!state.owns("battery"))
        return "Ve spíži je baterie pro generátor.";
    return "Spojte drát s baterií příkazem spoj drát s baterií.";
}

std::string HintService::evidenceHint() const {
    const auto& state = session_.state();
    if (!state.has(Flag::NotebookRead))
        return "Prozkoumejte zápisník v knihovně. Obsahuje pokyny k pracovně a trezoru.";
    if (!state.owns("key"))
        return "Promluvte se správcem v kuchyni a požádejte o klíč od pracovny.";
    if (!state.has(Flag::StudyUnlocked))
        return "V knihovně použijte mosazný klíč na dveře pracovny.";
    return "V pracovně prozkoumejte portrét. Den a měsíc bez teček zadejte jako použij <kód> na trezor.";
}

}
