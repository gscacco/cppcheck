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
#include "../src/checkclass.h"
#include "testsuite.h"

#include <sstream>

extern std::ostringstream errout;

class TestUnusedPrivateFunction : public TestFixture
{
public:
    TestUnusedPrivateFunction() : TestFixture("TestUnusedPrivateFunction")
    { }

private:
    void run()
    {
        TEST_CASE(test1);
        TEST_CASE(test2);
        TEST_CASE(test3);
        TEST_CASE(test4);

        // [ 2236547 ] False positive --style unused function, called via pointer
        TEST_CASE(func_pointer);

        TEST_CASE(ctor);

        TEST_CASE(classInClass);
        TEST_CASE(sameFunctionNames);
        TEST_CASE(incompleteImplementation);
    }


    void check(const char code[])
    {
        // Tokenize..
        Tokenizer tokenizer;
        std::istringstream istr(code);
        tokenizer.tokenize(istr, "test.cpp");

        // Clear the error buffer..
        errout.str("");

        // Check for unused private functions..
        Settings settings;
        settings._checkCodingStyle = true;
        CheckClass checkClass(&tokenizer, &settings, this);
        checkClass.privateFunctions();
    }



    void test1()
    {
        check("class Fred\n"
              "{\n"
              "private:\n"
              "    unsigned int f();\n"
              "public:\n"
              "    Fred();\n"
              "};\n"
              "\n"
              "Fred::Fred()\n"
              "{ }\n"
              "\n"
              "unsigned int Fred::f()\n"
              "{ }\n");

        ASSERT_EQUALS("[test.cpp:4]: (style) Unused private function 'Fred::f'\n", errout.str());

        check("#file \"p.h\"\n"
              "class Fred\n"
              "{\n"
              "private:\n"
              "    unsigned int f();\n"
              "public:\n"
              "    Fred();\n"
              "};\n"
              "\n"
              "#endfile\n"
              "Fred::Fred()\n"
              "{ }\n"
              "\n"
              "unsigned int Fred::f()\n"
              "{ }\n");

        TODO_ASSERT_EQUALS("[p.h:4]: (style) Unused private function 'Fred::f'\n", errout.str());

        check("#file \"p.h\"\n"
              "class Fred\n"
              "{\n"
              "private:\n"
              "void f();\n"
              "};\n"
              "\n"
              "\n"
              "#endfile\n"
              "\n"
              "void Fred::f()\n"
              "{\n"
              "}\n"
              "\n");
        TODO_ASSERT_EQUALS("[p.h:4]: (style) Unused private function 'Fred::f'\n", errout.str());

        // Don't warn about include files which implementation we don't see
        check("#file \"p.h\"\n"
              "class Fred\n"
              "{\n"
              "private:\n"
              "void f();\n"
              "void g() {}\n"
              "};\n"
              "\n"
              "#endfile\n"
              "\n"
              "int main()\n"
              "{\n"
              "}\n"
              "\n");
        ASSERT_EQUALS("", errout.str());
    }


    void test2()
    {
        check("class A {\n"
              "public:\n"
              "    A();\n"
              "\n"
              "    void a() const\n"
              "    { b(); }\n"
              "private:\n"
              "    void b( ) const\n"
              "    { }\n"
              "};\n"
              "\n"
              "A::A()\n"
              "{ }\n");
        ASSERT_EQUALS("", errout.str());
    }


    void test3()
    {
        check("class A {\n"
              "public:\n"
              "    A() { }\n"
              "    ~A();\n"
              "private:\n"
              "    void B() { }\n"
              "};\n"
              "\n"
              "A::~A()\n"
              "{ B(); }\n");
        ASSERT_EQUALS("", errout.str());
    }


    void test4()
    {
        check("class A {\n"
              "public:\n"
              "    A();\n"
              "private:\n"
              "    bool _owner;\n"
              "    void b() { }\n"
              "};\n"
              "\n"
              "A::A() : _owner(false)\n"
              "{ b(); }\n");
        ASSERT_EQUALS("", errout.str());
    }







    void func_pointer()
    {
        check("class Fred\n"
              "{\n"
              "private:\n"
              "    typedef void (*testfp)();\n"
              "\n"
              "    testfp get()\n"
              "    {\n"
              "        return test;\n"
              "    }\n"
              "\n"
              "    static void test()\n"
              "    { }\n"
              "\n"
              "public:\n"
              "    Fred();\n"
              "};\n"
              "\n"
              "Fred::Fred()\n"
              "{}\n");

        ASSERT_EQUALS("[test.cpp:6]: (style) Unused private function 'Fred::get'\n", errout.str());
    }


    void ctor()
    {
        check("class PrivateCtor\n"
              "{\n"
              "private:\n"
              "    PrivateCtor(int threadNum) :\n"
              "        numOfThreads(threadNum)\n"
              "    {\n"
              "    }\n"
              "\n"
              "    int numOfThreads;\n"
              "};\n");

        ASSERT_EQUALS("", errout.str());
    }


    void classInClass()
    {
        check("class A\n"
              "{\n"
              "public:\n"
              "\n"
              "    class B\n"
              "    {\n"
              "    private:\n"
              "    };\n"
              "\n"
              "    static void f()\n"
              "    { }\n"
              "};\n");
        ASSERT_EQUALS("", errout.str());
    }


    void sameFunctionNames()
    {
        check("class A\n"
              "{\n"
              "public:\n"
              "    void a()\n"
              "    {\n"
              "        f(1);\n"
              "    }\n"
              "\n"
              "private:\n"
              "    void f() { }\n"
              "    void f(int) { }\n"
              "};");
        ASSERT_EQUALS("", errout.str());
    }

    void incompleteImplementation()
    {
        // The implementation for "A::a" is missing - so don't check if
        // "A::b" is used or not
        check("#file \"test.h\"\n"
              "class A\n"
              "{\n"
              "public:\n"
              "    A()\n"
              "    void a();\n"
              "private:\n"
              "    void b();\n"
              "};\n"
              "#endfile\n"
              "A::A() { }\n"
              "void A::b() { }\n");
        ASSERT_EQUALS("", errout.str());
    }
};

REGISTER_TEST(TestUnusedPrivateFunction)

