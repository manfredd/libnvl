/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "matrix_test.h"

#include "ocv_matrix/ocv_matrix.h"

#include <nicta_common/quantity.h>

#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include <initializer_list>


CPPUNIT_TEST_SUITE_REGISTRATION(MatrixTest);


using namespace nicta;


void MatrixTest :: setUp()
{
}


void MatrixTest :: tearDown()
{
}


void MatrixTest :: extents()
{
    ocv::matrix::extents<3> e;
    e = 2, 7, 3;

    CPPUNIT_ASSERT(e.size() == 3);
    CPPUNIT_ASSERT(e[0] == 2);
    CPPUNIT_ASSERT(e[1] == 7);
    CPPUNIT_ASSERT(e[2] == 3);


    ocv::matrix::extents<2> ee;
    ee = 5;
    ee = 2;

    CPPUNIT_ASSERT(ee.size() == 2);
    CPPUNIT_ASSERT(ee[0] == 5);
    CPPUNIT_ASSERT(ee[1] == 2);


    ocv::matrix::extents<2>::value_type* p = ee();

    CPPUNIT_ASSERT(p[0] == 5);
    CPPUNIT_ASSERT(p[1] == 2);
}


void MatrixTest :: getSet()
{
    ocv::matrix::matrix<int, 1> M1(ocv::matrix::extents<1>(3));

    M1[0]() = 4;
    M1[1]() = 5;
    M1[2]() = -8;

    CPPUNIT_ASSERT(M1[0]() == 4);
    CPPUNIT_ASSERT(M1[1]() == 5);
    CPPUNIT_ASSERT(M1[2]() == -8);

    ocv::matrix::extents<1> M1_e;
    M1.extents(M1_e);

    CPPUNIT_ASSERT(M1_e[0] == 3);




    ocv::matrix::matrix<int, 1> M2(ocv::matrix::extents<1>(3), static_cast<int *> (0), ocv::matrix::Column);

    M2[0]() = 4;
    M2[1]() = 5;
    M2[2]() = -8;

    CPPUNIT_ASSERT(M2[0]() == 4);
    CPPUNIT_ASSERT(M2[1]() == 5);
    CPPUNIT_ASSERT(M2[2]() == -8);

    ocv::matrix::extents<1> M2_e;
    M2.extents(M2_e);

    CPPUNIT_ASSERT(M2_e[0] == 3);




    ocv::matrix::matrix<int, 2> m(ocv::matrix::extents<2>(2, 3));

    m[0][0]() = 2;  m[0][1]() = 5;  m[0][2]() = 7;
    m[1][0]() = 9;  m[1][1]() = 1;  m[1][2]() = 3;

    CPPUNIT_ASSERT(m[0][0]() == 2);
    CPPUNIT_ASSERT(m[0][1]() == 5);
    CPPUNIT_ASSERT(m[0][2]() == 7);
    CPPUNIT_ASSERT(m[1][0]() == 9);
    CPPUNIT_ASSERT(m[1][1]() == 1);
    CPPUNIT_ASSERT(m[1][2]() == 3);

    CPPUNIT_ASSERT(m.extent(0) == 2);
    CPPUNIT_ASSERT(m.extent(1) == 3);

    ocv::matrix::extents<2> e;
    m.extents(e);

    CPPUNIT_ASSERT(e[0] == 2);
    CPPUNIT_ASSERT(e[1] == 3);




    ocv::matrix::matrix<int, 2, 3> m3(ocv::matrix::extents<2>(2, 3));
    m3[0][0]() = 412;  m3[0][1]() = 415;  m3[0][2]() = 417;
    m3[1][0]() = 419;  m3[1][1]() = 411;  m3[1][2]() = 413;

    m3[0][0](1) = -42;  m3[0][1](1) = -45;  m3[0][2](1) = -47;
    m3[1][0](1) = -49;  m3[1][1](1) = -41;  m3[1][2](1) = -43;

    m3[0][0](2) = -942;  m3[0][1](2) = -945;  m3[0][2](2) = -947;
    m3[1][0](2) = -949;  m3[1][1](2) = -941;  m3[1][2](2) = -943;

    CPPUNIT_ASSERT(m3[0][0](0) == 412);
    CPPUNIT_ASSERT(m3[0][1](0) == 415);
    CPPUNIT_ASSERT(m3[0][2](0) == 417);
    CPPUNIT_ASSERT(m3[1][0](0) == 419);
    CPPUNIT_ASSERT(m3[1][1](0) == 411);
    CPPUNIT_ASSERT(m3[1][2](0) == 413);

    CPPUNIT_ASSERT(m3[0][0](1) == -42);
    CPPUNIT_ASSERT(m3[0][1](1) == -45);
    CPPUNIT_ASSERT(m3[0][2](1) == -47);
    CPPUNIT_ASSERT(m3[1][0](1) == -49);
    CPPUNIT_ASSERT(m3[1][1](1) == -41);
    CPPUNIT_ASSERT(m3[1][2](1) == -43);

    CPPUNIT_ASSERT(m3[0][0](2) == -942);
    CPPUNIT_ASSERT(m3[0][1](2) == -945);
    CPPUNIT_ASSERT(m3[0][2](2) == -947);
    CPPUNIT_ASSERT(m3[1][0](2) == -949);
    CPPUNIT_ASSERT(m3[1][1](2) == -941);
    CPPUNIT_ASSERT(m3[1][2](2) == -943);

    CPPUNIT_ASSERT(m3.extent(0) == 2);
    CPPUNIT_ASSERT(m3.extent(1) == 3);

    ocv::matrix::extents<2> e3;
    m3.extents(e3);

    CPPUNIT_ASSERT(e3[0] == 2);
    CPPUNIT_ASSERT(e3[1] == 3);




    ocv::matrix::matrix<int, 2> nn(ocv::matrix::extents<2>(2, 3));

    nn(0, 0)() = 2;  nn(0, 1)() = 5;  nn(0, 2)() = 7;
    nn(1, 0)() = 9;  nn(1, 1)() = 1;  nn(1, 2)() = 3;

    CPPUNIT_ASSERT(nn(0, 0)() == 2);
    CPPUNIT_ASSERT(nn(0, 1)() == 5);
    CPPUNIT_ASSERT(nn(0, 2)() == 7);
    CPPUNIT_ASSERT(nn(1, 0)() == 9);
    CPPUNIT_ASSERT(nn(1, 1)() == 1);
    CPPUNIT_ASSERT(nn(1, 2)() == 3);

    CPPUNIT_ASSERT(nn.extent(0) == 2);
    CPPUNIT_ASSERT(nn.extent(1) == 3);

    ocv::matrix::extents<2> ex;
    nn.extents(ex);

    CPPUNIT_ASSERT(ex[0] == 2);
    CPPUNIT_ASSERT(ex[1] == 3);




    ocv::matrix::matrix<int, 3, 2> mm(ocv::matrix::extents<3>(2, 2, 2));

    mm(0, 0, 0)() = 1;  mm(0, 0, 1)() = 2;
    mm(0, 1, 0)() = 3;  mm(0, 1, 1)() = 4;

    mm(1, 0, 0)() = 5;  mm(1, 0, 1)() = 6;
    mm(1, 1, 0)() = 7;  mm(1, 1, 1)() = 8;


    mm(0, 0, 0)(1) = -1;  mm(0, 0, 1)(1) = -2;
    mm(0, 1, 0)(1) = -3;  mm(0, 1, 1)(1) = -4;

    mm(1, 0, 0)(1) = -5;  mm(1, 0, 1)(1) = -6;
    mm(1, 1, 0)(1) = -7;  mm(1, 1, 1)(1) = -8;


    CPPUNIT_ASSERT(mm(0, 0, 0)() == 1);
    CPPUNIT_ASSERT(mm(0, 0, 1)() == 2);
    CPPUNIT_ASSERT(mm(0, 1, 0)() == 3);
    CPPUNIT_ASSERT(mm(0, 1, 1)() == 4);
    CPPUNIT_ASSERT(mm(1, 0, 0)() == 5);
    CPPUNIT_ASSERT(mm(1, 0, 1)() == 6);
    CPPUNIT_ASSERT(mm(1, 1, 0)() == 7);
    CPPUNIT_ASSERT(mm(1, 1, 1)() == 8);
    CPPUNIT_ASSERT(mm(0, 0, 0)(1) == -1);
    CPPUNIT_ASSERT(mm(0, 0, 1)(1) == -2);
    CPPUNIT_ASSERT(mm(0, 1, 0)(1) == -3);
    CPPUNIT_ASSERT(mm(0, 1, 1)(1) == -4);
    CPPUNIT_ASSERT(mm(1, 0, 0)(1) == -5);
    CPPUNIT_ASSERT(mm(1, 0, 1)(1) == -6);
    CPPUNIT_ASSERT(mm(1, 1, 0)(1) == -7);
    CPPUNIT_ASSERT(mm(1, 1, 1)(1) == -8);

    CPPUNIT_ASSERT(mm.extent(0) == 2);
    CPPUNIT_ASSERT(mm.extent(1) == 2);
    CPPUNIT_ASSERT(mm.extent(2) == 2);

    ocv::matrix::extents<3> ee;
    mm.extents(ee);

    CPPUNIT_ASSERT(ee[0] == 2);
    CPPUNIT_ASSERT(ee[1] == 2);
    CPPUNIT_ASSERT(ee[1] == 2);
}


void MatrixTest :: dataInit()
{
    ocv::matrix::matrix<int, 2> v(ocv::matrix::extents<2>(2, 2), -39);

    CPPUNIT_ASSERT(v[0][0]() == -39);
    CPPUNIT_ASSERT(v[0][1]() == -39);
    CPPUNIT_ASSERT(v[1][0]() == -39);
    CPPUNIT_ASSERT(v[1][1]() == -39);


    int data[] = { 1, 2, 3, 4 };
    ocv::matrix::matrix<int, 2> m(ocv::matrix::extents<2>(2, 2), &data[0]);

    CPPUNIT_ASSERT(m[0][0]() == 1);
    CPPUNIT_ASSERT(m[0][1]() == 2);
    CPPUNIT_ASSERT(m[1][0]() == 3);
    CPPUNIT_ASSERT(m[1][1]() == 4);


    // using std::initializer_list..
    ocv::matrix::matrix<int, 2> m_il(ocv::matrix::extents<2>(2, 2), {7, 25, -3, 14});

    CPPUNIT_ASSERT(m_il[0][0]() == 7);
    CPPUNIT_ASSERT(m_il[0][1]() == 25);
    CPPUNIT_ASSERT(m_il[1][0]() == -3);
    CPPUNIT_ASSERT(m_il[1][1]() == 14);


    int* data_n = new int[4];
    data_n[0] = 1;
    data_n[1] = 2;
    data_n[2] = 3;
    data_n[3] = 4;
    ocv::matrix::matrix<int, 2> m_n(ocv::matrix::extents<2>(2, 2), data_n);

    CPPUNIT_ASSERT(m_n[0][0]() == 1);
    CPPUNIT_ASSERT(m_n[0][1]() == 2);
    CPPUNIT_ASSERT(m_n[1][0]() == 3);
    CPPUNIT_ASSERT(m_n[1][1]() == 4);

    delete[] data_n;


    int data_c[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    ocv::matrix::matrix<int, 2, 2> m_c(ocv::matrix::extents<2>(2, 2), &data_c[0]);

    CPPUNIT_ASSERT(m_c[0][0](0) == 1);     CPPUNIT_ASSERT(m_c[0][0](1) == 2);
    CPPUNIT_ASSERT(m_c[0][1](0) == 3);     CPPUNIT_ASSERT(m_c[0][1](1) == 4);
    CPPUNIT_ASSERT(m_c[1][0](0) == 5);     CPPUNIT_ASSERT(m_c[1][0](1) == 6);
    CPPUNIT_ASSERT(m_c[1][1](0) == 7);     CPPUNIT_ASSERT(m_c[1][1](1) == 8);


    data_c[5] = 4001;
    CPPUNIT_ASSERT(m_c[1][0](1) == 4001);

    m_c[0][0](1) = 8312;
    CPPUNIT_ASSERT(data_c[1] == 8312);
}


void MatrixTest :: comparitors()
{
    int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    ocv::matrix::matrix<int, 2, 2> m1(ocv::matrix::extents<2>(2, 2), &data1[0]);
    ocv::matrix::matrix<int, 2, 2> m2(ocv::matrix::extents<2>(2, 2), &data1[0]);
    ocv::matrix::matrix<int, 2, 2> m3(ocv::matrix::extents<2>(2, 1), &data1[0]);

    CPPUNIT_ASSERT(m1 == m2);
    CPPUNIT_ASSERT(m2 != m3);   // ..because shape is different!


    ocv::matrix::matrix<int, 2> m4(ocv::matrix::extents<2>(2, 4), &data1[0]);
    ocv::matrix::matrix<int, 2> m5 = m4;

    CPPUNIT_ASSERT(m4 == m5);



    // Comparison on multi-dim matrices done in assignment-test below
}


void MatrixTest :: assignment()
{
    int Data1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    ocv::matrix::matrix<int, 2> M1(ocv::matrix::extents<2>(2, 2), &Data1[0]);

    ocv::matrix::matrix<int, 2> M2(M1);

    CPPUNIT_ASSERT(M2.extent(0) == M1.extent(0));
    CPPUNIT_ASSERT(M2.extent(1) == M1.extent(1));
    CPPUNIT_ASSERT(M2 == M1);


    int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 21, 22, 23, 24, 25, 26, 27, 28 };
    ocv::matrix::matrix<int, 3, 2> m1(ocv::matrix::extents<3>(2, 2, 2), &data1[0]);

    ocv::matrix::matrix<int, 3, 2> m2(ocv::matrix::extents<3>(1, 2, 2));
    m2 = m1;

    CPPUNIT_ASSERT(m1 == m2);

    // NOTE:  m2 is effectively reshaped!
    CPPUNIT_ASSERT(m2.extent(0) == 2);
    CPPUNIT_ASSERT(m2.extent(1) == 2);
    CPPUNIT_ASSERT(m2.extent(2) == 2);

    // NOTE:  a deep copy was made of m1 -> m2
    m1[1][1][0](1) = -2342;
    CPPUNIT_ASSERT(m2[1][1][0](1) != 2342);
    CPPUNIT_ASSERT(m1[1][1][0](1) == -2342);

    // Obviously, ..we've modified m1 since
    CPPUNIT_ASSERT(m1 != m2);
}


void MatrixTest :: rows_cols()
{
    ocv::matrix::matrix<int, 2> m(ocv::matrix::extents<2>(4, 3));

    m[0][0]() =  2;  m[0][1]() =  5;  m[0][2]() =  7;
    m[1][0]() =  9;  m[1][1]() =  1;  m[1][2]() =  3;
    m[2][0]() = -9;  m[2][1]() = -1;  m[2][2]() = -8;
    m[3][0]() = -2;  m[3][1]() = -5;  m[3][2]() = -7;


    CPPUNIT_ASSERT(m.rows() == 4);
    CPPUNIT_ASSERT(m.columns() == 3);


    boost::shared_ptr<ocv::matrix::matrix<int, 2> > r = m.rows(2);

    CPPUNIT_ASSERT(r->is_view() == true);
    CPPUNIT_ASSERT(r->extent(0) == 1);
    CPPUNIT_ASSERT(r->extent(1) == 3);
    CPPUNIT_ASSERT((*r)[0][0]() == -9);
    CPPUNIT_ASSERT((*r)[0][1]() == -1);
    CPPUNIT_ASSERT((*r)[0][2]() == -8);


    boost::shared_ptr<ocv::matrix::matrix<int, 2> > c = m.columns(0);

    CPPUNIT_ASSERT(c->is_view() == true);
    CPPUNIT_ASSERT(c->extent(0) == 4);
    CPPUNIT_ASSERT(c->extent(1) == 1);
    CPPUNIT_ASSERT((*c)[0][0]() ==  2);
    CPPUNIT_ASSERT((*c)[1][0]() ==  9);
    CPPUNIT_ASSERT((*c)[2][0]() == -9);
    CPPUNIT_ASSERT((*c)[3][0]() == -2);


    boost::shared_ptr<ocv::matrix::matrix<int, 2> > R = m.rows(0, 2, nicta::quantity::step<unsigned int> (2));

    CPPUNIT_ASSERT(R->is_view() == true);
    CPPUNIT_ASSERT(R->extent(0) == 2);
    CPPUNIT_ASSERT(R->extent(1) == 3);
    CPPUNIT_ASSERT((*R)[0][0]() ==  2);
    CPPUNIT_ASSERT((*R)[0][1]() ==  5);
    CPPUNIT_ASSERT((*R)[0][2]() ==  7);
    CPPUNIT_ASSERT((*R)[1][0]() == -9);
    CPPUNIT_ASSERT((*R)[1][1]() == -1);
    CPPUNIT_ASSERT((*R)[1][2]() == -8);


    boost::shared_ptr<ocv::matrix::matrix<int, 2> > C = m.columns(1, 2);

    CPPUNIT_ASSERT(C->is_view() == true);
    CPPUNIT_ASSERT(C->extent(0) == 4);
    CPPUNIT_ASSERT(C->extent(1) == 2);
    CPPUNIT_ASSERT((*C)[0][0]() ==  5);
    CPPUNIT_ASSERT((*C)[0][1]() ==  7);
    CPPUNIT_ASSERT((*C)[1][0]() ==  1);
    CPPUNIT_ASSERT((*C)[1][1]() ==  3);
    CPPUNIT_ASSERT((*C)[2][0]() == -1);
    CPPUNIT_ASSERT((*C)[2][1]() == -8);
    CPPUNIT_ASSERT((*C)[3][0]() == -5);
    CPPUNIT_ASSERT((*C)[3][1]() == -7);
}


void MatrixTest :: mtx_multiplication()
{
    ocv::matrix::matrix<double, 2> mL(ocv::matrix::extents<2>(2, 3));
    mL[0][0]() = 3;  mL[0][1]() = -1;  mL[0][2]() = -2;
    mL[1][0]() = 2;  mL[1][1]() = -2;  mL[1][2]() = -1;


    ocv::matrix::matrix<double, 2> mR(ocv::matrix::extents<2>(3, 3));
    mR[0][0]() = 4;  mR[0][1]() = 2;  mR[0][2]() = 0;
    mR[1][0]() = 3;  mR[1][1]() = 0;  mR[1][2]() = 2;
    mR[2][0]() = 1;  mR[2][1]() = 1;  mR[2][2]() = 0;


    boost::shared_ptr<ocv::matrix::matrix<double, 2> > result = mL * mR;

    CPPUNIT_ASSERT(result->extent(0) == 2);
    CPPUNIT_ASSERT(result->extent(1) == 3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.0, (*result)[0][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.0, (*result)[0][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0, (*result)[0][2](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, (*result)[1][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.0, (*result)[1][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, (*result)[1][2](), 1e-6);

    CPPUNIT_ASSERT(result->is_view() == false);



    ocv::matrix::matrix<float, 2> m1(ocv::matrix::extents<2>(1, 2));
    m1[0][0]() = 3;  m1[0][1]() = -2;

    ocv::matrix::matrix<float, 2> m2(ocv::matrix::extents<2>(2, 1));
    m2[0][0]() = 4;
    m2[1][0]() = 5;

    boost::shared_ptr<ocv::matrix::matrix<float, 2> > r = m1 * m2;

    CPPUNIT_ASSERT(r->extent(0) == 1);
    CPPUNIT_ASSERT(r->extent(1) == 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, (*r)[0][0](), 1e-6);

    CPPUNIT_ASSERT(r->is_view() == false);
}


void MatrixTest :: trace()
{
    ocv::matrix::matrix<int, 2> m(ocv::matrix::extents<2>(4, 3));


    m[0][0]() =  2;  m[0][1]() =  5;  m[0][2]() =  7;

    m[1][0]() =  9;  m[1][1]() =  1;  m[1][2]() =  3;

    m[2][0]() = -9;  m[2][1]() = -1;  m[2][2]() = -8;

    m[3][0]() = -2;  m[3][1]() = -5;  m[3][2]() = -7;


    double tr = m.trace();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-5.0, tr, 1e-6);
}


void MatrixTest :: determinant()
{
    ocv::matrix::matrix<float, 2> m(ocv::matrix::extents<2>(2, 2));
    m[0][0]() = 3;  m[0][1]() = -2;
    m[1][0]() = 8;  m[1][1]() = 1;

    float det = m.determinant();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(19.0, det, 1e-6);
}


void MatrixTest :: diagonal()
{
    ocv::matrix::matrix<int, 2> m(ocv::matrix::extents<2>(4, 3));


    m[0][0]() =  2;  m[0][1]() =  5;  m[0][2]() =  7;

    m[1][0]() =  9;  m[1][1]() =  1;  m[1][2]() =  3;

    m[2][0]() = -9;  m[2][1]() = -1;  m[2][2]() = -3;

    m[3][0]() = -2;  m[3][1]() = -5;  m[3][2]() = -7;


    boost::shared_ptr<nicta::ocv::matrix::matrix<int, 2> > d = m.diagonal();


    CPPUNIT_ASSERT((*d)[0][0]() == 2);
    CPPUNIT_ASSERT((*d)[1][0]() == 1);
    CPPUNIT_ASSERT((*d)[2][0]() == -3);

    CPPUNIT_ASSERT(d->extent(0) == 3);
    CPPUNIT_ASSERT(d->extent(1) == 1);

    CPPUNIT_ASSERT(d->is_view() == true);
}


void MatrixTest :: transpose()
{
    ocv::matrix::matrix<int, 2> m(ocv::matrix::extents<2>(4, 3));


    m[0][0]() =  2;  m[0][1]() =  5;  m[0][2]() =  7;

    m[1][0]() =  9;  m[1][1]() =  1;  m[1][2]() =  3;

    m[2][0]() = -9;  m[2][1]() = -1;  m[2][2]() = -3;

    m[3][0]() = -2;  m[3][1]() = -5;  m[3][2]() = -7;


    // [3 x 4]:
    boost::shared_ptr<nicta::ocv::matrix::matrix<int, 2> > d = m.transpose();


    CPPUNIT_ASSERT((*d)[0][0]() == 2);
    CPPUNIT_ASSERT((*d)[0][1]() == 9);
    CPPUNIT_ASSERT((*d)[0][2]() == -9);
    CPPUNIT_ASSERT((*d)[0][3]() == -2);
    CPPUNIT_ASSERT((*d)[1][0]() == 5);
    CPPUNIT_ASSERT((*d)[1][1]() == 1);
    CPPUNIT_ASSERT((*d)[1][2]() == -1);
    CPPUNIT_ASSERT((*d)[1][3]() == -5);
    CPPUNIT_ASSERT((*d)[2][0]() == 7);
    CPPUNIT_ASSERT((*d)[2][1]() == 3);
    CPPUNIT_ASSERT((*d)[2][2]() == -3);
    CPPUNIT_ASSERT((*d)[2][3]() == -7);

    CPPUNIT_ASSERT(d->extent(0) == 3);
    CPPUNIT_ASSERT(d->extent(1) == 4);


    (*d)[1][3]() = 743;        // a deep copy is made, 'd' is **NOT** a view of 'm' !!

    CPPUNIT_ASSERT((*d)[1][3]() == 743);
    CPPUNIT_ASSERT(m[3][1]() != 743);
    CPPUNIT_ASSERT(m[3][1]() == -5);

    CPPUNIT_ASSERT(d->is_view() == false);
    CPPUNIT_ASSERT(m.is_view() == false);
}


void MatrixTest :: ata()
{
    ocv::matrix::matrix<float, 2> m(ocv::matrix::extents<2>(2, 2));

    m[0][0]() = 2;  m[0][1]() = 3;
    m[1][0]() = 5;  m[1][1]() = 7;


    boost::shared_ptr<ocv::matrix::matrix<float, 2> > r = m.ATA();

    CPPUNIT_ASSERT(r->extent(0) == 2);
    CPPUNIT_ASSERT(r->extent(1) == 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(29.0, (*r)[0][0](), 1e-6);    CPPUNIT_ASSERT_DOUBLES_EQUAL(41.0, (*r)[0][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(41.0, (*r)[1][0](), 1e-6);    CPPUNIT_ASSERT_DOUBLES_EQUAL(58.0, (*r)[1][1](), 1e-6);

    CPPUNIT_ASSERT( r->is_view() == false );
}


void MatrixTest :: aat()
{
    ocv::matrix::matrix<double, 2> m(ocv::matrix::extents<2>(2, 2));

    m[0][0]() = 2;  m[0][1]() = 5;
    m[1][0]() = 3;  m[1][1]() = 7;


    boost::shared_ptr<ocv::matrix::matrix<double, 2> > r = m.AAT();

    CPPUNIT_ASSERT(r->extent(0) == 2);
    CPPUNIT_ASSERT(r->extent(1) == 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13.0, (*r)[0][0](), 1e-6);    CPPUNIT_ASSERT_DOUBLES_EQUAL(31.0, (*r)[0][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(31.0, (*r)[1][0](), 1e-6);    CPPUNIT_ASSERT_DOUBLES_EQUAL(74.0, (*r)[1][1](), 1e-6);

    CPPUNIT_ASSERT( r->is_view() == false );
}


void MatrixTest :: inverse()
{
    ocv::matrix::matrix<double, 2> m(ocv::matrix::extents<2>(2, 2));

    m[0][0]() = 2;  m[0][1]() = 3;
    m[1][0]() = 4;  m[1][1]() = 5;

    boost::shared_ptr<ocv::matrix::matrix<double, 2> > r = m.inverse< >();

    CPPUNIT_ASSERT(r->extent(0) == 2);
    CPPUNIT_ASSERT(r->extent(1) == 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.5, (*r)[0][0](), 1e-6);    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, (*r)[0][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, (*r)[1][0](), 1e-6);    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, (*r)[1][1](), 1e-6);

    CPPUNIT_ASSERT( r->is_view() == false );


    // Will compute using pseudo-inverse representation:  Inv(M) = Inv(transpose(M) * M) * transpose(M)
    ocv::matrix::matrix<float, 2> pm(ocv::matrix::extents<2>(3, 2));

    pm[0][0]() =  2;  pm[0][1]() = 3;
    pm[1][0]() =  4;  pm[1][1]() = 5;
    pm[2][0]() = -3;  pm[2][1]() = 1;

    boost::shared_ptr<ocv::matrix::matrix<float, 2> > pr = pm.inverse<nicta::ocv::matrix::SVD>();


    CPPUNIT_ASSERT(pr->extent(0) == pm.extent(1));
    CPPUNIT_ASSERT(pr->extent(1) == pm.extent(0));

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.00205761194229126, (*pr)[0][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0514403283596039, (*pr)[0][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.263374507427216, (*pr)[0][2](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0843621417880058, (*pr)[1][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.109053499996662, (*pr)[1][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.201646089553833, (*pr)[1][2](), 1e-6);
}


void MatrixTest :: svd()
{
    ocv::matrix::matrix<float, 2> m(ocv::matrix::extents<2>(2, 3));

    m[0][0]() = 2;  m[0][1]() = 5;  m[0][2]() = 7;
    m[1][0]() = 9;  m[1][1]() = 1;  m[1][2]() = 3;


    boost::shared_ptr< boost::tuples::tuple< boost::shared_ptr<ocv::matrix::matrix<float, 2> >
                                           , boost::shared_ptr<ocv::matrix::matrix<float, 2> >
                                           , boost::shared_ptr<ocv::matrix::matrix<float, 2> >
                                           >
                     >
    result = m.svd< >();


    // W
    CPPUNIT_ASSERT(result->get<nicta::ocv::matrix::svd::W>()->extent(0) == m.extent(0));
    CPPUNIT_ASSERT(result->get<nicta::ocv::matrix::svd::W>()->extent(1) == m.extent(1));

    // U
    CPPUNIT_ASSERT(result->get<nicta::ocv::matrix::svd::U>()->extent(0) == m.extent(0));
    CPPUNIT_ASSERT(result->get<nicta::ocv::matrix::svd::U>()->extent(1) == m.extent(0));

    // V
    CPPUNIT_ASSERT(result->get<nicta::ocv::matrix::svd::V>()->extent(0) == m.extent(1));
    CPPUNIT_ASSERT(result->get<nicta::ocv::matrix::svd::V>()->extent(1) == m.extent(1));


    boost::shared_ptr<ocv::matrix::matrix<float, 2> >& W = result->get<nicta::ocv::matrix::svd::W>();
    boost::shared_ptr<ocv::matrix::matrix<float, 2> >& U = result->get<nicta::ocv::matrix::svd::U>();
    boost::shared_ptr<ocv::matrix::matrix<float, 2> >& V = result->get<nicta::ocv::matrix::svd::V>();

    boost::shared_ptr<ocv::matrix::matrix<float, 2> > A = *((*U) * (*W)) * (*V->transpose());
    CPPUNIT_ASSERT(A->extent(0) == m.extent(0));
    CPPUNIT_ASSERT(A->extent(1) == m.extent(1));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(m[0][0](), (*A)[0][0](), 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m[0][1](), (*A)[0][1](), 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m[0][2](), (*A)[0][2](), 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m[1][0](), (*A)[1][0](), 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m[1][1](), (*A)[1][1](), 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m[1][2](), (*A)[1][2](), 1e-2);
}


void MatrixTest :: solve()
{
    ocv::matrix::matrix<double, 2> A(ocv::matrix::extents<2>(2, 2));
    A[0][0]() = 2;  A[0][1]() = 3;
    A[1][0]() = 4;  A[1][1]() = 5;

    ocv::matrix::matrix<double, 2> b(ocv::matrix::extents<2>(2, 1));
    b[0][0]() = 10;
    b[1][0]() = -7;


    boost::shared_ptr<nicta::ocv::matrix::matrix<double, 2> > x = A.solve_sys< >(b);

    CPPUNIT_ASSERT(x->extent(0) == b.extent(0));
    CPPUNIT_ASSERT(x->extent(1) == b.extent(1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-35.5, (*x)[0][0](), 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 27.0, (*x)[1][0](), 1e-2);


    boost::shared_ptr<ocv::matrix::matrix<double, 2> > B = A * (*x);

    CPPUNIT_ASSERT(B->extent(0) == b.extent(0));
    CPPUNIT_ASSERT(B->extent(1) == b.extent(1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[0][0](), (*B)[0][0](), 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[1][0](), (*B)[1][0](), 1e-2);
}


void MatrixTest :: reshape()
{
    ocv::matrix::matrix<int, 2> M(ocv::matrix::extents<2>(2, 3));

    M[0][0]() = 2;  M[0][1]() = 5;  M[0][2]() = 7;
    M[1][0]() = 9;  M[1][1]() = 1;  M[1][2]() = 3;


    ocv::matrix::extents<2> e(6, 1);
    boost::shared_ptr<ocv::matrix::matrix<int, 2> > R = M.reshape(e);

    CPPUNIT_ASSERT( M[0][0]() == (*R)[0][0]() );   //  2
    CPPUNIT_ASSERT( M[0][1]() == (*R)[1][0]() );   //  5
    CPPUNIT_ASSERT( M[0][2]() == (*R)[2][0]() );   //  7
    CPPUNIT_ASSERT( M[1][0]() == (*R)[3][0]() );   //  9
    CPPUNIT_ASSERT( M[1][1]() == (*R)[4][0]() );   //  1
    CPPUNIT_ASSERT( M[1][2]() == (*R)[5][0]() );   //  3


    (*R)[4][0]() = 123;

    CPPUNIT_ASSERT( M[1][1]() == 123 );    // remember, R is a *VIEW* (not a mtx copy)!
    CPPUNIT_ASSERT( (*R)[4][0]() == 123 );

    CPPUNIT_ASSERT( R->is_view() == true );
    CPPUNIT_ASSERT( M.is_view() == false );



    ocv::matrix::matrix<int, 2> m(ocv::matrix::extents<2>(3, 1));
    m[0][0]() = 10;
    m[1][0]() = 20;
    m[2][0]() = 30;

    ocv::matrix::extents<2> e0(1, 3);
    boost::shared_ptr<nicta::ocv::matrix::matrix<int, 2> > s = m.reshape(e0);
    CPPUNIT_ASSERT( (*s)[0][0]() == 10 );
    CPPUNIT_ASSERT( (*s)[0][1]() == 20 );
    CPPUNIT_ASSERT( (*s)[0][2]() == 30 );
    CPPUNIT_ASSERT( s->extent(0) == 1 );
    CPPUNIT_ASSERT( s->extent(1) == 3 );

    ocv::matrix::extents<2> e1(3, 1);
    boost::shared_ptr<nicta::ocv::matrix::matrix<int, 2> > ss = s->reshape(e1);
    CPPUNIT_ASSERT( (*ss)[0][0]() == 10 );
    CPPUNIT_ASSERT( (*ss)[1][0]() == 20 );
    CPPUNIT_ASSERT( (*ss)[2][0]() == 30 );
    CPPUNIT_ASSERT( ss->extent(0) == 3 );
    CPPUNIT_ASSERT( ss->extent(1) == 1 );


    (*ss)[1][0]() = 77;                             // 'ss' is a View to 's', which in turn is a View to 'm'

    CPPUNIT_ASSERT( m[1][0]() == 77 );
    CPPUNIT_ASSERT( (*s)[0][1]() == 77 );

    CPPUNIT_ASSERT( s->is_view() == true );
    CPPUNIT_ASSERT( m.is_view() == false );
}


void MatrixTest :: reshapeND()
{
    ocv::matrix::matrix<int, 3> M(ocv::matrix::extents<3>(2,2,3));

    M[0][0][0]() = 2;  M[0][0][1]() = 5;  M[0][0][2]() = 7;
    M[0][1][0]() = 9;  M[0][1][1]() = 1;  M[0][1][2]() = 3;

    M[1][0][0]() = 12;  M[1][0][1]() = 15;  M[1][0][2]() = 17;
    M[1][1][0]() = 19;  M[1][1][1]() = 11;  M[1][1][2]() = 13;


    ocv::matrix::extents<3> e(3, 4, 1);
    boost::shared_ptr<ocv::matrix::matrix<int, 3> > R = M.reshape(e);

    CPPUNIT_ASSERT( M[0][0][0]() == (*R)[0][0][0]() );   //  2
    CPPUNIT_ASSERT( M[0][0][1]() == (*R)[0][1][0]() );   //  5
    CPPUNIT_ASSERT( M[0][0][2]() == (*R)[0][2][0]() );   //  7
    CPPUNIT_ASSERT( M[0][1][0]() == (*R)[0][3][0]() );   //  9
    CPPUNIT_ASSERT( M[0][1][1]() == (*R)[1][0][0]() );   //  1
    CPPUNIT_ASSERT( M[0][1][2]() == (*R)[1][1][0]() );   //  3
    CPPUNIT_ASSERT( M[1][0][0]() == (*R)[1][2][0]() );   // 12
    CPPUNIT_ASSERT( M[1][0][1]() == (*R)[1][3][0]() );   // 15
    CPPUNIT_ASSERT( M[1][0][2]() == (*R)[2][0][0]() );   // 17
    CPPUNIT_ASSERT( M[1][1][0]() == (*R)[2][1][0]() );   // 19
    CPPUNIT_ASSERT( M[1][1][1]() == (*R)[2][2][0]() );   // 11
    CPPUNIT_ASSERT( M[1][1][2]() == (*R)[2][3][0]() );   // 13


    (*R)[1][3][0]() = 123;

    CPPUNIT_ASSERT( M[1][0][1]() == 123 );    // remember, R is a *VIEW* (not a mtx copy)!

    CPPUNIT_ASSERT( R->is_view() == true );
    CPPUNIT_ASSERT( M.is_view() == false );
}


void MatrixTest :: ew_plus()
{
    ocv::matrix::matrix<int, 2> A(ocv::matrix::extents<2>(2, 2));
    A[0][0]() =  2;  A[0][1]() = 3;
    A[1][0]() = -4;  A[1][1]() = 5;

    boost::shared_ptr<ocv::matrix::matrix<int, 2> > r1 = A + 3;

    CPPUNIT_ASSERT(r1->extent(0) == 2);
    CPPUNIT_ASSERT(r1->extent(1) == 2);
    CPPUNIT_ASSERT((*r1)[0][0]() == 5);
    CPPUNIT_ASSERT((*r1)[0][1]() == 6);
    CPPUNIT_ASSERT((*r1)[1][0]() == -1);
    CPPUNIT_ASSERT((*r1)[1][1]() == 8);
    CPPUNIT_ASSERT( r1->is_view() == false );


    boost::shared_ptr<ocv::matrix::matrix<int, 2> > r2 = A + (*r1);

    CPPUNIT_ASSERT(r2->extent(0) == 2);
    CPPUNIT_ASSERT(r2->extent(1) == 2);
    CPPUNIT_ASSERT((*r2)[0][0]() == 7);
    CPPUNIT_ASSERT((*r2)[0][1]() == 9);
    CPPUNIT_ASSERT((*r2)[1][0]() == -5);
    CPPUNIT_ASSERT((*r2)[1][1]() == 13);
    CPPUNIT_ASSERT( r2->is_view() == false );
}


void MatrixTest :: ew_minus()
{
    ocv::matrix::matrix<int, 2> A(ocv::matrix::extents<2>(2, 2));
    A[0][0]() =  2;  A[0][1]() = 3;
    A[1][0]() = -4;  A[1][1]() = 5;

    boost::shared_ptr<ocv::matrix::matrix<int, 2> > r1 = A - 3;

    CPPUNIT_ASSERT(r1->extent(0) == 2);
    CPPUNIT_ASSERT(r1->extent(1) == 2);
    CPPUNIT_ASSERT((*r1)[0][0]() == -1);
    CPPUNIT_ASSERT((*r1)[0][1]() == 0);
    CPPUNIT_ASSERT((*r1)[1][0]() == -7);
    CPPUNIT_ASSERT((*r1)[1][1]() == 2);
    CPPUNIT_ASSERT( r1->is_view() == false );


    boost::shared_ptr<ocv::matrix::matrix<int, 2> > r2 = A - (*r1);

    CPPUNIT_ASSERT(r2->extent(0) == 2);
    CPPUNIT_ASSERT(r2->extent(1) == 2);
    CPPUNIT_ASSERT((*r2)[0][0]() == 3);
    CPPUNIT_ASSERT((*r2)[0][1]() == 3);
    CPPUNIT_ASSERT((*r2)[1][0]() == 3);
    CPPUNIT_ASSERT((*r2)[1][1]() == 3);
    CPPUNIT_ASSERT( r2->is_view() == false );
}


void MatrixTest :: ew_multiply()
{
    ocv::matrix::matrix<int, 2> A(ocv::matrix::extents<2>(2, 2));
    A[0][0]() =  2;  A[0][1]() = 3;
    A[1][0]() = -4;  A[1][1]() = 5;

    boost::shared_ptr<ocv::matrix::matrix<int, 2> > r1 = A * 3;

    CPPUNIT_ASSERT(r1->extent(0) == 2);
    CPPUNIT_ASSERT(r1->extent(1) == 2);
    CPPUNIT_ASSERT((*r1)[0][0]() == 6);
    CPPUNIT_ASSERT((*r1)[0][1]() == 9);
    CPPUNIT_ASSERT((*r1)[1][0]() == -12);
    CPPUNIT_ASSERT((*r1)[1][1]() == 15);
    CPPUNIT_ASSERT( r1->is_view() == false );


    boost::shared_ptr<ocv::matrix::matrix<int, 2> > r2 = A.multiply(*r1);

    CPPUNIT_ASSERT(r2->extent(0) == 2);
    CPPUNIT_ASSERT(r2->extent(1) == 2);
    CPPUNIT_ASSERT((*r2)[0][0]() == 12);
    CPPUNIT_ASSERT((*r2)[0][1]() == 27);
    CPPUNIT_ASSERT((*r2)[1][0]() == 48);
    CPPUNIT_ASSERT((*r2)[1][1]() == 75);
    CPPUNIT_ASSERT( r2->is_view() == false );
}


void MatrixTest :: ew_divide()
{
    ocv::matrix::matrix<float, 2> A(ocv::matrix::extents<2>(2, 2));
    A[0][0]() =  2;  A[0][1]() = 3;
    A[1][0]() = -4;  A[1][1]() = 8;

    boost::shared_ptr<ocv::matrix::matrix<float, 2> > r1 = A / 3;

    CPPUNIT_ASSERT(r1->extent(0) == 2);
    CPPUNIT_ASSERT(r1->extent(1) == 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.66666666, (*r1)[0][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, (*r1)[0][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.33333333, (*r1)[1][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.66666666, (*r1)[1][1](), 1e-6);
    CPPUNIT_ASSERT( r1->is_view() == false );


    boost::shared_ptr<ocv::matrix::matrix<float, 2> > r2 = A / *r1;

    CPPUNIT_ASSERT(r2->extent(0) == 2);
    CPPUNIT_ASSERT(r2->extent(1) == 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, (*r2)[0][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, (*r2)[0][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, (*r2)[1][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, (*r2)[1][1](), 1e-6);
    CPPUNIT_ASSERT( r2->is_view() == false );
}


void MatrixTest :: ew_reciprocal()
{
    ocv::matrix::matrix<float, 2> A(ocv::matrix::extents<2>(2, 2));
    A[0][0]() =  2;  A[0][1]() = 3;
    A[1][0]() = -4;  A[1][1]() = 8;

    boost::shared_ptr<ocv::matrix::matrix<float, 2> > r = nicta::ocv::matrix::reciprocal(A);

    CPPUNIT_ASSERT(r->extent(0) == 2);
    CPPUNIT_ASSERT(r->extent(1) == 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, (*r)[0][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.33333333, (*r)[0][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.25, (*r)[1][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.125, (*r)[1][1](), 1e-6);
    CPPUNIT_ASSERT( r->is_view() == false );
}


void MatrixTest :: abs()
{
    ocv::matrix::matrix<float, 2> A(ocv::matrix::extents<2>(2, 2));
    A[0][0]() =  2;  A[0][1]() = 3;
    A[1][0]() = -4;  A[1][1]() = -0.5;

    boost::shared_ptr<ocv::matrix::matrix<float, 2> > r = nicta::ocv::matrix::abs(A);

    CPPUNIT_ASSERT(r->extent(0) == 2);
    CPPUNIT_ASSERT(r->extent(1) == 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, (*r)[0][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, (*r)[0][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, (*r)[1][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, (*r)[1][1](), 1e-6);
    CPPUNIT_ASSERT( r->is_view() == false );
}


void MatrixTest :: pow()
{
    ocv::matrix::matrix<float, 2> A(ocv::matrix::extents<2>(2, 2));
    A[0][0]() =  2;  A[0][1]() = 3;
    A[1][0]() = -4;  A[1][1]() = 0.5;


    boost::shared_ptr<ocv::matrix::matrix<float, 2> > r = nicta::ocv::matrix::pow(A, 3);

    CPPUNIT_ASSERT(r->extent(0) == 2);
    CPPUNIT_ASSERT(r->extent(1) == 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  8.0, (*r)[0][0](), 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 27.0, (*r)[0][1](), 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-64.0, (*r)[1][0](), 1e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.125, (*r)[1][1](), 1e-2);
    CPPUNIT_ASSERT( r->is_view() == false );
}


void MatrixTest :: exp()
{
    ocv::matrix::matrix<float, 2> A(ocv::matrix::extents<2>(2, 2));
    A[0][0]() =  2;  A[0][1]() = 3;
    A[1][0]() = -4;  A[1][1]() = 0.5;

    boost::shared_ptr<ocv::matrix::matrix<float, 2> > r = nicta::ocv::matrix::exp(A);

    CPPUNIT_ASSERT(r->extent(0) == 2);
    CPPUNIT_ASSERT(r->extent(1) == 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.389056099, (*r)[0][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(20.085536923, (*r)[0][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.018315639, (*r)[1][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.648721271, (*r)[1][1](), 1e-6);
    CPPUNIT_ASSERT( r->is_view() == false );
}


void MatrixTest :: log()
{
    ocv::matrix::matrix<float, 2> A(ocv::matrix::extents<2>(2, 2));
    A[0][0]() = 2;  A[0][1]() = 3;
    A[1][0]() = 4;  A[1][1]() = 0.5;

    boost::shared_ptr<ocv::matrix::matrix<float, 2> > r = nicta::ocv::matrix::log(A);

    CPPUNIT_ASSERT(r->extent(0) == 2);
    CPPUNIT_ASSERT(r->extent(1) == 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.693147181, (*r)[0][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.098612289, (*r)[0][1](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.386294361, (*r)[1][0](), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.693147181, (*r)[1][1](), 1e-6);
    CPPUNIT_ASSERT( r->is_view() == false );
}


void MatrixTest :: cast()
{
    ocv::matrix::matrix<float, 2> A(ocv::matrix::extents<2>(2, 2));
    A[0][0]() =  2.7;  A[0][1]() = 3.16;
    A[1][0]() = -4.0;  A[1][1]() = 0.5;

    boost::shared_ptr<ocv::matrix::matrix<int, 2> > r = nicta::ocv::matrix::matrix_cast<int> (A);

    CPPUNIT_ASSERT(r->extent(0) == 2);
    CPPUNIT_ASSERT(r->extent(1) == 2);
    CPPUNIT_ASSERT((*r)[0][0]() == 2);
    CPPUNIT_ASSERT((*r)[0][1]() == 3);
    CPPUNIT_ASSERT((*r)[1][0]() == -4);
    CPPUNIT_ASSERT((*r)[1][1]() == 0);
    CPPUNIT_ASSERT( r->is_view() == false );
}

