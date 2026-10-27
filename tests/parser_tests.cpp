#include "test.hpp"

namespace tests {

namespace {

void normalization() {
    CommandParser parser;
    const auto command = parser.parse("  POUŽIJ\t MOSAZNÝ   KLÍČ  na DVEŘE  ");
    equal(CommandKind::Use, command.kind);
    equal("mosazny klic", command.subject);
    equal("dvere", command.target);
    equal("zapisnik", Text::normalize("za\u0301pisni\u0301k"));
    equal("prilis zlutoucky kun", Text::normalize("PŘÍLIŠ ŽLUŤOUČKÝ KŮŇ"));
}

void argumentsAndAliases() {
    CommandParser parser;
    equal("sever", parser.parse("jdi na sever").subject);
    equal("knihovny", parser.parse("jdi do knihovny").subject);
    equal("spravcem", parser.parse("promluv se správcem").subject);
    for (const auto* text : {"spoj drát s baterií", "spoj drát + baterií", "zkombinuj drát a baterií"}) {
        const auto command = parser.parse(text);
        equal(CommandKind::Combine, command.kind);
        equal("drat", command.subject);
        equal("baterii", command.target);
    }
}

void invalidInput() {
    CommandParser parser;
    for (const auto* text : {"", "  ", "xyz", "použij klíč", "spoj drát", "vezmi", "jdi", "inventář navíc"})
        equal(CommandKind::Invalid, parser.parse(text).kind);
    for (const auto& text : {std::string("\xff\xc0"), std::string("\xe2\x82"), std::string("\xf4\x90\x80\x80")})
        equal(CommandKind::Invalid, parser.parse(text).kind);
}

}

void addParserTests(std::vector<Test>& tests) {
    tests.emplace_back("Normalizace češtiny a mezer", normalization);
    tests.emplace_back("Argumenty a aliasy příkazů", argumentsAndAliases);
    tests.emplace_back("Chybné a prázdné vstupy", invalidInput);
}

}
