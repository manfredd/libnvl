/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_DETAIL_XML_FILE_H
#define NICTA_OCV_MATRIX_DETAIL_XML_FILE_H

#include "ocv_matrix/detail/xml/xml.h"
#include "ocv_matrix/extents.h"
#include "ocv_matrix/models/vector_model.h"
#include "ocv_matrix/ocv_matrix.h"

#include <nicta_common/csv.h>

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/int.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/utility/enable_if.hpp>

#include <algorithm>
#include <fstream>
#include <ios>
#include <string>
#include <stdexcept>
#include <vector>


namespace nicta {
namespace ocv {
namespace matrix {

    /* forward declaration */
    template < typename T
             , unsigned int D
             , unsigned int C
             , unsigned int U
             >
    class matrix;


namespace detail {
namespace xml {

    template < typename T
             , unsigned int D
             , unsigned int C
             , unsigned int U
             >
    typename boost::enable_if< typename boost::mpl::greater<boost::mpl::int_<D>, boost::mpl::int_<1> >::type
                             , boost::shared_ptr<nicta::ocv::matrix::matrix<T, D, C, U> >
                             >::type
    read_xml(const char* filename) throw (std::runtime_error, std::bad_exception);

    template < typename T
             , unsigned int D
             , unsigned int C
             , unsigned int U
             >
    typename boost::enable_if< typename boost::mpl::equal_to<boost::mpl::int_<D>, boost::mpl::int_<1> >::type
                             , boost::shared_ptr<nicta::ocv::matrix::matrix<T, D, C, U> >
                             >::type
    read_xml(const char* filename) throw (std::runtime_error, std::bad_exception);

} } } } } // namespace nicta::ocv::matrix::detail::xml



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
typename boost::enable_if< typename boost::mpl::greater<boost::mpl::int_<D>, boost::mpl::int_<1> >::type
                         , boost::shared_ptr<nicta::ocv::matrix::matrix<T, D, C, U> >
                         >::type
nicta::ocv::matrix::detail::xml :: 
read_xml(const char* filename) throw (std::runtime_error, std::bad_exception)
{
    // Read openCV XML data file, representing matrix data

    std::ifstream f(filename, std::ios_base::in);
    if (!f)
    {
        throw std::runtime_error("could not open data-file");
    }
    std::string header;
    std::getline(f, header);

    std::string buffer;
    f.unsetf(std::ios::skipws);                  // turn off whitespace skipping
    std::copy(std::istream_iterator<char>(f),
              std::istream_iterator<char>(),
              std::back_inserter(buffer));

    f.close();

    nicta::ocv::matrix::detail::xml::xml_schema_grammar<std::string::const_iterator>::xml_schema_grammar grammar;
    nicta::ocv::matrix::detail::xml::xml_schema ast;

    std::string::const_iterator first = buffer.begin();
    std::string::const_iterator last = buffer.end();
    bool ret = boost::spirit::qi::phrase_parse(first, last, grammar, boost::spirit::ascii::space, ast);
    if (!ret || first != last)
    {
        throw std::runtime_error("failed to parse XML-grammar");
    }
    nicta::ocv::matrix::detail::xml::xml_schema_actor cleave(header);
    cleave(ast);

    if (nicta::ocv::matrix::detail::xml::data.find("rows") != nicta::ocv::matrix::detail::xml::data.end())
    {
        std::string value = nicta::ocv::matrix::detail::xml::data["rows"] + std::string(" ") + nicta::ocv::matrix::detail::xml::data["cols"];
        nicta::ocv::matrix::detail::xml::data.erase("rows");
        nicta::ocv::matrix::detail::xml::data.erase("cols");
        nicta::ocv::matrix::detail::xml::data["sizes"] = value;
    }
    std::vector<unsigned int> extents;
    nicta::csv::csv<unsigned int, std::vector> ev(extents);
    ev.template operator()<nicta::csv::unsigned_int>(nicta::ocv::matrix::detail::xml::data["sizes"].c_str(), " \n");

    std::vector<T> data;
    nicta::csv::csv<T, std::vector> dv(data);
    dv.template operator()<nicta::csv::numeric>(nicta::ocv::matrix::detail::xml::data["data"].c_str(), " \n");

    nicta::ocv::matrix::extents<D> e(extents);
    nicta::ocv::matrix::matrix<T, D, C, U> m(e, data.data());                                                            // this is a view (shallow-copy) of 'data'
    return boost::shared_ptr<nicta::ocv::matrix::matrix<T, D, C, U> >(new nicta::ocv::matrix::matrix<T, D, C, U>(m));    // here, we've forced a deep-copy of 'data'
}


template < typename T
         , unsigned int D
         , unsigned int C
         , unsigned int U
         >
typename boost::enable_if< typename boost::mpl::equal_to<boost::mpl::int_<D>, boost::mpl::int_<1> >::type
                         , boost::shared_ptr<nicta::ocv::matrix::matrix<T, D, C, U> >
                         >::type
nicta::ocv::matrix::detail::xml :: 
read_xml(const char* filename) throw (std::runtime_error, std::bad_exception)
{
    // Read openCV XML data file, representing matrix data

    std::ifstream f(filename, std::ios_base::in);
    if (!f)
    {
        throw std::runtime_error("could not open data-file");
    }
    std::string header;
    std::getline(f, header);

    std::string buffer;
    f.unsetf(std::ios::skipws);                  // turn off whitespace skipping
    std::copy(std::istream_iterator<char>(f),
              std::istream_iterator<char>(),
              std::back_inserter(buffer));

    f.close();

    nicta::ocv::matrix::detail::xml::xml_schema_grammar<std::string::const_iterator>::xml_schema_grammar grammar;
    nicta::ocv::matrix::detail::xml::xml_schema ast;

    std::string::const_iterator first = buffer.begin();
    std::string::const_iterator last = buffer.end();
    bool ret = boost::spirit::qi::phrase_parse(first, last, grammar, boost::spirit::ascii::space, ast);
    if (!ret || first != last)
    {
        throw std::runtime_error("failed to parse XML-grammar");
    }
    nicta::ocv::matrix::detail::xml::xml_schema_actor cleave(header);
    cleave(ast);

    if (nicta::ocv::matrix::detail::xml::data.find("rows") != nicta::ocv::matrix::detail::xml::data.end())
    {
        std::string value = nicta::ocv::matrix::detail::xml::data["rows"] + std::string(" ") + nicta::ocv::matrix::detail::xml::data["cols"];
        nicta::ocv::matrix::detail::xml::data.erase("rows");
        nicta::ocv::matrix::detail::xml::data.erase("cols");
        nicta::ocv::matrix::detail::xml::data["sizes"] = value;
    }
    std::vector<unsigned int> extents;
    nicta::csv::csv<unsigned int, std::vector> ev(extents);
    ev.template operator()<nicta::csv::unsigned_int>(nicta::ocv::matrix::detail::xml::data["sizes"].c_str(), " \n");

    std::vector<T> data;
    nicta::csv::csv<T, std::vector> dv(data);
    dv.template operator()<nicta::csv::numeric>(nicta::ocv::matrix::detail::xml::data["data"].c_str(), " \n");

    std::vector<unsigned int> ex;
    ex.push_back(extents[1] == 1U ? extents[0] : extents[1]);
    nicta::ocv::matrix::extents<D> e(ex);
    nicta::ocv::matrix::matrix<T, D, C, U> m(e, data.data(), extents[1] == 1U ? nicta::ocv::matrix::Column : nicta::ocv::matrix::Row);  // this is a view (shallow-copy) of 'data'
    return boost::shared_ptr<nicta::ocv::matrix::matrix<T, D, C, U> >(new nicta::ocv::matrix::matrix<T, D, C, U>(m));                   // here, we've forced a deep-copy of 'data'
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
