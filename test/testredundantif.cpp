/*
 * cppcheck - c/c++ syntax checking
 * Copyright (C) 2007-2009 Daniel Marjamäki, Reijo Tomperi, Nicolas Le Cam
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */


// Check for dangerous division..
// such as "svar / uvar". Treating "svar" as unsigned data is not good


#define UNIT_TESTING
#include "../src/tokenize.h"
#include "../src/checkother.h"
#include "testsuite.h"

#include <sstream>

extern std::ostringstream errout;

class TestRedundantIf : public TestFixture
{
public:
    TestRedundantIf() : TestFixture("TestRedundantIf")
    { }

private:
    void check(const char code[])
    {
        // Tokenize..
        Tokenizer tokenizer;
        std::istringstream istr(code);
        tokenizer.tokenize(istr, "test.cpp");

        // Clear the error buffer..
        errout.str("");

        // Check for redundant condition..
        CheckOther checkOther(&tokenizer, this);
        checkOther.redundantCondition2();
    }

    void run()
    {
        TEST_CASE(remove1);
        TEST_CASE(remove2);
    }

    void remove1()
    {
        check("void f()\n"
              "{\n"
              "    if (haystack.find(needle) != haystack.end())\n"
              "        haystack.remove(needle);"
              "}\n");
        ASSERT_EQUALS(std::string("[test.cpp:3]: Redundant condition found. The remove function in the STL will not do anything if element doesn't exist\n"), errout.str());
    }

    void remove2()
    {
        check("void f()\n"
              "{\n"
              "    if (haystack.find(needle) != haystack.end())\n"
              "    {\n"
              "        haystack.remove(needle);\n"
              "    }\n"
              "}\n");
        ASSERT_EQUALS(std::string("[test.cpp:3]: Redundant condition found. The remove function in the STL will not do anything if element doesn't exist\n"), errout.str());
    }

};

REGISTER_TEST(TestRedundantIf)

