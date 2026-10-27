# Podklady k obhajobě

Hra je dokončená v C++17 včetně volitelných dialogů, paměťového ukládání a nápovědy. Níže jsou vazby mezi požadavky zadání a implementací. Diagramy i průchody odpovídají této verzi projektu.

## Pokrytí zadání

| Požadavek | Realizace a ověření |
| --- | --- |
| Hlavní komunikační smyčka | `src/console.cpp`; příkazy až do `konec` nebo EOF |
| Svět v dynamických strukturách | `include/vila/model.hpp`; `std::map` a `std::vector`, 12 lokací |
| Pohyb mezi lokacemi | `src/navigation.cpp`; směry, aliasy a podmíněné východy |
| Popis reagující na stav | `src/view.cpp`, `src/items.cpp`; východy, generátor, dveře a postavy |
| Vrstva mezi textem a logikou | `src/parser.cpp`, `include/vila/commands.hpp` |
| Návrhový vzor pro příkazy | `src/dispatcher.cpp`; registr strategií přes `std::function` |
| Sbírání, kombinace, použití | `src/items.cpp`, `src/puzzles.cpp`; baterie a drát, klíč, páčidlo |
| Deník oddělený od inventáře | `GameState`; vlastní mapa indicií a množina předmětů |
| NPC a podmíněné možnosti | `src/dialogue.cpp`; správce a majitelka, žádost o klíč až po zápisníku |
| Centralizovaný stav a více konců | `GameState`, `Flag`, `Ending`, `src/report.cpp`; čtyři výsledky |
| Oddělená definice, logika a komunikace | Katalog v `world.cpp` a `locations.cpp`, služby ve vlastních souborech, I/O v `console.cpp` |
| Malé metody a přehledné odpovědnosti | Herní smyčka pouze zajišťuje I/O; pohyb, předměty, hádanky a dialogy mají vlastní služby |
| Uložení a načtení pouze v RAM | `src/session.cpp`; `std::optional<GameState>` a hodnotové kopie |
| Nápověda | `src/hints.cpp`; další krok podle postupu |
| Diagram tříd | `ARCHITEKTURA.md`, grafická verze `diagramy.html` |
| Funkční ukázka | Windows sestavení, `PRUCHODY.md`, 31 scénářů přes CTest |
| Týmová historie v Gitu | Není doložena; nelze ji zpětně vytvořit jako skutečnou spolupráci |

Souboj, plížení a rozvrhy NPC jsou v zadání doporučené možnosti pro větší tým. Do této verze nejsou přidány; pro dokončení vyšetřování nejsou potřeba.

## První obhajoba

Otevřete diagram a spusťte hru. Ukažte `pomoc`, přechod do haly a knihovny, sběr zápisníku a neúspěšný pokus o vstup do pracovny. V kódu vysvětlete cestu od `std::getline` přes parser a dispatcher do `NavigationService`.

Požadovaný plán zbývající práce se vztahuje k okamžiku první obhajoby. Tato verze už obsahuje celý projekt. Při prezentaci dřívější vlastní verze uvádějte skutečný stav a odpovídající plán; nepředstírejte, že zde dokončené části teprve vzniknou.

## Druhá obhajoba

Předveďte uložení, jednu hádanku, změnu popisu, rozhovor a závěrečnou zprávu. Pro úplný konec lze použít `PRUCHODY.md`. Potom spusťte CTest nebo přímo testovací program. U paměťového ukládání ukažte první načtení, další akci a opakované načtení stejné pozice.

Každý člen týmu by měl vlastními slovy vysvětlit zejména:

- Proč parser vrací strukturovaný příkaz a neprovádí herní akce.
- Jak `std::map` a `std::vector` popisují graf světa.
- Proč je definice světa konstantní a stav oddělený.
- Jak `std::function` a registry strategií vybírají obsluhu akce.
- Jak se ověřuje přítomnost cíle a vlastnictví předmětu.
- Proč přiřazení `GameState` kopíruje kolekce, zatímco předání reference nic nekopíruje.
- Proč dispatcher nelze kopírovat a jaká je životnost referencí jeho služeb.
- Co se mění při přidání místnosti, předmětu nebo příkazu.
- Jak příznaky ovlivní popisy, dialogy a výslednou zprávu.

Tyto body jsou témata k pochopení návrhu. Kód neobsahuje komentářový scénář určený k předčítání u obhajoby.

## Git a autorství

Zadání hodnotí průběžnou týmovou práci a výslovně požaduje porozumění návrhu. Tento výstup vznikl s pomocí AI. Neobsahuje dodatečně vyrobené commity ani tvrzení o práci členů týmu.

Repozitář lze založit příkazy `git init` a `git add .`; první commit má zachytit skutečný výchozí stav. Další commity mají odpovídat skutečným změnám a skutečným autorům. Před obhajobou projekt projděte, upravujte a testujte podle vlastní práce a pravidel kurzu.
