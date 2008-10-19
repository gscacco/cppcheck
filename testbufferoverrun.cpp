
#include "tokenize.h"
#include "CommonCheck.h"
#include "CheckBufferOverrun.h"
#include "testsuite.h"

#include <sstream>

extern std::ostringstream errout;
extern bool ShowAll;

class TestBufferOverrun : public TestFixture
{
public:
    TestBufferOverrun() : TestFixture("TestBufferOverrun")
    { }

private:
    void check( const char code[] )
    {
        // Tokenize..
        tokens = tokens_back = NULL;
        std::istringstream istr(code);
        TokenizeCode( istr );
        SimplifyTokenList();

        // Fill function list
        FillFunctionList(0);

        // Clear the error buffer..
        errout.str("");

        // Check for memory leaks..
        ShowAll = true;
        CheckBufferOverrun();
    }

    void run()
    {
        TEST_CASE( noerr1 );
        TEST_CASE( noerr2 );
        TEST_CASE( noerr3 );

        TEST_CASE( array_index_1 );
        TEST_CASE( array_index_2 );
        TEST_CASE( array_index_3 );
        TEST_CASE( array_index_4 );
        TEST_CASE( array_index_5 );
        TEST_CASE( array_index_6 );
        TEST_CASE( array_index_7 );
        TEST_CASE( array_index_8 );
        TEST_CASE( array_index_9 );
        TEST_CASE( array_index_10 );
        TEST_CASE( array_index_11 );
        //TEST_CASE( array_index_12 );

        TEST_CASE( buffer_overrun_1 );
        TEST_CASE( buffer_overrun_2 );
    }



    void noerr1()
    {
        check( "void f()\n"
               "{\n"
               "    if (ab)\n"
               "    {\n"
               "        char str[50];\n"
               "    }\n"
               "    if (ab)\n"
               "    {\n"
               "        char str[50];\n"
               "    }\n"
               "}\n" );
        ASSERT_EQUALS( std::string(""), errout.str() );
    }


    void noerr2()
    {
        check( "void f1(char *str)\n"
               "{\n"
               "    strcpy(buf,str);\n"
               "}\n"
               "void f2(char *str)\n"
               "{\n"
               "    strcat(buf,str);\n"
               "}\n"
               "void f3(char *str)\n"
               "{\n"
               "    sprintf(buf,\"%s\",str);\n"
               "}\n"
               "void f4(const char str[])\n"
               "{\n"
               "    strcpy(buf, str);\n"
               "}\n" );
        ASSERT_EQUALS( std::string(""), errout.str() );
    }


    void noerr3()
    {
        check( "static void f()\n"
               "{\n"
               "    char data[1];\n"
               "    return abc.data[1];\n"
               "}\n" );
        ASSERT_EQUALS( std::string(""), errout.str() );
    }


    void array_index_1()
    {
        check("void f()\n"
               "{\n"
               "    char str[0x10];\n"
               "    str[15] = 0;\n"
               "    str[16] = 0;\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:5]: Array index out of bounds\n"), errout.str() );
    }


    void array_index_2()
    {
        check("void f()\n"
               "{\n"
               "    char *str = new char[0x10];\n"
               "    str[15] = 0;\n"
               "    str[16] = 0;\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:5]: Array index out of bounds\n"), errout.str() );
    }


    void array_index_3()
    {
        check( "void f()\n"
               "{\n"
               "    int val[50];\n"
               "    for (i = 0; i < 100; i++)\n"
               "        sum += val[i];\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:5]: Buffer overrun\n"), errout.str() );
    }


    void array_index_4()
    {
        check( "const int SIZE = 10;\n"
               "void f()\n"
               "{\n"
               "    int i[SIZE];\n"
               "    i[SIZE] = 0;\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:5]: Array index out of bounds\n"), errout.str() );
    }


    void array_index_5()
    {
        check( "void f()\n"
               "{\n"
               "    int i[10];\n"
               "    i[ sizeof(i) - 1 ] = 0;\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:4]: Array index out of bounds\n"), errout.str() );
    }


    void array_index_6()
    {
        check( "struct ABC\n"
               "{\n"
               "    char str[10];\n"
               "};\n"
               "\n"
               "static void f()\n"
               "{\n"
               "    struct ABC abc;\n"
               "    abc.str[10] = 0;\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:9]: Array index out of bounds\n"), errout.str() );
    }


    void array_index_7()
    {
        check( "struct ABC\n"
               "{\n"
               "    char str[10];\n"
               "};\n"
               "\n"
               "static void f(ABC *abc)\n"
               "{\n"
               "    abc->str[10] = 0;\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:8]: Array index out of bounds\n"), errout.str() );
    }


    void array_index_8()
    {
        check( "const int SIZE = 10;\n"
               "\n"
               "struct ABC\n"
               "{\n"
               "    char str[SIZE];\n"
               "};\n"
               "\n"
               "static void f()\n"
               "{\n"
               "    struct ABC abc;\n"
               "    abc.str[SIZE] = 0;\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:11]: Array index out of bounds\n"), errout.str() );
    }

    void array_index_9()
    {
        check( "static void memclr( char *data )\n"
               "{\n"
               "    data[10] = 0;\n"
               "}\n"
               "\n"
               "static void f()\n"
               "{\n"
               "    char str[5];\n"
               "    memclr( str );   // ERROR\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:9] -> [test.cpp:3]: Array index out of bounds\n"), errout.str() );
    }


    void array_index_10()
    {
        check( "struct ABC\n"
               "{\n"
               "    char str[10];\n"
               "};\n"
               "\n"
               "static void memclr( char *data )\n"
               "{\n"
               "    data[10] = 0;\n"
               "}\n"
               "\n"
               "static void f(ABC *abc)\n"
               "{\n"
               "    memclr(abc->str);\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:13] -> [test.cpp:8]: Array index out of bounds\n"), errout.str() );
    }


    void array_index_11()
    {
        check( "class ABC\n"
               "{\n"
               "public:\n"
               "    ABC();\n"
               "    char *str[10];\n"
               "    struct ABC *next;"
               "};\n"
               "\n"
               "static void f()\n"
               "{\n"
               "    for ( ABC *abc = abc1; abc; abc = abc->next )\n"
               "    {\n"
               "        abc->str[10] = 0;\n"
               "    }\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:12]: Array index out of bounds\n"), errout.str() );
    }


    void array_index_12()
    {
        check( "class Fred\n"
               "{\n"
               "private:\n"
               "    char str[10];\n"
               "public:\n"
               "    Fred();\n"
               "};\n"
               "Fred::Fred()\n"
               "{\n"
               "    str[10] = 0;\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:5]: Array index out of bounds\n"), errout.str() );
    }


    void buffer_overrun_1()
    {
        check( "void f()\n"
               "{\n"
               "    char str[3];\n"
               "    strcpy(str, \"abc\");\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:4]: Buffer overrun\n"), errout.str() );
    }


    void buffer_overrun_2()
    {
        check( "struct ABC\n"
               "{\n"
               "    char str[5];\n"
               "};\n"
               "\n"
               "static void f(ABC *abc)\n"
               "{\n"
               "    strcpy( abc->str, \"abcdef\" );\n"
               "}\n" );
        ASSERT_EQUALS( std::string("[test.cpp:8]: Buffer overrun\n"), errout.str() );
    }


};

REGISTER_TEST( TestBufferOverrun )

