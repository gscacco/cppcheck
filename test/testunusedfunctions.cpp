/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2009 Daniel Marjamäki and Cppcheck team.
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


#include "../src/tokenize.h"
#include "testsuite.h"
#include "../src/checkunusedfunctions.h"
#include <sstream>

extern std::ostringstream errout;

class TestUnusedFunctions : public TestFixture
{
public:
    TestUnusedFunctions() : TestFixture("TestUnusedFunctions")
    { }

private:


    void run()
    {
        TEST_CASE(incondition);
        TEST_CASE(return1);
        TEST_CASE(return2);
        TEST_CASE(callback1);
        TEST_CASE(else1);
        TEST_CASE(functionpointer);
        TEST_CASE(template1);
    }

    void check(const char code[])
    {
        // Tokenize..
        Tokenizer tokenizer;
        std::istringstream istr(code);
        tokenizer.tokenize(istr, "test.cpp");

        // Clear the error buffer..
        errout.str("");

        // Check for unused functions..
        CheckUnusedFunctions checkUnusedFunctions(this);
        checkUnusedFunctions.parseTokens(tokenizer);
        checkUnusedFunctions.check();
    }

    void incondition()
    {
        check("int f1()\n"
              "{\n"
              "    if (f1())\n"
              "    { }\n"
              "}\n");
        ASSERT_EQUALS("", errout.str());
    }

    void return1()
    {
        check("int f1()\n"
              "{\n"
              "    return f1();\n"
              "}\n");
        ASSERT_EQUALS("", errout.str());
    }

    void return2()
    {
        check("char * foo()\n"
              "{\n"
              "    return *foo();\n"
              "}\n");
        ASSERT_EQUALS("", errout.str());
    }

    void callback1()
    {
        check("void f1()\n"
              "{\n"
              "    void (*f)() = cond ? f1 : NULL;\n"
              "}\n");
        ASSERT_EQUALS("", errout.str());
    }

    void else1()
    {
        check("void f1()\n"
              "{\n"
              "    if (cond) ;\n"
              "    else f1();\n"
              "}\n");
        ASSERT_EQUALS("", errout.str());
    }

    void functionpointer()
    {
        check("namespace abc {\n"
              "void foo() { }\n"
              "};\n"
              "\n"
              "int main()\n"
              "{\n"
              "    f(&abc::foo);\n"
              "    return 0\n"
              "}\n");
        ASSERT_EQUALS("", errout.str());
    }

    void template1()
    {
        check("template<class T> void foo() { }\n"
              "\n"
              "int main()\n"
              "{\n"
              "    foo<int>();\n"
              "    return 0\n"
              "}\n");
        ASSERT_EQUALS("", errout.str());
    }
};

REGISTER_TEST(TestUnusedFunctions)

