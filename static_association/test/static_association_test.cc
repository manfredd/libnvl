/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "static_association_test.h"
#include "static_association/static_association.h"
#include "nicta_common/nicta_exception.h"
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(StaticAssociationTest);



void StaticAssociationTest :: setUp()
{
}


void StaticAssociationTest :: tearDown()
{
}


void StaticAssociationTest :: staticAssociation()
{
    nicta::association<int, int> AssocArray[] = { { 1, 6 },
                                                  { 2, 5 },
                                                  { 3, 9 } };

    nicta::static_association<int, int> myMap(AssocArray, sizeof(AssocArray) / sizeof(AssocArray[0]));

    CPPUNIT_ASSERT_EQUAL(myMap.size(), static_cast<size_t> (3));

    CPPUNIT_ASSERT(myMap.has_key(1) == true);
    CPPUNIT_ASSERT(myMap.has_key(2) == true);
    CPPUNIT_ASSERT(myMap.has_key(3) == true);
    CPPUNIT_ASSERT(myMap.has_key(4) == false);

    CPPUNIT_ASSERT_EQUAL(myMap(1), 6);
    CPPUNIT_ASSERT_EQUAL(myMap(2), 5);
    CPPUNIT_ASSERT_EQUAL(myMap(3), 9);

    CPPUNIT_ASSERT_THROW(myMap(4), nicta::nicta_exception);
}

