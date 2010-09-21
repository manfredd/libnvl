/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "csv_test.h"
#include "nicta_common/csv.h"
#include <string>
#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION(CSVTest);


using namespace nicta;


void CSVTest :: setUp()
{
}


void CSVTest :: tearDown()
{
}


void CSVTest :: parseBoolean()
{
    std::vector<bool> vec;
    csv::csv<bool, std::vector> p(vec);
    bool ret = p.operator()<csv::boolean>("true,T,FaLsE,false,0,1,TRUE");                        // alt:  p.operator()<boolean, char_>(..)

    CPPUNIT_ASSERT(ret == true);
    CPPUNIT_ASSERT(vec.size() == 7);
    CPPUNIT_ASSERT(vec[0] == true);
    CPPUNIT_ASSERT(vec[1] == true);
    CPPUNIT_ASSERT(vec[2] == false);
    CPPUNIT_ASSERT(vec[3] == false);
    CPPUNIT_ASSERT(vec[4] == false);
    CPPUNIT_ASSERT(vec[5] == true);
    CPPUNIT_ASSERT(vec[6] == true);

    bool e = false;
    bool result = csv::validate<csv::boolean>()("tRuE", e);
    CPPUNIT_ASSERT(result == true);
    CPPUNIT_ASSERT(e == true);


    std::vector<bool> vec1;
    csv::csv<bool, std::vector> p1(vec1);
    bool ret1 = p1.operator()<csv::boolean, csv::string_>("truexeTxeFaLsExefalsexexe0xe1xeTRUExeFxe", "xe");

    CPPUNIT_ASSERT(ret1 == true);
    CPPUNIT_ASSERT(vec1.size() == 8);
    CPPUNIT_ASSERT(vec1[0] == true);
    CPPUNIT_ASSERT(vec1[1] == true);
    CPPUNIT_ASSERT(vec1[2] == false);
    CPPUNIT_ASSERT(vec1[3] == false);
    CPPUNIT_ASSERT(vec1[4] == false);
    CPPUNIT_ASSERT(vec1[5] == true);
    CPPUNIT_ASSERT(vec1[6] == true);
    CPPUNIT_ASSERT(vec1[7] == false);
}


void CSVTest :: parseBinary()
{
    std::vector<double> vec;
    csv::csv<double, std::vector> p(vec);
    bool ret = p.operator()<csv::binary>("1001.101p3b,0010b,1b,0b,-100b");

    CPPUNIT_ASSERT(ret == true);
    CPPUNIT_ASSERT(vec.size() == 5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(77, vec[0], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2, vec[1], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, vec[2], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, vec[3], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4, vec[4], 1e-1);

    long double e = 0;
    bool result = csv::validate<csv::binary>()("-1001.101p3b", e);
    CPPUNIT_ASSERT(result == true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-77, static_cast<double> (e), 1e-1);


    std::vector<double> vec1;
    csv::csv<double, std::vector> p1(vec1);
    bool ret1 = p1.operator()<csv::binary, csv::string_>("xe1001.101p3bxe0010bxe1bxexe0bxe-100bxe", "xe");

    CPPUNIT_ASSERT(ret1 == true);
    CPPUNIT_ASSERT(vec1.size() == 5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(77, vec1[0], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2, vec1[1], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, vec1[2], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, vec1[3], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4, vec1[4], 1e-1);
}


void CSVTest :: parseOctal()
{
    std::vector<double> vec;
    csv::csv<double, std::vector> p(vec);
    bool ret = p.operator()<csv::octal>("027.033p2o,0041o,01,0o,-0100");

    CPPUNIT_ASSERT(ret == true);
    CPPUNIT_ASSERT(vec.size() == 5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(92.2109375, vec[0], 1e-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(33, vec[1], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, vec[2], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, vec[3], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-64, vec[4], 1e-1);

    long double e = 0;
    bool result = csv::validate<csv::octal>()("-1001.101p3o", e);
    CPPUNIT_ASSERT(result == true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4105.015625, static_cast<double> (e), 1e-7);


    std::vector<double> vec1;
    csv::csv<double, std::vector> p1(vec1);
    bool ret1 = p1.operator()<csv::octal, csv::string_>("xe027.033p2xe0010oxe1oxexe00xe-100oxe", "xe");

    CPPUNIT_ASSERT(ret1 == true);
    CPPUNIT_ASSERT(vec1.size() == 5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(92.2109375, vec1[0], 1e-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8, vec1[1], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, vec1[2], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, vec1[3], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-64, vec1[4], 1e-1);
}


void CSVTest :: parseHexadecimal()
{
    std::vector<double> vec;
    csv::csv<double, std::vector> p(vec);
    bool ret = p.operator()<csv::hexadecimal>("0x27A.C04p2,0x041f,01h,0x0,-0x100h");

    CPPUNIT_ASSERT(ret == true);
    CPPUNIT_ASSERT(vec.size() == 5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2539.00390625, vec[0], 1e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1055, vec[1], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, vec[2], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, vec[3], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-256, vec[4], 1e-1);

    long double e = 0;
    bool result = csv::validate<csv::hexadecimal>()("-1001.101p3h", e);
    CPPUNIT_ASSERT(result == true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-32776.501953125, static_cast<double> (e), 1e-10);


    std::vector<double> vec1;
    csv::csv<double, std::vector> p1(vec1);
    bool ret1 = p1.operator()<csv::hexadecimal, csv::string_>("xe0x27E.C04p2xe0x041fxe01hxexe0x0xe-0x349Chxe", "xe");

    CPPUNIT_ASSERT(ret1 == true);
    CPPUNIT_ASSERT(vec1.size() == 5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2555.00390625, vec1[0], 1e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1055, vec1[1], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, vec1[2], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, vec1[3], 1e-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-13468, vec1[4], 1e-1);
}


void CSVTest :: parseSignedInt()
{
    std::vector<int> vec;
    csv::csv<int, std::vector> p(vec);
    bool ret = p.operator()<csv::signed_int>("124,-23");                                         // alt:  p.operator()<signed_int, char_>(..)

    CPPUNIT_ASSERT(ret == true);
    CPPUNIT_ASSERT(vec.size() == 2);
    CPPUNIT_ASSERT_EQUAL(vec[0], 124);
    CPPUNIT_ASSERT_EQUAL(vec[1], -23);

    long long int e = 0;
    bool result = csv::validate<csv::signed_int>()("-1293", e);
    CPPUNIT_ASSERT(result == true);
    CPPUNIT_ASSERT(static_cast<int>(e) == -1293);


    std::vector<int> vec1;
    csv::csv<int, std::vector> p1(vec1);
    bool ret1 = p1.operator()<csv::signed_int, csv::string_>("12xe345xe-43xe3", "xe");

    CPPUNIT_ASSERT(ret1 == true);
    CPPUNIT_ASSERT(vec1.size() == 4);
    CPPUNIT_ASSERT_EQUAL(static_cast<int> (vec1[0]), 12);
    CPPUNIT_ASSERT_EQUAL(static_cast<int> (vec1[1]), 345);
    CPPUNIT_ASSERT_EQUAL(static_cast<int> (vec1[2]), -43);
    CPPUNIT_ASSERT_EQUAL(static_cast<int> (vec1[3]), 3);
}


void CSVTest :: parseUnsignedInt()
{
    std::vector<unsigned int> vec;
    csv::csv<unsigned int, std::vector> p(vec);
    bool ret = p.operator()<csv::unsigned_int>("124,23");                                        // alt:  p.operator()<unsigned_int, char_>(..)

    CPPUNIT_ASSERT(ret == true);
    CPPUNIT_ASSERT(vec.size() == 2);
    CPPUNIT_ASSERT_EQUAL(static_cast<int> (vec[0]), 124);
    CPPUNIT_ASSERT_EQUAL(static_cast<int> (vec[1]), 23);

    unsigned long long int e = 0;
    bool result = csv::validate<csv::unsigned_int>()("1293", e);
    CPPUNIT_ASSERT(result == true);
    CPPUNIT_ASSERT(static_cast<int>(e) == 1293);


    std::vector<unsigned int> vec1;
    csv::csv<unsigned int, std::vector> p1(vec1);
    bool ret1 = p1.operator()<csv::unsigned_int, csv::string_>("12xe345xe43xe3", "xe");

    CPPUNIT_ASSERT(ret1 == true);
    CPPUNIT_ASSERT(vec1.size() == 4);
    CPPUNIT_ASSERT_EQUAL(static_cast<int> (vec1[0]), 12);
    CPPUNIT_ASSERT_EQUAL(static_cast<int> (vec1[1]), 345);
    CPPUNIT_ASSERT_EQUAL(static_cast<int> (vec1[2]), 43);
    CPPUNIT_ASSERT_EQUAL(static_cast<int> (vec1[3]), 3);
}


void CSVTest :: parseNumeric()
{
    std::vector<double> vec;
    csv::csv<double, std::vector> p(vec);
    bool ret = p.operator()<csv::numeric>("+124,-23,849.3");                                     // alt:  p.operator()<numeric, char_>(..)

    CPPUNIT_ASSERT(ret == true);
    CPPUNIT_ASSERT(vec.size() == 3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124, vec[0], 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-23, vec[1], 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(849.3, vec[2], 1e-2);

    long double e = 0;
    bool result = csv::validate<csv::numeric>()("-12.3", e);
    CPPUNIT_ASSERT(result == true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-12.3, e, 1e-2);


    std::vector<double> vec1;
    csv::csv<double, std::vector> p1(vec1);
    bool ret1 = p1.operator()<csv::numeric, csv::string_>("12xe345xe-43xe3.2", "xe");

    CPPUNIT_ASSERT(ret1 == true);
    CPPUNIT_ASSERT(vec1.size() == 4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(12, vec1[0], 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(345, vec1[1], 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-43, vec1[2], 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.2, vec1[3], 1e-2);
}


void CSVTest :: parseAlpha()
{
    std::vector<std::string> vec;
    csv::csv<std::string, std::vector> p(vec);
    bool ret = p.operator()<csv::alpha>("abc,de");                                               // alt:  p.operator()<alpha, char_>(..)

    CPPUNIT_ASSERT(ret == true);
    CPPUNIT_ASSERT(vec.size() == 2);
    CPPUNIT_ASSERT(vec[0] == "abc");
    CPPUNIT_ASSERT(vec[1] == "de");

    bool result = csv::validate<csv::alpha>()("abcbcd");
    CPPUNIT_ASSERT(result == true);


    std::vector<std::string> vec1;
    csv::csv<std::string, std::vector> p1(vec1);
    bool ret1 = p1.operator()<csv::alpha, csv::string_>("xeexxeaaxedfxe", "xe");

    CPPUNIT_ASSERT(ret1 == true);
    CPPUNIT_ASSERT(vec1.size() == 3);
    CPPUNIT_ASSERT(vec1[0] == "ex");
    CPPUNIT_ASSERT(vec1[1] == "aa");
    CPPUNIT_ASSERT(vec1[2] == "df");
}


void CSVTest :: parseAlphaNum()
{
    std::vector<std::string> vec;
    csv::csv<std::string, std::vector> p(vec);
    bool ret = p.operator()<csv::alphanum>("12a,adf");                                           // alt:  p.operator()<alphanum, char_>(..)

    CPPUNIT_ASSERT(ret == true);
    CPPUNIT_ASSERT(vec.size() == 2);
    CPPUNIT_ASSERT(vec[0] == "12a");
    CPPUNIT_ASSERT(vec[1] == "adf");

    bool result = csv::validate<csv::alphanum>()("ab2c5a1b6c2d");
    CPPUNIT_ASSERT(result == true);


    std::vector<std::string> vec1;
    csv::csv<std::string, std::vector> p1(vec1);
    bool ret1 = p1.operator()<csv::alphanum, csv::string_>("xe1ex2xeaaxedfxe", "xe");

    CPPUNIT_ASSERT(ret1 == true);
    CPPUNIT_ASSERT(vec1.size() == 3);
    CPPUNIT_ASSERT(vec1[0] == "1ex2");
    CPPUNIT_ASSERT(vec1[1] == "aa");
    CPPUNIT_ASSERT(vec1[2] == "df");
}


void CSVTest :: parseUsrDef()
{
    std::vector<std::string> vec;
    csv::csv<std::string, std::vector> p(vec);
    bool ret = p.operator()<csv::usrdef>("23ab,@rw+, m2e!._i/-", ", ", "0-9a-zA-Z@!./_+-");      // alt:  p.operator()<usrdef, char_>(..)

    CPPUNIT_ASSERT(ret == true);
    CPPUNIT_ASSERT(vec.size() == 3);
    CPPUNIT_ASSERT(vec[0] == "23ab");
    CPPUNIT_ASSERT(vec[1] == "@rw+");
    CPPUNIT_ASSERT(vec[2] == "m2e!._i/-");

    bool result = csv::validate<csv::usrdef>()("r3_23.24*.jpg/2+23_5-3", "0-9a-zA-Z.*/_+-");     // "0-9a-zA-Z.*/_+-":  allowed chars
    CPPUNIT_ASSERT(result == true);


    std::vector<std::string> vec1;
    csv::csv<std::string, std::vector> p1(vec1);
    bool ret1 = p1.operator()<csv::usrdef, csv::string_>("xe1ex2xea,axedfxe", "xe", ",xe12adf");

    CPPUNIT_ASSERT(ret1 == true);
    CPPUNIT_ASSERT(vec1.size() == 3);
    CPPUNIT_ASSERT(vec1[0] == "1ex2");
    CPPUNIT_ASSERT(vec1[1] == "a,a");
    CPPUNIT_ASSERT(vec1[2] == "df");
}


void CSVTest :: parseAnyStr()
{
    std::vector<std::string> vec;
    csv::csv<std::string, std::vector> p(vec);
    bool ret = p.operator()<csv::anystr>("23ab,@rxw+, m2e!._i/-", ", +x");                       // alt:  p.operator()<anystr, char_>(..)
    CPPUNIT_ASSERT(ret == true);

    CPPUNIT_ASSERT(vec.size() == 4);
    CPPUNIT_ASSERT(vec[0] == "23ab");
    CPPUNIT_ASSERT(vec[1] == "@r");
    CPPUNIT_ASSERT(vec[2] == "w");
    CPPUNIT_ASSERT(vec[3] == "m2e!._i/-");

    bool result = csv::validate<csv::anystr>()("r3_23.24*.jpg/2+23_5-3", "!xyz");                // { !, x, y, z } are not permissible chars
    CPPUNIT_ASSERT(result == true);


    std::vector<std::string> vec1;
    csv::csv<std::string, std::vector> p1(vec1);
    bool ret1 = p1.operator()<csv::anystr, csv::string_>("xe1ex2xea   axedfxe", "  ", 0);        // '0' - admit all printable chars

    CPPUNIT_ASSERT(ret1 == true);
    CPPUNIT_ASSERT(vec1.size() == 2);
    CPPUNIT_ASSERT(vec1[0] == "xe1ex2xea");
    CPPUNIT_ASSERT(vec1[1] == " axedfxe");
}
