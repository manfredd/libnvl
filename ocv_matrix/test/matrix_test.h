/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef MATRIX_TEST_H
#define MATRIX_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class MatrixTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(MatrixTest);
    CPPUNIT_TEST(extents);
    CPPUNIT_TEST(getSet);
    CPPUNIT_TEST(dataInit);
    CPPUNIT_TEST(comparitors);
    CPPUNIT_TEST(assignment);
    CPPUNIT_TEST(rows_cols);
    CPPUNIT_TEST(mtx_multiplication);
    CPPUNIT_TEST(determinant);
    CPPUNIT_TEST(trace);
    CPPUNIT_TEST(diagonal);
    CPPUNIT_TEST(transpose);
    CPPUNIT_TEST(ata);
    CPPUNIT_TEST(aat);
    CPPUNIT_TEST(inverse);
    CPPUNIT_TEST(svd);
    CPPUNIT_TEST(solve);
    CPPUNIT_TEST(reshape);
    CPPUNIT_TEST(reshapeND);
    CPPUNIT_TEST(ew_plus);
    CPPUNIT_TEST(ew_minus);
    CPPUNIT_TEST(ew_multiply);
    CPPUNIT_TEST(ew_divide);
    CPPUNIT_TEST(ew_reciprocal);
    CPPUNIT_TEST(abs);
    CPPUNIT_TEST(pow);
    CPPUNIT_TEST(exp);
    CPPUNIT_TEST(log);
    CPPUNIT_TEST(cast);
    CPPUNIT_TEST_SUITE_END();

public:

    virtual void setUp();
    virtual void tearDown();

protected:

    void extents();
    void getSet();
    void dataInit();
    void comparitors();
    void assignment();
    void rows_cols();
    void mtx_multiplication();
    void determinant();
    void trace();
    void diagonal();
    void transpose();
    void ata();
    void aat();
    void inverse();
    void svd();
    void solve();
    void reshape();
    void reshapeND();
    void ew_plus();
    void ew_minus();
    void ew_multiply();
    void ew_divide();
    void ew_reciprocal();
    void abs();
    void pow();
    void exp();
    void log();
    void cast();
};

#endif
