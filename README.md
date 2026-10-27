# Vila Rubín

Konzolová detektivní hra v **C++17** podle zadání „Projekt 26–27Z“. Hráč vyšetřuje přerušení komunikace s vilou, hledá obyvatele a u auta uzavírá zprávu pro pojišťovnu.

Projekt obsahuje 12 propojených lokací, práci s předměty, tři navazující hádanky, samostatný deník, dvě postavy s podmíněnými dialogy a čtyři výsledky vyšetřování. Uložení a načtení funguje pouze v paměti během jednoho spuštění. Nápověda reaguje na postup hráče.

## Spuštění ve Windows

Otevřete **Spustit.cmd**, případně přímo **hra/vila_rubin.exe**. Přiložené sestavení je pro Windows x64 a má staticky připojenou C++ běhovou knihovnu. K jeho spuštění není potřeba kompilátor ani další instalace.

```powershell
.\hra\vila_rubin.exe
```

## Překlad ze zdrojů

Potřebujete CMake 3.20 nebo novější a kompilátor s podporou C++17, například GCC, Clang nebo MSVC. Zdrojový kód je v UTF-8. Projekt používá pouze standardní knihovnu; nestahuje žádné balíčky a s připraveným kompilátorem se sestaví bez internetu.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
```

První příkaz zvolí dostupný výchozí generátor. Pro MinGW ve Windows s nástroji na `PATH` lze konfiguraci určit výslovně:

```sh
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
```

Generátor se vybírá při založení adresáře sestavení. Pro jiný generátor použijte nový adresář, například `build-msvc`.

Po překladu spusťte podle prostředí:

```text
Windows, MinGW:         build\vila_rubin.exe
Windows, Visual Studio: build\Release\vila_rubin.exe
Linux nebo macOS:       ./build/vila_rubin
```

Kód hry je přenositelný. Nastavení UTF-8 konzole přes Windows API je oddělené v `src/main.cpp` a na ostatních systémech se nepřekládá. Přiložené binární sestavení a ověření pochází z Windows x64 s GCC 16.2; ostatní kompilátory zde nebyly spuštěny.

Nové sestavení lze zkopírovat do složky `hra` standardní instalací CMake:

```sh
cmake --install build --config Release --prefix hra
```

## Ovládání

| Příkaz | Význam |
| --- | --- |
| `rozhlédni se` | Místnost, předměty, postavy a východy |
| `jdi na sever` nebo `jdi knihovna` | Pohyb uvedeným východem |
| `vezmi zápisník` | Přidání předmětu do inventáře |
| `prozkoumej zápisník` | Prohlídka dostupné věci nebo objektu, případně nalezení indicie |
| `použij klíč na dveře` | Použití vlastněného předmětu na místní cíl |
| `spoj drát s baterií` | Kombinování dvou předmětů z inventáře |
| `promluv se správcem` | Rozhovor s přítomnou postavou |
| `odpověz 1` | Volba z aktuálního rozhovoru |
| `inventář`, `deník` | Předměty, případně nalezené informace |
| `nápověda`, `pomoc` | Další krok, případně seznam příkazů |
| `ulož`, `načti` | Jeden uložený stav v RAM; nové uložení nahradí předchozí |
| `zpráva` | U auta uzavře vyšetřování a určí konec |
| `konec` | Ukončí aplikaci |

Příkazy přijímají česká velká písmena, nadbytečné mezery i text bez diakritiky. Názvy vycházejí z výpisu místnosti a inventáře. Vybrané tvary jako `baterii` a `opravnou sadu` jsou také podporovány. Nejde o obecné rozpoznávání přirozeného jazyka.

Po uzavření zprávy se svět dál nemění. Stále lze číst deník, prohlížet stav, uložit jej nebo načíst dřívější hru. Příkaz `konec` a konec vstupního proudu ukončí aplikaci bez vytvoření nového výsledku vyšetřování.

## Testy

CTest spouští program `vila_tests`, který provede **31 scénářů**. Při selhání vrátí nenulový návratový kód. Podrobnosti lze vypsat i přímým spuštěním:

```powershell
.\build\vila_tests.exe
```

Pro Visual Studio je program v `build/Release`; na Linuxu a macOS se jmenuje `build/vila_tests`.

Testy ověřují parser, české UTF-8 včetně rozložené diakritiky a chybného vstupu, graf světa, všechny konce, různá pořadí řešení, vlastnictví předmětů, dialogy, aktuální popisy a nezávislé kopie při ukládání. Překlad používá vysokou úroveň varování a považuje je za chyby.

## Struktura projektu

```text
include/vila/   deklarace modelu, parseru, relace a služeb
src/            implementace; main.cpp pouze sestaví a spustí aplikaci
tests/          automatické průchody a kontroly stavu
docs/           návrh, diagramy, obhajoba a řešení hry
hra/            připravený program pro Windows x64
CMakeLists.txt  společné sestavení hry a testů
```

[Objektový návrh](docs/ARCHITEKTURA.md) vysvětluje rozdělení odpovědností, hodnotové kopírování a použité vzory. [Diagram tříd a mapa](docs/diagramy.html) se otevřou v prohlížeči bez serveru. [Podklady k obhajobě](docs/OBHAJOBA.md) přiřazují požadavky ke konkrétním souborům. [Úplné průchody](docs/PRUCHODY.md) obsahují řešení všech konců.

Historie skutečné týmové spolupráce není součástí tohoto jednorázově připraveného řešení. Zadání ji hodnotí samostatně a vyžaduje vlastní vysvětlení návrhu i obhajobu kódu.
