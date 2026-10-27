# Úplné průchody hry

Následující příkazy obsahují řešení hádanek. Každý samostatný průchod začíná novým spuštěním hry. Příkazy lze zadávat i s diakritikou.

## Úplné vyšetření

```text
jdi sever
jdi zapad
vezmi zapisnik
prozkoumej zapisnik
jdi vychod
jdi vychod
promluv se spravcem
odpovez 2
jdi zapad
jdi zapad
pouzij klic na dvere pracovny
jdi sever
prozkoumej portret
pouzij 2410 na trezor
jdi jih
jdi vychod
jdi vychod
jdi sever
vezmi baterii
jdi jih
jdi vychod
prozkoumej navod
vezmi drat
vezmi pacidlo
spoj drat s baterii
jdi zapad
jdi zapad
jdi dolu
prozkoumej rozvadec
pouzij opravnou sadu na generator
pouzij pacidlo na dvere krytu
jdi sever
promluv se majitelkou
odpovez 2
jdi sever
jdi zapad
jdi jih
zprava
konec
```

Výsledek: **Majitelka zachráněna, příčina i závada opravy doloženy.**

Záchrana přesune hráče i majitelku na verandu. Poslední tři pohyby proto vedou přes kuchyni a halu k autu.

## Záchrana bez servisního protokolu

```text
jdi sever
jdi vychod
jdi sever
vezmi baterii
jdi jih
jdi vychod
vezmi drat
vezmi pacidlo
spoj drat s baterii
jdi zapad
jdi zapad
jdi dolu
pouzij opravnou sadu na generator
pouzij pacidlo na dvere krytu
jdi sever
promluv se majitelkou
odpovez 2
jdi sever
jdi zapad
jdi jih
zprava
konec
```

Výsledek: **Majitelka zachráněna. Odpovědnost servisu není doložena protokolem.**

## Zjištění příčiny bez záchrany

```text
jdi sever
jdi dolu
prozkoumej rozvadec
jdi nahoru
jdi jih
zprava
konec
```

Výsledek: **Příčina zjištěna, záchrana nedokončena.**

## Odjezd bez vyřešení

```text
zprava
konec
```

Výsledek: **Případ zůstal nevyřešený.**

## Ukázka opakovaného načtení

```text
jdi sever
jdi zapad
uloz
vezmi zapisnik
prozkoumej zapisnik
denik
nacti
inventar
denik
rozhlédni se
vezmi zapisnik
nacti
rozhlédni se
konec
```

Po obou načteních leží zápisník opět v místnosti. Inventář i deník odpovídají okamžiku uložení, nikoli pozdějším akcím. Když aplikaci ukončíte a znovu spustíte, tento slot už neexistuje.
