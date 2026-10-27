#include "vila/world.hpp"

namespace vila {
namespace {

Location driveway() {
    return {"driveway", "Příjezdová cesta",
        "Stojíte před vilou Rubín. Vchod poškodila bouře. Vaše auto čeká u brány.",
        {{"sever", "foyer", {"vchod", "vila", "dovnitr"}}},
        {{"car", "auto", "Zde můžete uzavřít vyšetřování příkazem zpráva.", {"automobil"}}}};
}

Location foyer() {
    return {"foyer", "Vstupní hala",
        "Na podlaze je voda ze střechy. Schodiště vede do patra, kamenné schody do sklepa.",
        {{"jih", "driveway", {"ven", "auto", "prijezdova cesta"}},
         {"západ", "library", {"knihovna", "knihovny"}},
         {"východ", "kitchen", {"kuchyne"}},
         {"nahoru", "landing", {"patro", "schody"}},
         {"dolů", "cellar", {"sklep", "sklepa"}}}, {}};
}

Location library() {
    return {"library", "Knihovna",
        "Na čtecím stole leží věci majitelky. Vedle regálů jsou dveře pracovny.",
        {{"východ", "foyer", {"hala", "haly"}},
         {"sever", "study", {"pracovna", "pracovny"}, Flag::StudyUnlocked,
             "Pracovna je zamčená. Potřebujete klíč."}},
        {{"study-door", "dveře pracovny", "V zámku je otvor pro malý mosazný klíč.",
            {"dvere", "pracovna", "zamek", "dvere do pracovny"}}}};
}

Location study() {
    return {"study", "Pracovna",
        "Pracovní stůl stojí pod rodinným portrétem. Ve zdi je číselný trezor.",
        {{"jih", "library", {"knihovna", "knihovny"}}},
        {{"portrait", "portrét", "Popisek pod fotografií: Svatba, 24. 10. Čtyři číslice bez teček.",
             {"obraz", "fotografie"}, Clue{"portrait", "Na portrétu je datum 24. 10.; kód má čtyři číslice."}},
         {"safe", "trezor", "Číselný zámek má čtyři místa. Kód zadáte jako použij <kód> na trezor.", {"sejf"}}}};
}

Location kitchen() {
    return {"kitchen", "Kuchyně",
        "V krbu dohořívá oheň. Správce tu ošetřoval poraněnou nohu.",
        {{"západ", "foyer", {"hala", "haly"}},
         {"sever", "pantry", {"spiz", "spize"}},
         {"východ", "workshop", {"dilna", "dilny"}},
         {"jih", "veranda", {"veranda", "verandu"}}}, {}};
}

Location pantry() {
    return {"pantry", "Spíž", "Na polici jsou zásoby a součástky pro nouzové napájení.",
        {{"jih", "kitchen", {"kuchyne"}}}, {}};
}

Location workshop() {
    return {"workshop", "Dílna", "Pod ponkem je nářadí. Na stěně visí návod ke generátoru.",
        {{"západ", "kitchen", {"kuchyne"}}},
        {{"manual", "návod", "Generátor: spojte baterii s drátem a sadu připojte ke startéru. "
            "Po obnovení proudu lze uvolnit dveře krytu páčidlem.", {"manual", "navod ke generatoru"},
            Clue{"manual", "Generátor potřebuje baterii s drátem. Napájené dveře krytu je nutné uvolnit páčidlem."}}}};
}

Location cellar() {
    return {"cellar", "Sklep", "Denní světlo dopadá sklepním oknem na generátor a dveře nouzového krytu.",
        {{"nahoru", "foyer", {"hala", "haly"}},
         {"sever", "shelter", {"kryt", "krytu"}, Flag::ShelterOpened,
             "Kryt je uzavřený. Elektrický zámek potřebuje proud a zkřížené dveře páčidlo."}},
        {{"generator", "generátor", "Startéru chybí baterie a přívodní drát.", {"starter"}},
         {"shelter-door", "dveře krytu", "Elektrický zámek a zkřížená zárubeň brání otevření.",
             {"dvere", "kryt", "zamek", "dvere do krytu"}},
         {"damage", "rozvaděč", "Blesk spálil rozvaděč. Zámek krytu zůstal při výpadku uzavřený; "
             "telefonní kabel přerušila spadlá větev.", {"kabel", "poskozeni"},
             Clue{"damage", "Bouře zničila napájení a telefon. Elektrický zámek uvěznil obyvatele krytu.", Flag::CauseKnown}}}};
}

Location landing() {
    return {"landing", "Patro", "Část chodby zakryl stropní trám. Bezpečně lze dojít do ložnice.",
        {{"dolů", "foyer", {"hala", "haly"}}, {"východ", "bedroom", {"loznice"}}}, {}};
}

Location bedroom() {
    return {"bedroom", "Ložnice", "Postel není rozestlaná. Na stolku leží evakuační seznam.",
        {{"západ", "landing", {"patro", "chodba"}}},
        {{"list", "evakuační seznam", "Kuchařka a zahradník odjeli před bouří. "
             "Majitelka šla zkontrolovat zásoby vody v krytu; správce zůstal v kuchyni.",
             {"seznam", "papir"}, Clue{"staff", "Kuchařka a zahradník odjeli před bouří. "
                 "Při výpadku byl správce v kuchyni a majitelka v krytu."}}}};
}

Location veranda() {
    return {"veranda", "Veranda", "Střecha verandy vydržela. Je tu suchá lavice a přístup čerstvého vzduchu.",
        {{"sever", "kitchen", {"kuchyne"}}}, {}};
}

Location shelter() {
    return {"shelter", "Nouzový kryt", "Nouzová nádrž poskytovala pitnou vodu. Dveře se zevnitř nedaly otevřít.",
        {{"jih", "cellar", {"sklep", "sklepa"}}}, {}};
}

}

std::vector<Location> LocationCatalog::create() {
    return {driveway(), foyer(), library(), study(), kitchen(), pantry(),
        workshop(), cellar(), landing(), bedroom(), veranda(), shelter()};
}

}
