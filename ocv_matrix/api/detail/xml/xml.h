/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001-2010 Joel de Guzman.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_OCV_MATRIX_DETAIL_XML_XML_H
#define NICTA_OCV_MATRIX_DETAIL_XML_XML_H

#include <boost/foreach.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_variant.hpp>

#include <iostream>
#include <map>
#include <string>
#include <vector>


namespace nicta {
namespace ocv {
namespace matrix {
namespace detail {
namespace xml {

    struct xml_schema;

    typedef boost::variant< boost::recursive_wrapper<nicta::ocv::matrix::detail::xml::xml_schema>
                          , std::string
                          >
            xml_node;

    struct xml_schema
    {
        std::string name;                                                       // tag name
        std::vector<nicta::ocv::matrix::detail::xml::xml_node> children;        // children
    };

} } } } } // namespace nicta::ocv::matrix::detail::xml


// register xml_schema with boost::fusion
BOOST_FUSION_ADAPT_STRUCT(nicta::ocv::matrix::detail::xml::xml_schema,
                          (std::string, name)
                          (std::vector<nicta::ocv::matrix::detail::xml::xml_node>, children))


namespace nicta {
namespace ocv {
namespace matrix {
namespace detail {
namespace xml {

    /* warning:  global data! */
    std::string tag;
    std::map<std::string, std::string> data;         // a *flatened* representation of (non-repeats) from xml data-file


    class xml_schema_actor
    {
    public:

        xml_schema_actor(std::string& xml_header) throw ()
        {
            // Constructor:  pull out version string from header:-   <?xml version="1.0"?>

            // reset all global data!
            nicta::ocv::matrix::detail::xml::tag.clear();
            nicta::ocv::matrix::detail::xml::data.clear();

            std::string::size_type pos = xml_header.find_first_of("\"", 0);
            std::string::size_type rpos = xml_header.find_last_of("\"", std::string::npos);
            nicta::ocv::matrix::detail::xml::data["xml version"];
            if (pos != std::string::npos && rpos != std::string::npos)
            {
                ++pos;
                nicta::ocv::matrix::detail::xml::data["xml version"] = xml_header.substr(pos, rpos - pos);
            }
        }


        void operator()(xml_schema const& xml) const throw ();


    private:

        xml_schema_actor() throw ()
        {
            // Constructor
        }

        friend class xml_node_actor;
    };


    struct xml_node_actor : boost::static_visitor< >
    {
        xml_node_actor() throw ()
        {
        }

        void operator()(xml_schema const& xml) const throw ()
        {
            nicta::ocv::matrix::detail::xml::tag = xml.name;
            nicta::ocv::matrix::detail::xml::data[nicta::ocv::matrix::detail::xml::tag];
            nicta::ocv::matrix::detail::xml::xml_schema_actor()(xml);
        }


        void operator()(std::string const& text) const throw ()
        {
            nicta::ocv::matrix::detail::xml::data[nicta::ocv::matrix::detail::xml::tag] = text;
        }
    };


    void
    nicta::ocv::matrix::detail::xml::xml_schema_actor :: 
    operator()(nicta::ocv::matrix::detail::xml::xml_schema const& xml) const throw ()
    {
        BOOST_FOREACH(nicta::ocv::matrix::detail::xml::xml_node const& node, xml.children)
        {
            boost::apply_visitor(nicta::ocv::matrix::detail::xml::xml_node_actor(), node);
        }
    }


    //  Our mini XML grammar definition

    template <typename Iterator>
    class xml_schema_grammar : public boost::spirit::qi::grammar< Iterator
                                                                , nicta::ocv::matrix::detail::xml::xml_schema()
                                                                , boost::spirit::qi::locals<std::string>
                                                                , boost::spirit::ascii::space_type
                                                                >
    {
    public:

        xml_schema_grammar()
        : xml_schema_grammar::base_type(xml, "xml")
        {
            // Constructor

            text %= boost::spirit::qi::lexeme[+(boost::spirit::ascii::char_ - '<')];
            node %= xml | text;

            start_tag %= '<'
                      >> !boost::spirit::qi::lit('/')
                      >> boost::spirit::qi::lexeme[+(boost::spirit::qi::char_ - '>')]
                      >> '>';

            end_tag =  '<'
                    >> boost::spirit::qi::lexeme[+((boost::spirit::qi::char_ - '<') - '>')]
                    >> '>';

            xml %= start_tag[boost::spirit::labels::_a = boost::spirit::labels::_1]
                >> *node
                >> end_tag(boost::spirit::labels::_a);

            xml.name("xml");
            node.name("node");
            text.name("text");
            start_tag.name("start_tag");
            end_tag.name("end_tag");

            boost::spirit::qi::on_error<boost::spirit::qi::fail> (xml, std::cerr << boost::phoenix::val("Error! Expecting ")
                                               /* what failed.. */               << boost::spirit::labels::_4
                                                                                 << boost::phoenix::val(" here: \"")
                                               /* iters to error-pos, end */     << boost::phoenix::construct<std::string>(boost::spirit::labels::_3, boost::spirit::labels::_2)
                                                                                 << boost::phoenix::val("\"")
                                                                                 << std::endl);
        }


    private:

        boost::spirit::qi::rule<Iterator, xml_schema(), boost::spirit::qi::locals<std::string>, boost::spirit::ascii::space_type> xml;
        boost::spirit::qi::rule<Iterator, xml_node(), boost::spirit::ascii::space_type> node;
        boost::spirit::qi::rule<Iterator, std::string(), boost::spirit::ascii::space_type> text;
        boost::spirit::qi::rule<Iterator, std::string(), boost::spirit::ascii::space_type> start_tag;
        boost::spirit::qi::rule<Iterator, void(std::string), boost::spirit::ascii::space_type> end_tag;
    };

} } } } } // namespace nicta::ocv::matrix::detail::xml



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
