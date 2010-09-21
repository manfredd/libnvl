/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef CSV_TEST_H
#define CSV_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class CSVTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CSVTest);
    CPPUNIT_TEST(parseBoolean);
    CPPUNIT_TEST(parseBinary);
    CPPUNIT_TEST(parseOctal);
    CPPUNIT_TEST(parseHexadecimal);
    CPPUNIT_TEST(parseSignedInt);
    CPPUNIT_TEST(parseUnsignedInt);
    CPPUNIT_TEST(parseNumeric);
    CPPUNIT_TEST(parseAlpha);
    CPPUNIT_TEST(parseAlphaNum);
    CPPUNIT_TEST(parseUsrDef);
    CPPUNIT_TEST(parseAnyStr);
    CPPUNIT_TEST_SUITE_END();

public:

    virtual void setUp();
    virtual void tearDown();

protected:

    void parseBoolean();
    void parseBinary();
    void parseOctal();
    void parseHexadecimal();
    void parseSignedInt();
    void parseUnsignedInt();
    void parseNumeric();
    void parseAlpha();
    void parseAlphaNum();
    void parseUsrDef();
    void parseAnyStr();
};

#endif
