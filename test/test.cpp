#include <cosevalues/cosevalues.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"


TEST_SUITE("cosevalues") {


    TEST_CASE("Parsing empty string") {
        auto r = cosevalues::reader::from_string("");
        REQUIRE_EQ(r.text_size(), 0);
        std::size_t n = 0;
        for (auto &row: r.first_to_last_rows())
            ++n;
        REQUIRE_EQ(n, 0);
        for (auto &row: r.second_to_last_rows())
            ++n;
        REQUIRE_EQ(n, 0);
    }

    TEST_CASE("Parsing points header") {
        auto r = cosevalues::reader::from_string("x,y");
        REQUIRE_EQ(r.text_size(), 3);
        std::string x, y;
        auto const parsed = r.first_row().parse(x, y);
        REQUIRE(parsed);
        REQUIRE_EQ(x, "x");
        REQUIRE_EQ(y, "y");
    }


    TEST_CASE("Parsing points") {
        auto r = cosevalues::reader::from_string("x,y\n1,\"2\"");
        int x, y;
        for (auto &row: r.second_to_last_rows()) {
            auto const parsed = row.parse(x, y);
            REQUIRE(parsed);
            REQUIRE_EQ(x, 1);
            REQUIRE_EQ(y, 2);
        }
    }


    TEST_CASE("Parsing quotes") {
        auto r = cosevalues::reader::from_string("lines\n\"quo\"\"ted\"");
        std::string s;
        for (auto &row: r.second_to_last_rows()) {
            auto const parsed = row.parse(s);
            REQUIRE(parsed);
            REQUIRE_EQ(s, "quo\"ted");
        }
    }


    TEST_CASE("Formatting headers") {
        cosevalues::writer w;
        w.format("x", "y");
        REQUIRE_EQ(w.to_string(), "\"x\",\"y\"\n");
    }


    TEST_CASE("Formatting integer data") {
        cosevalues::writer w;
        w.format(-1,2, -3);
        REQUIRE_EQ(w.to_string(), "-1,2,-3\n");
    }


    TEST_CASE("Formatting floating data") {
        cosevalues::writer w;
        w.format(1.1, -2.2, 3.3);
        REQUIRE_EQ(w.to_string(), "1.1,-2.2,3.3\n");
    }


    TEST_CASE("Escaping quotes") {
        REQUIRE_EQ(cosevalues::escape("\"\" \"\" \"\""), "\"\"\"\" \"\"\"\" \"\"\"\"");
    }

}

