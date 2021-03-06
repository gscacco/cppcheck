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



#include "testsuite.h"
#define private public
#include "../src/tokenize.h"
#undef private
#include "../src/token.h"
#include <sstream>

extern std::ostringstream errout;


class TestSimplifyTokens : public TestFixture
{
public:
    TestSimplifyTokens() : TestFixture("TestSimplifyTokens")
    { }


private:

    void run()
    {
        TEST_CASE(cast0);
        TEST_CASE(cast1);
        TEST_CASE(iftruefalse);
        TEST_CASE(combine_strings);
        TEST_CASE(double_plus);
        TEST_CASE(redundant_plus);
        TEST_CASE(parantheses1);
        TEST_CASE(paranthesesVar);      // Remove redundant parantheses around variable .. "( %var% )"
        TEST_CASE(declareVar);

        TEST_CASE(declareArray);

        TEST_CASE(dontRemoveIncrement);
        TEST_CASE(removePostIncrement);
        TEST_CASE(removePreIncrement);

        TEST_CASE(elseif1);

        TEST_CASE(sizeof1);
        TEST_CASE(sizeof2);
        TEST_CASE(sizeof3);
        TEST_CASE(sizeof4);
        TEST_CASE(sizeof5);
        TEST_CASE(sizeof6);
        TEST_CASE(sizeof7);
        TEST_CASE(sizeof8);
        TEST_CASE(casting);

        TEST_CASE(template1);
        TEST_CASE(template2);
        TEST_CASE(template3);
        TEST_CASE(template4);
        TEST_CASE(template5);
        TEST_CASE(template6);
        TEST_CASE(template7);
        TEST_CASE(template8);
        TEST_CASE(template9);
        TEST_CASE(template10);
        TEST_CASE(template11);
        TEST_CASE(template12);

        TEST_CASE(namespaces);

        // Assignment in condition..
        TEST_CASE(ifassign1);
        TEST_CASE(whileAssign);

        // "if(0==x)" => "if(!x)"
        TEST_CASE(ifnot);
        TEST_CASE(combine_wstrings);

        // Simplify "not" to "!" (#345)
        TEST_CASE(not1);
        TEST_CASE(comma_keyword);
        TEST_CASE(remove_comma);

        // Simplify "?:"
        TEST_CASE(conditionOperator);

        // Simplify calculations
        TEST_CASE(calculations);
    }

    std::string tok(const char code[])
    {
        std::istringstream istr(code);
        Tokenizer tokenizer;
        tokenizer.tokenize(istr, "test.cpp");
        tokenizer.simplifyTokenList();
        std::string ret;
        for (const Token *tok = tokenizer.tokens(); tok; tok = tok->next())
        {
            if (tok != tokenizer.tokens())
                ret += " ";
            ret += tok->str();
        }

        return ret;
    }

    void cast0()
    {
        const char code1[] = " if ( p == (char *)0 ) ";
        const char code2[] = " if ( p == 0 ) ";
        ASSERT_EQUALS(tok(code1), tok(code2));
    }

    void cast1()
    {
        const char code[] = "return (unsigned char *)str;";
        const char expected[] = "return str;";
        ASSERT_EQUALS(tok(expected), tok(code));
    }

    void iftruefalse()
    {
        {
            const char code1[] = " void f() { int a; bool use = false; if( use ) { a=0; } else {a=1;} }";
            const char code2[] = " void f() { int a; bool use = false; {a=1;} }";
            ASSERT_EQUALS(tok(code2), tok(code1));
        }

        {
            const char code1[] = " void f() { int a; bool use = true; if( use ) { a=0; } else {a=1;} }";
            const char code2[] = " void f() { int a; bool use = true; { a=0; } }";
            ASSERT_EQUALS(tok(code2), tok(code1));
        }

        {
            const char code1[] = " void f() { int a; int use = 5; if( use ) { a=0; } else {a=1;} }";
            const char code2[] = " void f() { int a; int use = 5; { a=0; } }";
            ASSERT_EQUALS(tok(code2), tok(code1));
        }

        {
            const char code1[] = " void f() { int a; int use = 0; if( use ) { a=0; } else {a=1;} }";
            const char code2[] = " void f() { int a; int use = 0; {a=1;} }";
            ASSERT_EQUALS(tok(code2), tok(code1));
        }

        {
            const char code1[] = " void f() { int a; bool use = false; if( use ) a=0; else a=1; int c=1; }";
            const char code2[] = " void f() { int a; bool use = false; { a=1; } int c=1; }";
            ASSERT_EQUALS(tok(code2), tok(code1));
        }

        {
            const char code1[] = " void f() { int a; bool use = true; if( use ) a=0; else a=1; int c=1; }";
            const char code2[] = " void f() { int a; bool use = true; { a=0; } int c=1; }";
            ASSERT_EQUALS(tok(code2), tok(code1));
        }

        {
            const char code1[] = " void f() { int a; bool use = false; if( use ) a=0; else if( bb ) a=1; int c=1; }";
            const char code2[] = " void f ( ) { int a ; bool use ; use = false ; { if ( bb ) { a = 1 ; } } int c ; c = 1 ; }";
            ASSERT_EQUALS(tok(code2), tok(code1));
        }

        {
            const char code1[] = " void f() { int a; bool use = true; if( use ) a=0; else if( bb ) a=1; int c=1; }";
            const char code2[] = " void f() { int a; bool use = true; { a=0;} int c=1; }";
            ASSERT_EQUALS(tok(code2), tok(code1));
        }

        {
            const char code1[] = " void f() { int a; bool use = true; if( use ) a=0; else if( bb ) a=1; else if( cc ) a=33; else { gg = 0; } int c=1; }";
            const char code2[] = " void f() { int a; bool use = true; { a=0; }int c=1; }";
            ASSERT_EQUALS(tok(code2), tok(code1));
        }

        {
            const char code1[] = " void f() { if( aa ) { a=0; } else if( true ) a=1; else { a=2; } }";
            const char code2[] = " void f ( ) { if ( aa ) { a = 0 ; } else { { a = 1 ; } } }";
            ASSERT_EQUALS(tok(code2), tok(code1));
        }

        {
            const char code1[] = " void f() { if( aa ) { a=0; } else if( false ) a=1; else { a=2; } }";
            const char code2[] = " void f ( ) { if ( aa ) { a = 0 ; } else { { a = 2 ; } } }";
            ASSERT_EQUALS(tok(code2), tok(code1));
        }
    }

    void combine_strings()
    {
        const char code1[] =  "void foo()\n"
                              "{\n"
                              "const char *a =\n"
                              "{\n"
                              "\"hello \"\n"
                              "\"world\"\n"
                              "};\n"
                              "}\n";

        const char code2[] =  "void foo()\n"
                              "{\n"
                              "const char *a =\n"
                              "{\n"
                              "\"hello world\"\n"
                              "};\n"
                              "}\n";
        ASSERT_EQUALS(tok(code2), tok(code1));
    }

    void combine_wstrings()
    {
        const char code1[] =  "void foo()\n"
                              "{\n"
                              "const wchar_t *a =\n"
                              "{\n"
                              "L\"hello \"\n"
                              "L\"world\"\n"
                              "};\n"
                              "}\n";

        const char code2[] =  "void foo()\n"
                              "{\n"
                              "const wchar_t *a =\n"
                              "{\n"
                              "\"hello world\"\n"
                              "};\n"
                              "}\n";
        ASSERT_EQUALS(tok(code2), tok(code1));
    }

    void double_plus()
    {
        {
            const char code1[] =  "void foo( int a )\n"
                                  "{\n"
                                  "a++;\n"
                                  "a--;\n"
                                  "++a;\n"
                                  "--a;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a ) { a ++ ; a -- ; ++ a ; -- a ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a )\n"
                                  "{\n"
                                  "a=a+a;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a ) { a = a + a ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a+++b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a ++ + b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a---b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a -- - b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a--+b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a -- + b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a++-b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a ++ - b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a+--b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a + -- b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a-++b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a - ++ b ; }", tok(code1));
        }
    }

    void redundant_plus()
    {
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a + + b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a + b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a + + + b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a + b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a + - b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a - b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a - + b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a - b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a - - b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a + b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a - + - b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a + b ; }", tok(code1));
        }
        {
            const char code1[] =  "void foo( int a, int b )\n"
                                  "{\n"
                                  "a=a - - - b;\n"
                                  "}\n";
            ASSERT_EQUALS("void foo ( int a , int b ) { a = a - b ; }", tok(code1));
        }
    }


    void parantheses1()
    {
        const char code1[] = "<= (10+100);";
        ASSERT_EQUALS("<= 110 ;", tok(code1));
    }

    void paranthesesVar()
    {
        // remove parantheses..
        ASSERT_EQUALS("= p ;", tok("= (p);"));
        ASSERT_EQUALS("if ( a < p ) { }", tok("if(a<(p)){}"));
        ASSERT_EQUALS("void f ( ) { int p ; if ( p == -1 ) { } }", tok("void f(){int p; if((p)==-1){}}"));
        ASSERT_EQUALS("void f ( ) { int p ; if ( -1 == p ) { } }", tok("void f(){int p; if(-1==(p)){}}"));
        ASSERT_EQUALS("void f ( ) { int p ; if ( p ) { } }", tok("void f(){int p; if((p)){}}"));
        ASSERT_EQUALS("return p ;", tok("return (p);"));

        // keep parantheses..
        ASSERT_EQUALS("= a ;", tok("= (char)a;"));
        ASSERT_EQUALS("cast < char * > ( p )", tok("cast<char *>(p)"));
        ASSERT_EQUALS("return ( a + b ) * c ;", tok("return (a+b)*c;"));
    }

    void declareVar()
    {
        const char code[] = "void f ( ) { char str [ 100 ] = \"100\" ; }";
        ASSERT_EQUALS(code, tok(code));
    }

    void declareArray()
    {
        const char code[] = "void f ( ) { char str [ ] = \"100\" ; }";
        const char expected[] = "void f ( ) { char * str ; str = \"100\" ; }";
        ASSERT_EQUALS(expected, tok(code));
    }

    void dontRemoveIncrement()
    {
        {
            const char code[] = "void f(int a)\n"
                                "{\n"
                                "    if (a > 10)\n"
                                "        a = 5;\n"
                                "    else\n"
                                "        a = 10;\n"
                                "    a++;\n"
                                "}\n";
            ASSERT_EQUALS("void f ( int a ) { if ( a > 10 ) { a = 5 ; } else { a = 10 ; } a ++ ; }", tok(code));
        }

        {
            const char code[] = "void f(int a)\n"
                                "{\n"
                                "    if (a > 10)\n"
                                "        a = 5;\n"
                                "    else\n"
                                "        a = 10;\n"
                                "    ++a;\n"
                                "}\n";
            ASSERT_EQUALS("void f ( int a ) { if ( a > 10 ) { a = 5 ; } else { a = 10 ; } ++ a ; }", tok(code));
        }
    }

    void removePostIncrement()
    {
        const char code[] = "void f()\n"
                            "{\n"
                            "    unsigned int c = 0;\n"
                            "    c++;\n"
                            "    if (c>0) { c++; }\n"
                            "    c++;\n"
                            "}\n";
        ASSERT_EQUALS("void f ( ) { int c ; c = 3 ; ; { ; } ; }", tok(code));
    }


    void removePreIncrement()
    {
        const char code[] = "void f()\n"
                            "{\n"
                            "    unsigned int c = 0;\n"
                            "    ++c;\n"
                            "    if (c>0) { ++c; }\n"
                            "    ++c;\n"
                            "}\n";
        ASSERT_EQUALS("void f ( ) { int c ; c = 3 ; ; { ; } ; }", tok(code));
    }


    std::string elseif(const char code[])
    {
        std::istringstream istr(code);

        Tokenizer tokenizer;
        tokenizer.createTokens(istr);
        tokenizer.elseif();
        return tokenizer.tokens()->stringifyList(false);
    }

    void elseif1()
    {
        const char code[] = "else if(ab) { cd } else { ef }gh";
        ASSERT_EQUALS("\n\n##file 0\n1: else { if ( ab ) { cd } else { ef } } gh\n", elseif(code));
    }






    // Simplify 'sizeof'..
    std::string sizeof_(const char code[])
    {
        // tokenize..
        Tokenizer tokenizer;
        std::istringstream istr(code);
        tokenizer.tokenize(istr, "test.cpp");

        tokenizer.setVarId();
        tokenizer.simplifyTokenList();

        std::ostringstream ostr;
        for (const Token *tok = tokenizer.tokens(); tok; tok = tok->next())
            ostr << " " << tok->str();

        return ostr.str();
    }


    void sizeof1()
    {
        const char code1[] = " struct ABC *abc = malloc(sizeof(*abc)); ";
        const char code2[] = " struct ABC *abc = malloc(100); ";
        const char code3[] = " struct ABC *abc = malloc(sizeof *abc ); ";
        ASSERT_EQUALS(tok(code1), tok(code2));
        ASSERT_EQUALS(tok(code2), tok(code3));
    }


    void sizeof2()
    {
        const char code[] = "void foo()\n"
                            "{\n"
                            "    int i[4];\n"
                            "    sizeof(i);\n"
                            "    sizeof(*i);\n"
                            "}\n";
        ASSERT_EQUALS(" void foo ( ) { int i [ 4 ] ; 16 ; 4 ; }", sizeof_(code));
    }

    void sizeof3()
    {
        const char code[] = "static int i[4];\n"
                            "void f()\n"
                            "{\n"
                            "    int i[10];\n"
                            "    sizeof(i);\n"
                            "}\n";
        ASSERT_EQUALS(" static int i [ 4 ] ; void f ( ) { int i [ 10 ] ; 40 ; }", sizeof_(code));
    }

    void sizeof4()
    {
        const char code[] = "int i[10];\n"
                            "sizeof(i[0]);\n";
        ASSERT_EQUALS(" int i [ 10 ] ; 4 ;", sizeof_(code));
    }

    void sizeof5()
    {
        const char code[] =
            "for (int i = 0; i < sizeof(g_ReservedNames[0]); i++)"
            "{}";
        ASSERT_EQUALS(" for ( int i = 0 ; i < 100 ; i ++ ) { }", sizeof_(code));
    }

    void sizeof6()
    {
        const char code[] = ";int i;\n"
                            "sizeof(i);\n";

        std::ostringstream expected;
        expected << " ; int i ; " << sizeof(int) << " ;";

        ASSERT_EQUALS(expected.str(), sizeof_(code));
    }

    void sizeof7()
    {
        const char code[] = ";INT32 i[10];\n"
                            "sizeof(i[0]);\n";
        ASSERT_EQUALS(" ; INT32 i [ 10 ] ; sizeof ( i [ 0 ] ) ;", sizeof_(code));
    }

    void sizeof8()
    {
        {
            const char code[] = "void f()\n"
                                "{\n"
                                "  char* ptrs[2];\n"
                                "  int a = sizeof( ptrs );\n"
                                "}\n";
            std::ostringstream oss;
            oss << (sizeof(void *) * 2);
            ASSERT_EQUALS(" void f ( ) { char * ptrs [ 2 ] ; int a ; a = " + oss.str() + " ; }", sizeof_(code));
        }

        {
            const char code[] = "void f()\n"
                                "{\n"
                                "  char* ptrs[55];\n"
                                "  int a = sizeof( ptrs );\n"
                                "}\n";
            std::ostringstream oss;
            oss << (sizeof(void *) * 55);
            ASSERT_EQUALS(" void f ( ) { char * ptrs [ 55 ] ; int a ; a = " + oss.str() + " ; }", sizeof_(code));
        }


        {
            const char code[] = "void f()\n"
                                "{\n"
                                "  char* ptrs;\n"
                                "  int a = sizeof( ptrs );\n"
                                "}\n";
            std::ostringstream oss;
            oss << sizeof(void *);
            ASSERT_EQUALS(" void f ( ) { char * ptrs ; int a ; a = " + oss.str() + " ; }", sizeof_(code));
        }
    }

    void casting()
    {
        {
            const char code[] = "void f()\n"
                                "{\n"
                                "for (int i = 0; i < static_cast<int>(3); ++i) {}\n"
                                "}\n";

            const std::string expected(" void f ( ) { for ( int i = 0 ; i < 3 ; ++ i ) { } }");

            ASSERT_EQUALS(expected, sizeof_(code));
        }

        {
            const char code[] = "void f()\n"
                                "{\n"
                                "    p = const_cast<char *> qtu ();\n"
                                "}\n";

            const std::string expected(" void f ( ) { p = const_cast < char * > qtu ( ) ; }");

            ASSERT_EQUALS(expected, sizeof_(code));
        }
    }




    void template1()
    {
        const char code[] = "template <classname T> void f(T val) { T a; }\n"
                            "f<int>(10);";

        const std::string expected(" "
                                   "template < classname T > void f ( T val ) { T a ; } "
                                   "f<int> ( 10 ) ; "
                                   "void f<int> ( int val ) { int a ; }");

        ASSERT_EQUALS(expected, sizeof_(code));
    }

    void template2()
    {
        const char code[] = "template <classname T> class Fred { T a; };\n"
                            "Fred<int> fred;";

        const std::string expected(" "
                                   "template < classname T > class Fred { T a ; } ; "
                                   "Fred<int> fred ; "
                                   "class Fred<int> { int a ; }");

        ASSERT_EQUALS(expected, sizeof_(code));
    }

    void template3()
    {
        const char code[] = "template <classname T, int sz> class Fred { T data[sz]; };\n"
                            "Fred<float,4> fred;";

        const std::string expected(" "
                                   "template < classname T , int sz > class Fred { T data [ sz ] ; } ; "
                                   "Fred<float,4> fred ; "
                                   "class Fred<float,4> { float data [ 4 ] ; }");

        ASSERT_EQUALS(expected, sizeof_(code));
    }

    void template4()
    {
        const char code[] = "template <classname T> class Fred { Fred(); };\n"
                            "Fred<float> fred;";

        const std::string expected(" "
                                   "template < classname T > class Fred { Fred ( ) ; } ; "
                                   "Fred<float> fred ; "
                                   "class Fred<float> { Fred<float> ( ) ; }");

        ASSERT_EQUALS(expected, sizeof_(code));
    }

    void template5()
    {
        const char code[] = "template <classname T> class Fred { };\n"
                            "template <classname T> Fred<T>::Fred() { }\n"
                            "Fred<float> fred;";

        const std::string expected(" "
                                   "template < classname T > class Fred { } ; "
                                   "template < classname T > Fred < T > :: Fred ( ) { } "
                                   "Fred<float> fred ; "
                                   "class Fred<float> { } "
                                   "Fred<float> :: Fred<float> ( ) { }");

        ASSERT_EQUALS(expected, sizeof_(code));
    }

    void template6()
    {
        const char code[] = "template <classname T> class Fred { };\n"
                            "Fred<float> fred1;\n"
                            "Fred<float> fred2;";

        const std::string expected(" template < classname T > class Fred { } ;"
                                   " Fred<float> fred1 ;"
                                   " Fred<float> fred2 ;"
                                   " class Fred<float> { }");

        ASSERT_EQUALS(expected, sizeof_(code));
    }

    void template7()
    {
        // A template class that is not used => no simplification
        {
            const char code[] = "template <class T>\n"
                                "class ABC\n"
                                "{\n"
                                "public:\n"
                                "    typedef ABC<T> m;\n"
                                "\n"
                                "};\n";

            const std::string expected(" template < class T > "
                                       "class ABC "
                                       "{ "
                                       "public: "
                                       "typedef ABC < T > m ; "
                                       "} ;");

            ASSERT_EQUALS(expected, sizeof_(code));
        }

        {
            const char code[] = "template <typename T> class ABC {\n"
                                "public:\n"
                                "    typedef std::vector<T> type;\n"
                                "};\n"
                                "int main() {\n"
                                "    ABC<int>::type v;\n"
                                "    v.push_back(4);\n"
                                "    return 0;\n"
                                "}\n";

            const std::string expected(" template < typename T > class ABC {"
                                       " public: "
                                       "typedef std :: vector < T > type ; "
                                       "} ; "
                                       "int main ( ) { "
                                       "std :: vector < int > v ; "
                                       "v . push_back ( 4 ) ; "
                                       "return 0 ; "
                                       "}");

            TODO_ASSERT_EQUALS(expected, sizeof_(code));
        }

        {
            const char code[] = "template <typename T> class ABC {\n"
                                "public:\n"
                                "    typedef std::vector<T> type;\n"
                                "    void f()\n"
                                "    {\n"
                                "      ABC<int>::type v;\n"
                                "      v.push_back(4);\n"
                                "    }\n"
                                "};\n";

            const std::string expected(" template < typename T > class ABC "
                                       "{"
                                       " public: typedef std :: vector < T > type ;"
                                       " void f ( ) "
                                       "{ "
                                       "std :: vector < int > v ;"
                                       " v . push_back ( 4 ) ;"
                                       " } "
                                       "} ;");

            TODO_ASSERT_EQUALS(expected, sizeof_(code));
        }
    }

    // Template definitions but no usage => no expansion
    void template8()
    {
        const char code[] = "template<typename T> class A;\n"
                            "template<typename T> class B;\n"
                            "\n"
                            "typedef A<int> x;\n"
                            "typedef B<int> y;\n"
                            "\n"
                            "template<typename T> class A {\n"
                            "    void f() {\n"
                            "        B<T> a = B<T>::g();\n"
                            "        T b = 0;\n"
                            "        if (b)\n"
                            "            b = 0;\n"
                            "    }\n"
                            "};\n"
                            "\n"
                            "template<typename T> inline B<T> h() { return B<T>(); }\n";

        const std::string expected(" template < typename T > class A ;"
                                   " template < typename T > class B ;"
                                   ""
                                   " typedef A < int > x ;"
                                   " typedef B < int > y ;"
                                   ""
                                   " template < typename T > class A {"
                                   " void f ( ) {"
                                   " B < T > a = B < T > :: g ( ) ;"
                                   " T b ; b = 0 ;"
                                   " }"
                                   " } ;"
                                   ""
                                   " template < typename T > inline B < T > h ( ) { return B < T > ( ) ; }");

        ASSERT_EQUALS(expected, sizeof_(code));
    }

    void template9()
    {
        const char code[] = "template < typename T > class A { } ;\n"
                            "\n"
                            "void f ( ) {\n"
                            "    A<int> a ;\n"
                            "}\n"
                            "\n"
                            "template < typename T >\n"
                            "class B {\n"
                            "    void g ( ) {\n"
                            "        A < T > b = A < T > :: h ( ) ;\n"
                            "    }\n"
                            "} ;\n";

        // The expected result..
        std::string expected(std::string(" ") + code + " class A<int> { }");
        std::string::size_type pos;
        while ((pos = expected.find("\n")) != std::string::npos)
            expected[pos] = ' ';
        while ((pos = expected.find("  ")) != std::string::npos)
            expected.erase(pos, 1);

        ASSERT_EQUALS(expected, sizeof_(code));
    }

    void template10()
    {
        const char code[] = "template <int ui, typename T> T * foo()\n"
                            "{ return new T[ui]; }\n"
                            "\n"
                            "void f ( )\n"
                            "{\n"
                            "    foo<3,int>();\n"
                            "}\n";

        // The expected result..
        const std::string expected(" template < int ui , typename T > T * foo ( )"
                                   " { return new T [ ui ] ; }"
                                   " void f ( )"
                                   " {"
                                   " foo<3,int> ( ) ;"
                                   " }"
                                   " int * foo<3,int> ( ) { return new int [ 3 ] ; }");
        ASSERT_EQUALS(expected, sizeof_(code));
    }

    void template11()
    {
        const char code[] = "template <int ui, typename T> T * foo()\n"
                            "{ return new T[ui]; }\n"
                            "\n"
                            "void f ( )\n"
                            "{\n"
                            "    char * p = foo<3,char>();\n"
                            "}\n";

        // The expected result..
        const std::string expected(" template < int ui , typename T > T * foo ( )"
                                   " { return new T [ ui ] ; }"
                                   " void f ( )"
                                   " {"
                                   " char * p ; p = foo<3,char> ( ) ;"
                                   " }"
                                   " char * foo<3,char> ( ) { return new char [ 3 ] ; }");
        ASSERT_EQUALS(expected, sizeof_(code));
    }

    void template12()
    {
        const char code[] = "template <int x, int y, int z>\n"
                            "class A : public B<x, y, (x - y) ? ((y < z) ? 1 : -1) : 0>\n"
                            "{ };\n"
                            "\n"
                            "void f()\n"
                            "{\n"
                            "    A<12,12,11> a;\n"
                            "}\n";

        // The expected result..
        const std::string expected(" template < int x , int y , int z >"
                                   " class A : public B < x , y , ( x - y ) ? ( ( y < z ) ? 1 : - 1 ) : 0 >"
                                   " { } ;"
                                   " void f ( )"
                                   " {"
                                   " A<12,12,11> a ;"
                                   " }"
                                   " class A<12,12,11> : public B < 12 , 12 , 0 >"
                                   " { }");
        ASSERT_EQUALS(expected, sizeof_(code));
    }




    void namespaces()
    {
        {
            const char code[] = "using namespace std; namespace a{ namespace b{ void f(){} } }";

            const std::string expected(" using namespace std ; void f ( ) { }");

            ASSERT_EQUALS(expected, sizeof_(code));
        }

        {
            const char code[] = "namespace b{ void f(){} }";

            const std::string expected(" void f ( ) { }");

            ASSERT_EQUALS(expected, sizeof_(code));
        }

        {
            const char code[] = "int a; namespace b{ }";

            const std::string expected(" int a ;");

            ASSERT_EQUALS(expected, sizeof_(code));
        }
    }


    std::string simplifyIfAssign(const char code[])
    {
        // tokenize..
        Tokenizer tokenizer;
        std::istringstream istr(code);
        tokenizer.tokenize(istr, "test.cpp");

        tokenizer.simplifyIfAssign();

        std::ostringstream ostr;
        for (const Token *tok = tokenizer.tokens(); tok; tok = tok->next())
            ostr << (tok->previous() ? " " : "") << tok->str();

        return ostr.str();
    }

    void ifassign1()
    {
        ASSERT_EQUALS("; a = b ; if ( a ) ;", simplifyIfAssign(";if(a=b);"));
        ASSERT_EQUALS("; a = b ( ) ; if ( ( a ) ) ;", simplifyIfAssign(";if((a=b()));"));
        ASSERT_EQUALS("; a = b ( ) ; if ( ! ( a ) ) ;", simplifyIfAssign(";if(!(a=b()));"));
        ASSERT_EQUALS("; a . x = b ( ) ; if ( ! ( a . x ) ) ;", simplifyIfAssign(";if(!(a->x=b()));"));
    }


    void whileAssign()
    {
        ASSERT_EQUALS("; a = b ; while ( a ) { b = 0 ; a = b ; }", simplifyIfAssign(";while(a=b) { b = 0; }"));
        ASSERT_EQUALS("; a . b = c ; while ( a . b ) { c = 0 ; a . b = c ; }", simplifyIfAssign(";while(a.b=c) { c=0; }"));
    }


    std::string simplifyIfNot(const char code[])
    {
        // tokenize..
        Tokenizer tokenizer;
        std::istringstream istr(code);
        tokenizer.tokenize(istr, "test.cpp");

        tokenizer.simplifyIfNot();

        std::ostringstream ostr;
        for (const Token *tok = tokenizer.tokens(); tok; tok = tok->next())
            ostr << (tok->previous() ? " " : "") << tok->str();

        return ostr.str();
    }

    void ifnot()
    {
        ASSERT_EQUALS("if ( ! x )", simplifyIfNot("if(0==x)"));
        ASSERT_EQUALS("if ( ! x )", simplifyIfNot("if(x==0)"));
        ASSERT_EQUALS("if ( ! ( a = b ) )", simplifyIfNot("if(0==(a=b))"));
        ASSERT_EQUALS("if ( ! x )", simplifyIfNot("if(x==0)"));
        ASSERT_EQUALS("if ( ! a && b ( ) )", simplifyIfNot("if( 0 == a && b() )"));
        ASSERT_EQUALS("if ( b ( ) && ! a )", simplifyIfNot("if( b() && 0 == a )"));
        ASSERT_EQUALS("if ( ! ( a = b ) )", simplifyIfNot("if((a=b)==0)"));
        ASSERT_EQUALS("if ( ! x . y )", simplifyIfNot("if(x.y==0)"));
        ASSERT_EQUALS("if ( ( ! x ) )", simplifyIfNot("if((x==0))"));
        ASSERT_EQUALS("if ( ( ! x ) && ! y )", simplifyIfNot("if((x==0) && y==0)"));
    }



    std::string simplifyNot(const char code[])
    {
        // tokenize..
        Tokenizer tokenizer;
        std::istringstream istr(code);
        tokenizer.tokenize(istr, "test.cpp");

        tokenizer.simplifyNot();

        std::ostringstream ostr;
        for (const Token *tok = tokenizer.tokens(); tok; tok = tok->next())
            ostr << (tok->previous() ? " " : "") << tok->str();

        return ostr.str();
    }

    void not1()
    {
        ASSERT_EQUALS("if ( ! p )", simplifyNot("if (not p)"));
        ASSERT_EQUALS("if ( p && ! q )", simplifyNot("if (p && not q)"));
        ASSERT_EQUALS("void foo ( not i )", simplifyNot("void foo ( not i )"));
    }

    void comma_keyword()
    {
        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    char *a, *b;\n"
                                "    delete a, delete b;\n"
                                "}\n";
            ASSERT_EQUALS(" void foo ( ) { char * a ; char * b ; delete a ; delete b ; }", sizeof_(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    struct A *a, *b;\n"
                                "}\n";
            ASSERT_EQUALS(" void foo ( ) { struct A * a ; struct A * b ; }", sizeof_(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    struct A **a, **b;\n"
                                "}\n";
            ASSERT_EQUALS(" void foo ( ) { struct A * * a ; struct A * * b ; }", sizeof_(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    char *a, *b;\n"
                                "    delete a, b;\n"
                                "}\n";
            ASSERT_EQUALS(" void foo ( ) { char * a ; char * b ; delete a ; delete b ; }", sizeof_(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    char **a, **b, **c;\n"
                                "}\n";
            ASSERT_EQUALS(" void foo ( ) { char * * a ; char * * b ; char * * c ; }", sizeof_(code));
        }

        {
            const char code[] = "int f()\n"
                                "{\n"
                                "    if (something)\n"
                                "        return a(2, c(3, 4)), b(3), 10;\n"
                                "    return a(), b(0, 0, 0), 10;\n"
                                "}\n";
            ASSERT_EQUALS(" int f ( )"
                          " {"
                          " if ( something )"
                          " {"
                          " a ( 2 , c ( 3 , 4 ) ) ;"
                          " b ( 3 ) ;"
                          " return 10 ;"
                          " }"
                          " a ( ) ;"
                          " b ( 0 , 0 , 0 ) ;"
                          " return 10 ; "
                          "}", sizeof_(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    delete [] a, a = 0;\n"
                                "}\n";
            ASSERT_EQUALS(" void foo ( ) { delete [ ] a ; a = 0 ; }", sizeof_(code));
        }

        {
            const char code[] = "void foo()\n"
                                "{\n"
                                "    delete a, a = 0;\n"
                                "}\n";
            ASSERT_EQUALS(" void foo ( ) { delete a ; a = 0 ; }", sizeof_(code));
        }

        {
            const char code[] = "void f()\n"
                                "{\n"
                                "    for(int a,b; a < 10; a = a + 1, b = b + 1);\n"
                                "}\n";
            ASSERT_EQUALS(" void f ( ) { for ( int a , b ; a < 10 ; a = a + 1 , b = b + 1 ) { ; } }", sizeof_(code));
        }

        {
            const char code[] = "void f()\n"
                                "{\n"
                                "    char buf[BUFSIZ], **p;\n"
                                "    char *ptrs[BUFSIZ], **pp;\n"
                                "}\n";
            ASSERT_EQUALS(" void f ( ) { char buf [ BUFSIZ ] ; char * * p ; char * ptrs [ BUFSIZ ] ; char * * pp ; }", sizeof_(code));
        }
    }

    void remove_comma()
    {
        {
            const char code[] = "void f()\n"
                                "{\n"
                                "  int a,b;\n"
                                "  if( a )\n"
                                "  a=0,\n"
                                "  b=0;\n"
                                "}\n";
            ASSERT_EQUALS(" void f ( ) { int a ; int b ; if ( a ) { a = 0 ; b = 0 ; } }", sizeof_(code));
        }

        {
            const char code[] = "void f()\n"
                                "{\n"
                                "  A a,b;\n"
                                "  if( a.f )\n"
                                "  a.f=b.f,\n"
                                "  a.g=b.g;\n"
                                "}\n";
            ASSERT_EQUALS(" void f ( ) { A a ; A b ; if ( a . f ) { a . f = b . f ; a . g = b . g ; } }", sizeof_(code));
        }
    }

    void conditionOperator()
    {
        {
            const char code[] = "; x = a ? b : c;";
            ASSERT_EQUALS("; if ( a ) { x = b ; } else { x = c ; }", tok(code));
        }

        {
            const char code[] = "(0?(false?1:2):3)";
            ASSERT_EQUALS("( 3 )", tok(code));
        }

        {
            const char code[] = "(1?(false?1:2):3)";
            ASSERT_EQUALS("( 2 )", tok(code));
        }

        {
            const char code[] = "( true ? a ( ) : b ( ) )";
            ASSERT_EQUALS(code, tok(code));
            TODO_ASSERT_EQUALS("( a ( ) )", tok(code));
        }

        {
            const char code[] = "( true ? abc . a : abc . b )";
            ASSERT_EQUALS(code, tok(code));
            TODO_ASSERT_EQUALS("( abc . a )", tok(code));
        }
    }

    void calculations()
    {
        {
            const char code[] = "a[i+8+2]";
            ASSERT_EQUALS("a [ i + 10 ]", tok(code));
        }
        {
            const char code[] = "a[8+2+i]";
            ASSERT_EQUALS("a [ 10 + i ]", tok(code));
        }
        {
            const char code[] = "a[i + 2 * (2 * 4)]";
            ASSERT_EQUALS("a [ i + 16 ]", tok(code));
        }
        {
            const char code[] = "a[i + 100 - 90]";
            ASSERT_EQUALS("a [ i + 10 ]", tok(code));
        }
        {
            const char code[] = "a[1+1+1+1+1+1+1+1+1+1-2+5-3]";
            ASSERT_EQUALS("a [ 10 ]", tok(code));
        }
        {
            const char code[] = "a[10+10-10-10]";
            ASSERT_EQUALS("a [ 0 ]", tok(code));
        }
    }

};

REGISTER_TEST(TestSimplifyTokens)
