/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_CSV_H
#define NICTA_CSV_H

#include <boost/lexical_cast.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_bool.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_no_case.hpp>
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/spirit/include/qi_real.hpp>
#include <boost/spirit/include/qi_uint.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/utility/enable_if.hpp>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>

namespace nicta {
namespace csv {

    enum DelimiterType
    {
        char_   = 0 ,                    // delimiters are any of a series of literals
        string_ = 1                      // delimiter is a single phrase
    };

    enum CSVType
    {
        boolean      =  0 ,              // "true" | "false" | "T" | "F" | "1" | "0"                             * [case-insensitive]
        binary       =  1 ,              // +-01<b>
        octal        =  2 ,              // +-<0>01234567<o>
        hexadecimal  =  3 ,              // +-<0x>012345678910abcdefABCDEF<h>
        signed_int   =  4 ,              // +-0123456789
        unsigned_int =  5 ,              // +0123456789
        numeric      =  6 ,              // +-.0123456789                                                        * [incl decimal pt]
        alpha        =  7 ,              // a-zA-Z
        alphanum     =  8 ,              // a-zA-Z+-0123456789                                                   * [does NOT incl decimal pt]
        usrdef       =  9 ,              // user-specified printable chars modulo user-specified delimiters
        anystr       = 10                // all printable chars, modulo a literal-set, and modulo user-specified delimiters (we do modulo delims first)
    };


    template <typename nicta::csv::CSVType C>
    struct csv_type
    {
        typedef typename boost::mpl::map< boost::mpl::pair<boost::mpl::int_<nicta::csv::boolean>, bool>
                                        , boost::mpl::pair<boost::mpl::int_<nicta::csv::binary>, long double>
                                        , boost::mpl::pair<boost::mpl::int_<nicta::csv::octal>, long double>
                                        , boost::mpl::pair<boost::mpl::int_<nicta::csv::hexadecimal>, long double>
                                        , boost::mpl::pair<boost::mpl::int_<nicta::csv::signed_int>, long long int>
                                        , boost::mpl::pair<boost::mpl::int_<nicta::csv::unsigned_int>, unsigned long long int>
                                        , boost::mpl::pair<boost::mpl::int_<nicta::csv::numeric>, long double>
                                        , boost::mpl::pair<boost::mpl::int_<nicta::csv::alpha>, std::string>
                                        , boost::mpl::pair<boost::mpl::int_<nicta::csv::alphanum>, std::string>
                                        , boost::mpl::pair<boost::mpl::int_<nicta::csv::usrdef>, std::string>
                                        , boost::mpl::pair<boost::mpl::int_<nicta::csv::anystr>, std::string>
                                        > c_type;

        typedef typename boost::mpl::at<typename nicta::csv::csv_type<C>::c_type, boost::mpl::int_<C> >::type type;
    };



    template <nicta::csv::CSVType P>
    struct validate
    { };

    template < >
    struct validate <nicta::csv::boolean>
    {
        bool operator()(const char* arg, nicta::csv::csv_type<nicta::csv::boolean>::type& e) const throw ()
        {
            std::string Arg(arg);
            std::string::const_iterator first = Arg.begin();
            std::string::const_iterator last  = Arg.end();
            boost::spirit::qi::symbols<char, bool> sym_table;
            sym_table.add("true"  , true)
                         ("t"     , true)
                         ("1"     , true)
                         ("false" , false)
                         ("f"     , false)
                         ("0"     , false);
            return boost::spirit::qi::phrase_parse(first, last, (boost::spirit::qi::ascii::no_case[sym_table]), boost::spirit::qi::eol, e) && (first == last);
        }
    };

    template < >
    struct validate <nicta::csv::binary>
    {
        bool operator()(const char* arg, nicta::csv::csv_type<nicta::csv::binary>::type& e) const throw ()
        {
            // [See:  http://www.redhat.com/docs/manuals/enterprise/RHEL-4-Manual/gcc/hex-floats.html  for floating-pt hexadecimal in C]

            std::string s(arg);

            nicta::csv::csv_type<nicta::csv::binary>::type sign     = static_cast<nicta::csv::csv_type<nicta::csv::binary>::type> (1);
            nicta::csv::csv_type<nicta::csv::binary>::type fraction = static_cast<nicta::csv::csv_type<nicta::csv::binary>::type> (0);
            nicta::csv::csv_type<nicta::csv::binary>::type exponent = static_cast<nicta::csv::csv_type<nicta::csv::binary>::type> (0);
            if (s.size())
            {
                if (s[0] == '-')
                {
                    sign = static_cast<nicta::csv::csv_type<nicta::csv::binary>::type> (-1);
                }
                if (s[0] == '-' || s[0] == '+')
                {
                    std::string::iterator it = s.begin();
                    s = std::string(++it, s.end());
                }
                if (s[s.size() - 1] == 'b')
                {
                    // must be in format:  0-1.0-1p0-9<b>

                    std::string::iterator it = s.end();
                    s = std::string(s.begin(), --it);

                    std::string::size_type fp = s.find_first_of('.', 0);
                    std::string::size_type ex = s.find_first_of('p', 0);
                    if (ex == std::string::npos)
                    {
                        ex = s.find_first_of('P', 0);
                    }
                    if (fp != std::string::npos && ex != std::string::npos && (fp > ex))
                    {
                        // exponent precedes period  ..force an error

                        return false;
                    }
                    if (ex != std::string::npos && ex)
                    {
                        std::string exp = s.substr(ex + 1, std::string::npos);
                        s = s.substr(0, ex);

                        std::string::const_iterator first = exp.begin();
                        std::string::const_iterator last  = exp.end();
                        bool ret = boost::spirit::qi::phrase_parse(first, last, (boost::spirit::qi::long_double), boost::spirit::qi::eol, exponent) && (first == last);
                        if (!ret)
                        {
                            return false;
                        }
                    }
                    if (fp != std::string::npos)
                    {
                        std::string frac = s.substr(fp + 1, std::string::npos);
                        s = s.substr(0, fp);
                        if (!s.size())
                        {
                            s = "0";
                        }

                        std::string::const_iterator first = frac.begin();
                        std::string::const_iterator last  = frac.end();
                        boost::spirit::qi::rule<std::string::const_iterator, std::string()> rule = *boost::spirit::qi::char_("01");
                        bool ret = boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
                        if (!ret)
                        {
                            return false;
                        }
                        nicta::csv::csv_type<nicta::csv::binary>::type size = static_cast<nicta::csv::csv_type<nicta::csv::binary>::type> (frac.size());
                        for (nicta::csv::csv_type<nicta::csv::binary>::type i = 0; i != size; ++i)
                        {
                            fraction += static_cast<nicta::csv::csv_type<nicta::csv::binary>::type>
                                                   (boost::lexical_cast<nicta::csv::csv_type<nicta::csv::binary>::type> (frac[i]) / ::powl(2, i + 1));
                        }
                    }
                }
                else
                {
                    // no trailing 'b'

                    return false;
                }
            }
            std::string::const_iterator first = s.begin();
            std::string::const_iterator last  = s.end();
            boost::spirit::qi::rule<std::string::const_iterator, std::string()> rule = +boost::spirit::qi::char_("01");
            bool ret = boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
            if (!ret)
            {
                return false;
            }
            e = static_cast<nicta::csv::csv_type<nicta::csv::binary>::type> (0);
            nicta::csv::csv_type<nicta::csv::binary>::type size = static_cast<nicta::csv::csv_type<nicta::csv::binary>::type> (s.size());
            for (nicta::csv::csv_type<nicta::csv::binary>::type i = 0, j = size - 1; i != size; ++i, --j)
            {
                e += static_cast<nicta::csv::csv_type<nicta::csv::binary>::type> (::powl(2, i) *
                                                                                  boost::lexical_cast<nicta::csv::csv_type<nicta::csv::binary>::type> (s[j]));
            }
            e += fraction;
            e *= ::powl(2, exponent);
            e *= sign;

            return true;
        }
    };

    template < >
    struct validate <nicta::csv::octal>
    {
        bool operator()(const char* arg, nicta::csv::csv_type<nicta::csv::octal>::type& e) const throw ()
        {
            // [See:  http://www.redhat.com/docs/manuals/enterprise/RHEL-4-Manual/gcc/hex-floats.html  for floating-pt hexadecimal in C]

            std::string s(arg);

            nicta::csv::csv_type<nicta::csv::octal>::type sign     = static_cast<nicta::csv::csv_type<nicta::csv::octal>::type> (1);
            nicta::csv::csv_type<nicta::csv::octal>::type fraction = static_cast<nicta::csv::csv_type<nicta::csv::octal>::type> (0);
            nicta::csv::csv_type<nicta::csv::octal>::type exponent = static_cast<nicta::csv::csv_type<nicta::csv::octal>::type> (0);
            if (s.size())
            {
                if (s[0] == '-')
                {
                    sign = static_cast<nicta::csv::csv_type<nicta::csv::octal>::type> (-1);
                }
                if (s[0] == '-' || s[0] == '+')
                {
                    std::string::iterator it = s.begin();
                    s = std::string(++it, s.end());
                }
                bool flag = false;
                if (s[s.size() - 1] == 'o')
                {
                    // must be in format:  0-7.0-7p0-9<o> or <0>0-7.0-7p0-9

                    flag = true;
                    std::string::iterator it = s.end();
                    s = std::string(s.begin(), --it);
                }
                if (flag || s[0] == '0')
                {
                    std::string::size_type fp = s.find_first_of('.', 0);
                    std::string::size_type ex = s.find_first_of('p', 0);
                    if (ex == std::string::npos)
                    {
                        ex = s.find_first_of('P', 0);
                    }
                    if (fp != std::string::npos && ex != std::string::npos && (fp > ex))
                    {
                        // exponent precedes period  ..force an error

                        return false;
                    }
                    if (ex != std::string::npos && ex)
                    {
                        std::string exp = s.substr(ex + 1, std::string::npos);
                        s = s.substr(0, ex);

                        std::string::const_iterator first = exp.begin();
                        std::string::const_iterator last  = exp.end();
                        bool ret = boost::spirit::qi::phrase_parse(first, last, (boost::spirit::qi::long_double), boost::spirit::qi::eol, exponent) && (first == last);
                        if (!ret)
                        {
                            return false;
                        }
                    }
                    if (fp != std::string::npos)
                    {
                        std::string frac = s.substr(fp + 1, std::string::npos);
                        s = s.substr(0, fp);
                        if (!s.size())
                        {
                            s = "0";
                        }

                        std::string::const_iterator first = s.begin();
                        std::string::const_iterator last  = s.end();
                        boost::spirit::qi::rule<std::string::const_iterator, std::string()> rule = *boost::spirit::qi::char_("0-7");
                        bool ret = boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
                        if (!ret)
                        {
                            return false;
                        }
                        nicta::csv::csv_type<nicta::csv::octal>::type size = static_cast<nicta::csv::csv_type<nicta::csv::octal>::type> (frac.size());
                        for (nicta::csv::csv_type<nicta::csv::octal>::type i = 0; i != size; ++i)
                        {
                            fraction += static_cast<nicta::csv::csv_type<nicta::csv::octal>::type>
                                                   (boost::lexical_cast<nicta::csv::csv_type<nicta::csv::octal>::type> (frac[i]) / ::powl(8, i + 1));
                        }
                    }
                }
                else
                {
                    // no leading '0' but no trailing 'o' either

                    return false;
                }
            }
            std::string::const_iterator first = s.begin();
            std::string::const_iterator last  = s.end();
            boost::spirit::qi::rule<std::string::const_iterator, std::string()> rule = +boost::spirit::qi::char_("0-7");
            bool ret = boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
            if (!ret)
            {
                return false;
            }
            long long int ee;
            std::istringstream iss(s);
            iss >> std::oct >> ee;
            e  = ee;
            e += fraction;
            e *= ::powl(2, exponent);
            e *= sign;

            return true;
        }
    };

    template < >
    struct validate <nicta::csv::hexadecimal>
    {
        bool operator()(const char* arg, nicta::csv::csv_type<nicta::csv::hexadecimal>::type& e) const throw ()
        {
            // [See:  http://www.redhat.com/docs/manuals/enterprise/RHEL-4-Manual/gcc/hex-floats.html  for floating-pt hexadecimal in C]

            std::string s(arg);

            nicta::csv::csv_type<nicta::csv::hexadecimal>::type sign     = static_cast<nicta::csv::csv_type<nicta::csv::hexadecimal>::type> (1);
            nicta::csv::csv_type<nicta::csv::hexadecimal>::type fraction = static_cast<nicta::csv::csv_type<nicta::csv::hexadecimal>::type> (0);
            nicta::csv::csv_type<nicta::csv::hexadecimal>::type exponent = static_cast<nicta::csv::csv_type<nicta::csv::hexadecimal>::type> (0);
            if (s.size())
            {
                if (s[0] == '-')
                {
                    sign = static_cast<nicta::csv::csv_type<nicta::csv::hexadecimal>::type> (-1);
                }
                if (s[0] == '-' || s[0] == '+')
                {
                    std::string::iterator it = s.begin();
                    s = std::string(++it, s.end());
                }
                bool flag = false;
                if (s[s.size() - 1] == 'h')
                {
                    // must be in format:  0-9A-F.0-9A-Fp0-9<h> or <0x>0-9A-F.0-9A-Fp0-9

                    flag = true;
                    std::string::iterator it = s.end();
                    s = std::string(s.begin(), --it);
                }
                if (s.substr(0, 2) == std::string("0x"))
                {
                    flag = true;
                    std::string::iterator it = s.begin();
                    std::advance(it, 2);
                    s = std::string(it, s.end());
                }
                if (flag)
                {
                    std::string::size_type fp = s.find_first_of('.', 0);
                    std::string::size_type ex = s.find_first_of('p', 0);
                    if (ex == std::string::npos)
                    {
                        ex = s.find_first_of('P', 0);
                    }
                    if (fp != std::string::npos && ex != std::string::npos && (fp > ex))
                    {
                        // exponent precedes period  ..force an error

                        return false;
                    }
                    if (ex != std::string::npos && ex)
                    {
                        std::string exp = s.substr(ex + 1, std::string::npos);
                        s = s.substr(0, ex);

                        std::string::const_iterator first = exp.begin();
                        std::string::const_iterator last  = exp.end();
                        bool ret = boost::spirit::qi::phrase_parse(first, last, (boost::spirit::qi::long_double), boost::spirit::qi::eol, exponent) && (first == last);
                        if (!ret)
                        {
                            return false;
                        }
                    }
                    if (fp != std::string::npos)
                    {
                        std::string frac = s.substr(fp + 1, std::string::npos);
                        s = s.substr(0, fp);
                        if (!s.size())
                        {
                            s = "0";
                        }

                        std::string::const_iterator first = s.begin();
                        std::string::const_iterator last  = s.end();
                        boost::spirit::qi::rule<std::string::const_iterator, std::string()> rule = *boost::spirit::qi::char_("0-9a-fA-F");
                        bool ret = boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
                        if (!ret)
                        {
                            return false;
                        }
                        nicta::csv::csv_type<nicta::csv::hexadecimal>::type size = static_cast<nicta::csv::csv_type<nicta::csv::hexadecimal>::type> (frac.size());
                        for (nicta::csv::csv_type<nicta::csv::hexadecimal>::type i = 0; i != size; ++i)
                        {
                            unsigned int ee;
                            std::istringstream iss(std::string("0x") + frac[i]);
                            iss >> std::hex >> ee;
                            nicta::csv::csv_type<nicta::csv::hexadecimal>::type e = static_cast<nicta::csv::csv_type<nicta::csv::hexadecimal>::type> (ee);
                            fraction += static_cast<nicta::csv::csv_type<nicta::csv::hexadecimal>::type> (e / ::powl(16, i + 1));
                        }
                    }
                }
                else
                {
                    // no leading '0x' but no trailing 'h' either

                    return false;
                }
            }
            std::string::const_iterator first = s.begin();
            std::string::const_iterator last  = s.end();
            boost::spirit::qi::rule<std::string::const_iterator, std::string()> rule = +boost::spirit::qi::char_("0-9a-fA-F");
            bool ret = boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
            if (!ret)
            {
                return false;
            }
            long long int ee;
            std::istringstream iss(s);
            iss >> std::hex >> ee;
            e  = ee;
            e += fraction;
            e *= ::powl(2, exponent);
            e *= sign;

            return true;
        }
    };

    template < >
    struct validate <nicta::csv::signed_int>
    {
        bool operator()(const char* arg, nicta::csv::csv_type<nicta::csv::signed_int>::type& e) const throw ()
        {
            std::string Arg(arg);
            std::string::const_iterator first = Arg.begin();
            std::string::const_iterator last  = Arg.end();
            return boost::spirit::qi::phrase_parse(first, last, (boost::spirit::qi::long_long), boost::spirit::qi::eol, e) && (first == last);
        }
    };

    template < >
    struct validate <nicta::csv::unsigned_int>
    {
        bool operator()(const char* arg, nicta::csv::csv_type<nicta::csv::unsigned_int>::type& e) const throw ()
        {
            std::string Arg(arg);
            std::string::const_iterator first = Arg.begin();
            std::string::const_iterator last  = Arg.end();
            return boost::spirit::qi::phrase_parse(first, last, (boost::spirit::qi::ulong_long), boost::spirit::qi::eol, e) && (first == last);
        }
    };

    template < >
    struct validate <nicta::csv::numeric>
    {
        bool operator()(const char* arg, nicta::csv::csv_type<nicta::csv::numeric>::type& e) const throw ()
        {
            std::string Arg(arg);
            std::string::const_iterator first = Arg.begin();
            std::string::const_iterator last  = Arg.end();
            return boost::spirit::qi::phrase_parse(first, last, (boost::spirit::qi::long_double), boost::spirit::qi::eol, e) && (first == last);
        }
    };

    template < >
    struct validate <nicta::csv::alpha>
    {
        bool operator()(const char* arg) const throw ()
        {
            std::string Arg(arg);
            std::string::const_iterator first = Arg.begin();
            std::string::const_iterator last  = Arg.end();
            boost::spirit::qi::rule<std::string::const_iterator> rule = +boost::spirit::qi::alpha;
            return boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
        }
    };

    template < >
    struct validate <nicta::csv::alphanum>
    {
        bool operator()(const char* arg) const throw ()
        {
            std::string Arg(arg);
            std::string::const_iterator first = Arg.begin();
            std::string::const_iterator last  = Arg.end();
            boost::spirit::qi::rule<std::string::const_iterator, std::string()> rule = +boost::spirit::qi::char_("0-9a-zA-Z");
            return boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
        }
    };

    template < >
    struct validate <nicta::csv::usrdef>
    {
        bool operator()(const char* arg, const char* literals = "0-9a-zA-Z.*/_+-") const throw ()
        {
            // Return false if 'arg' contains anything *other than* 'literals', true otherwise
            // NOTE:  arg : { literals }

            if (!literals) return false;

            std::string Arg(arg);
            std::string::const_iterator first = Arg.begin();
            std::string::const_iterator last  = Arg.end();
            boost::spirit::qi::rule<std::string::const_iterator, std::string()> rule = +boost::spirit::qi::char_(literals);
            return boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
        }
    };

    template < >
    struct validate <nicta::csv::anystr>
    {
        bool operator()(const char* arg, const char* literals = 0) const throw ()
        {
            // Return false if 'arg' contains *any of* 'literals', true otherwise
            // NOTE:  if `literals = 0' then all printable characters are admitted
            // NOTE:  arg \ { literals }

            if (!arg) return false;
            if (!literals) return true;

            std::string Arg(arg);
            return Arg.find_first_of(literals, 0) == std::string::npos;
        }
    };


    template < typename T
             , template <typename T_, typename A_ = std::allocator<T_> > class Ctr
             , typename Enable = void
             >
    class csv
    { };


    template < typename T
             , template <typename T_, typename A_ = std::allocator<T_> > class Ctr
             >
    class csv < T
              , Ctr
              , typename boost::enable_if<boost::is_arithmetic<T> >::type
              >
    {
    public:

        csv(Ctr<T>& ctr) throw ();

        template < nicta::csv::CSVType P
                 , nicta::csv::DelimiterType D = nicta::csv::char_
                 >
        typename boost::enable_if< boost::mpl::equal_to< boost::mpl::int_<D>
                                                       , boost::mpl::int_<nicta::csv::char_>
                                                       >
                                 , bool
                                 >::type
        operator()(const char* arg, const char* delim = ",;:") const throw ();

        template < nicta::csv::CSVType P
                 , nicta::csv::DelimiterType D
                 >
        typename boost::enable_if< boost::mpl::equal_to< boost::mpl::int_<D>
                                                       , boost::mpl::int_<nicta::csv::string_>
                                                       >
                                 , bool
                                 >::type
        operator()(const char* arg, const char* delim = " ") const throw ();


    private:

        class actor
        {
        public:

            actor(Ctr<T>& ctr) throw ()
            : ctr_(ctr)
            {
            }

            template <typename V>
            void operator()(V const& value, boost::spirit::qi::unused_type, boost::spirit::qi::unused_type) const throw ()
            {
                BOOST_STATIC_ASSERT((boost::is_convertible<T, V>::value));

                this->ctr_.insert(this->ctr_.end(), static_cast<T> (value));
            }


        private:

            Ctr<T>& ctr_;
        };


        Ctr<T>& ctr_;
    };



    template <template <typename T_, typename A_ = std::allocator<T_> > class Ctr>
    class csv < std::string
              , Ctr
              , typename boost::enable_if<boost::is_class<std::string> >::type
              >
    {
    public:

        csv(Ctr<std::string>& ctr) throw ();

        template < nicta::csv::CSVType P
                 , nicta::csv::DelimiterType D = nicta::csv::char_
                 >
        typename boost::disable_if< boost::mpl::or_< boost::mpl::equal_to< boost::mpl::int_<P>
                                                                         , boost::mpl::int_<nicta::csv::usrdef>
                                                                         >
                                                   , boost::mpl::equal_to< boost::mpl::int_<P>
                                                                         , boost::mpl::int_<nicta::csv::anystr>
                                                                         >
                                                   , boost::mpl::equal_to< boost::mpl::int_<D>
                                                                         , boost::mpl::int_<nicta::csv::string_>
                                                                         >
                                                   >
                                  , bool
                                  >::type
        operator()(const char* arg, const char* delim = ",;:") const throw ();

        template < nicta::csv::CSVType P
                 , nicta::csv::DelimiterType D = nicta::csv::char_
                 >
        typename boost::enable_if< boost::mpl::and_< boost::mpl::equal_to< boost::mpl::int_<P>
                                                                         , boost::mpl::int_<nicta::csv::usrdef>
                                                                         >
                                                   , boost::mpl::equal_to< boost::mpl::int_<D>
                                                                         , boost::mpl::int_<nicta::csv::char_>
                                                                         >
                                                   >
                                 , bool
                                 >::type
        operator()(const char* arg,
                        const char* delim = ",;:",
                        const char* literals = "0-9a-zA-Z.*/_+-"
                       ) const throw ();

        template < nicta::csv::CSVType P
                 , nicta::csv::DelimiterType D = nicta::csv::char_
                 >
        typename boost::enable_if< boost::mpl::and_< boost::mpl::equal_to< boost::mpl::int_<P>
                                                                         , boost::mpl::int_<nicta::csv::anystr>
                                                                         >
                                                   , boost::mpl::equal_to< boost::mpl::int_<D>
                                                                         , boost::mpl::int_<nicta::csv::char_>
                                                                         >
                                                   >
                                 , bool
                                 >::type
        operator()(const char* arg,
                        const char* delim = ",;:",
                        const char* literals = 0
                       ) const throw ();



        template < nicta::csv::CSVType P
                 , nicta::csv::DelimiterType D
                 >
        typename boost::disable_if< boost::mpl::or_< boost::mpl::equal_to< boost::mpl::int_<P>
                                                                         , boost::mpl::int_<nicta::csv::usrdef>
                                                                         >
                                                   , boost::mpl::equal_to< boost::mpl::int_<P>
                                                                         , boost::mpl::int_<nicta::csv::anystr>
                                                                         >
                                                   , boost::mpl::equal_to< boost::mpl::int_<D>
                                                                         , boost::mpl::int_<nicta::csv::char_>
                                                                         >
                                                   >
                                  , bool
                                  >::type
        operator()(const char* arg, const char* delim = " ") const throw ();

        template < nicta::csv::CSVType P
                 , nicta::csv::DelimiterType D
                 >
        typename boost::enable_if< boost::mpl::and_< boost::mpl::equal_to< boost::mpl::int_<P>
                                                                         , boost::mpl::int_<nicta::csv::usrdef>
                                                                         >
                                                   , boost::mpl::equal_to< boost::mpl::int_<D>
                                                                         , boost::mpl::int_<nicta::csv::string_>
                                                                         >
                                                   >
                                 , bool
                                 >::type
        operator()(const char* arg,
                        const char* delim = " ",
                        const char* literals = "0-9a-zA-Z.*/_+-"
                       ) const throw ();

        template < nicta::csv::CSVType P
                 , nicta::csv::DelimiterType D
                 >
        typename boost::enable_if< boost::mpl::and_< boost::mpl::equal_to< boost::mpl::int_<P>
                                                                         , boost::mpl::int_<nicta::csv::anystr>
                                                                         >
                                                   , boost::mpl::equal_to< boost::mpl::int_<D>
                                                                         , boost::mpl::int_<nicta::csv::string_>
                                                                         >
                                                   >
                                 , bool
                                 >::type
        operator()(const char* arg,
                        const char* delim = " ",
                        const char* literals = 0
                       ) const throw ();


    private:

        Ctr<std::string>& ctr_;
    };

} } // namespace nicta::csv




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


template < typename T
         , template <typename T_, typename A_ = std::allocator<T_> > class Ctr
         >
nicta::csv::csv< T
               , Ctr
               , typename boost::enable_if<boost::is_arithmetic<T> >::type
               > :: 
csv(Ctr<T>& ctr) throw ()
: ctr_(ctr)
{
    // Constructor
}


template <template <typename T_, typename A_ = std::allocator<T_> > class Ctr>
nicta::csv::csv< std::string
               , Ctr
               , typename boost::enable_if<boost::is_class<std::string> >::type
               > :: 
csv(Ctr<std::string>& ctr) throw ()
: ctr_(ctr)
{
    // Constructor
}


template < typename T
         , template <typename T_, typename A_ = std::allocator<T_> > class Ctr
         >
template < nicta::csv::CSVType P
         , nicta::csv::DelimiterType D
         >
typename boost::enable_if< boost::mpl::equal_to< boost::mpl::int_<D>
                                               , boost::mpl::int_<nicta::csv::char_>
                                               >
                         , bool
                         >::type
nicta::csv::csv< T
               , Ctr
               , typename boost::enable_if<boost::is_arithmetic<T> >::type
               > :: 
operator()(const char* arg, const char* delim) const throw ()
{
    // Do the parse - using delimiters at the character level

    BOOST_STATIC_ASSERT(P != nicta::csv::alpha);
    BOOST_STATIC_ASSERT(P != nicta::csv::alphanum);
    BOOST_STATIC_ASSERT(P != nicta::csv::usrdef);
    BOOST_STATIC_ASSERT(P != nicta::csv::anystr);

    BOOST_STATIC_ASSERT((boost::is_convertible<T, typename nicta::csv::csv_type<P>::type>::value));

    std::string Arg(arg);
    std::string::const_iterator first = Arg.begin();
    std::string::const_iterator last  = Arg.end();
    boost::spirit::qi::rule<std::string::const_iterator, char()> delimiters = boost::spirit::qi::char_(" ");
    if (delim && std::string(delim).size())
    {
        delimiters = boost::spirit::qi::char_(delim);
    }
    boost::spirit::qi::rule<std::string::const_iterator> rule;
    switch (static_cast<int> (P))
    {
        case static_cast<int> (nicta::csv::boolean) :
        {
            boost::spirit::qi::symbols<char, bool> sym_table;
            sym_table.add("true"  , true)
                         ("t"     , true)
                         ("1"     , true)
                         ("false" , false)
                         ("f"     , false)
                         ("0"     , false);
            rule = boost::spirit::ascii::no_case[ sym_table[typename nicta::csv::csv< T
                                                                                    , Ctr
                                                                                    , typename boost::enable_if<boost::is_arithmetic<T> >::type
                                                                                    >::actor(this->ctr_)] ]
                   >> *(+delimiters >> boost::spirit::ascii::no_case[ sym_table[typename nicta::csv::csv< T
                                                                                                        , Ctr
                                                                                                        , typename boost::enable_if<boost::is_arithmetic<T> >::type
                                                                                                        >::actor(this->ctr_)] ]);
            return boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
        }

        case static_cast<int> (nicta::csv::binary) :
        case static_cast<int> (nicta::csv::octal) :
        case static_cast<int> (nicta::csv::hexadecimal) :
        {
            std::string delimiters(" ");
            if (std::string(delim).size())
            {
                delimiters = delim;
            }

            std::advance(first, Arg.find_first_not_of(delimiters, 0));
            std::string::const_iterator it = first;
            bool result = true;
            typename nicta::csv::csv_type<P>::type e = static_cast<typename nicta::csv::csv_type<P>::type> (0);
            while (it != last)
            {
                it = std::find_first_of(first, last, delimiters.begin(), delimiters.end());
                if (first != it)
                {
                    std::string s(first, it);
                    result = nicta::csv::validate<P>()(s.c_str(), e);
                    if (!result)
                    {
                        return result;
                    }
                    this->ctr_.insert(this->ctr_.end(), static_cast<T> (e));
                }
                if (it != last)
                {
                    first = ++it;
                }
            }
            return true;
        }

        case static_cast<int> (nicta::csv::signed_int) :
        {
            rule = boost::spirit::qi::long_long[typename nicta::csv::csv< T
                                                                        , Ctr
                                                                        , typename boost::enable_if<boost::is_arithmetic<T> >::type
                                                                        >::actor(this->ctr_)]
                   >> *(+delimiters >> boost::spirit::qi::long_long[typename nicta::csv::csv< T
                                                                                            , Ctr
                                                                                            , typename boost::enable_if<boost::is_arithmetic<T> >::type
                                                                                            >::actor(this->ctr_)]);
        }

        case static_cast<int> (nicta::csv::unsigned_int) :
        {
            rule = boost::spirit::qi::ulong_long[typename nicta::csv::csv< T
                                                                         , Ctr
                                                                         , typename boost::enable_if<boost::is_arithmetic<T> >::type
                                                                         >::actor(this->ctr_)]
                   >> *(+delimiters >> boost::spirit::qi::ulong_long[typename nicta::csv::csv< T
                                                                                             , Ctr
                                                                                             , typename boost::enable_if<boost::is_arithmetic<T> >::type
                                                                                             >::actor(this->ctr_)]);
        }

        case static_cast<int> (nicta::csv::numeric) :
        {
             rule = boost::spirit::qi::long_double[typename nicta::csv::csv< T
                                                                           , Ctr
                                                                           , typename boost::enable_if<boost::is_arithmetic<T> >::type
                                                                           >::actor(this->ctr_)]
                    >> *(+delimiters >> boost::spirit::qi::long_double[typename nicta::csv::csv< T
                                                                                               , Ctr
                                                                                               , typename boost::enable_if<boost::is_arithmetic<T> >::type
                                                                                               >::actor(this->ctr_)]);
        }
    }
    return boost::spirit::qi::parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
}


template < typename T
         , template <typename T_, typename A_ = std::allocator<T_> > class Ctr
         >
template < nicta::csv::CSVType P
         , nicta::csv::DelimiterType D
         >
typename boost::enable_if< boost::mpl::equal_to< boost::mpl::int_<D>
                                               , boost::mpl::int_<nicta::csv::string_>
                                               >
                         , bool
                         >::type
nicta::csv::csv< T
               , Ctr
               , typename boost::enable_if<boost::is_arithmetic<T> >::type
               > :: 
operator()(const char* arg, const char* delim) const throw ()
{
    // Do the parse - using delimiters at the phrase level

    BOOST_STATIC_ASSERT(P != nicta::csv::alpha);
    BOOST_STATIC_ASSERT(P != nicta::csv::alphanum);
    BOOST_STATIC_ASSERT(P != nicta::csv::usrdef);
    BOOST_STATIC_ASSERT(P != nicta::csv::anystr);

    BOOST_STATIC_ASSERT((boost::is_convertible<T, typename nicta::csv::csv_type<P>::type>::value));

    std::string delimiter(" ");
    if (delim && std::string(delim).size())
    {
        delimiter = delim;
    }

    std::string t;
    std::string s(arg);
    bool result = true;
    typename nicta::csv::csv_type<P>::type e = static_cast<typename nicta::csv::csv_type<P>::type> (0);
    do
    {
        std::string::size_type pos = s.find(delimiter, 0);
        if (pos == std::string::npos)
        {
            result = nicta::csv::validate<P>()(s.c_str(), e);
            if (result && s.size())
            {
                this->ctr_.insert(this->ctr_.end(), static_cast<T> (e));
            }
            return result;
        }
        std::string::iterator it = s.begin();
        std::advance(it, pos + delimiter.size());

        std::string u(s, 0, pos);
        std::string v(it, s.end());
        s = u;
        t = v;
        result = true;
        if (s.size())
        {
            result = nicta::csv::validate<P>()(s.c_str(), e);
            if (!result)
            {
                return result;
            }
            this->ctr_.insert(this->ctr_.end(), static_cast<T> (e));
        }
        s = t;
    }
    while (t.size());

    return result;
}


template <template <typename T_, typename A_ = std::allocator<T_> > class Ctr>
template < nicta::csv::CSVType P
         , nicta::csv::DelimiterType D
         >
typename boost::disable_if< boost::mpl::or_< boost::mpl::equal_to< boost::mpl::int_<P>
                                                                 , boost::mpl::int_<nicta::csv::usrdef>
                                                                 >
                                           , boost::mpl::equal_to< boost::mpl::int_<P>
                                                                 , boost::mpl::int_<nicta::csv::anystr>
                                                                 >
                                           , boost::mpl::equal_to< boost::mpl::int_<D>
                                                                 , boost::mpl::int_<nicta::csv::string_>
                                                                 >
                                           >
                          , bool
                          >::type
nicta::csv::csv< std::string
               , Ctr
               , typename boost::enable_if<boost::is_class<std::string> >::type
               > :: 
operator()(const char* arg, const char* delim) const throw ()
{
    // Do the parse using - delimiters at the character level

    BOOST_STATIC_ASSERT(P != nicta::csv::boolean);
    BOOST_STATIC_ASSERT(P != nicta::csv::binary);
    BOOST_STATIC_ASSERT(P != nicta::csv::octal);
    BOOST_STATIC_ASSERT(P != nicta::csv::hexadecimal);
    BOOST_STATIC_ASSERT(P != nicta::csv::signed_int);
    BOOST_STATIC_ASSERT(P != nicta::csv::unsigned_int);
    BOOST_STATIC_ASSERT(P != nicta::csv::numeric);
    //BOOST_STATIC_ASSERT(P != nicta::csv::usrdef);
    //BOOST_STATIC_ASSERT(P != nicta::csv::anystr);

    std::string Arg(arg);
    std::string::const_iterator first = Arg.begin();
    std::string::const_iterator last  = Arg.end();
    boost::spirit::qi::rule<std::string::const_iterator, char()> delimiters = boost::spirit::qi::char_(" ");
    if (delim && std::string(delim).size())
    {
        delimiters = boost::spirit::qi::char_(delim);
    }
    boost::spirit::qi::rule<std::string::const_iterator, std::string()> rtypes;
    switch (static_cast<int> (P))
    {
        case static_cast<int> (nicta::csv::alpha) :
        {
            rtypes     = +boost::spirit::qi::char_("a-zA-Z");
            break;
        }

        case static_cast<int> (nicta::csv::alphanum) :
        {
            rtypes     = +boost::spirit::qi::char_("0-9a-zA-Z");
            break;
        }
    }
    // GCC will ICE on this code b/c op()() here is a template function:
    //boost::spirit::qi::rule<std::string::const_iterator, Ctr<std::string>(), boost::spirit::eol_type> rule = rtypes % +delimiters;
    //return boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol, this->ctr_) && (first == last);

    // ..Alternative:
    boost::spirit::qi::rule<std::string::const_iterator, boost::spirit::eol_type> rule =
         rtypes[boost::phoenix::push_back(boost::phoenix::ref(this->ctr_), boost::spirit::qi::_1)]
         >> *(+delimiters >> rtypes[boost::phoenix::push_back(boost::phoenix::ref(this->ctr_), boost::spirit::qi::_1)]);

    return boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
}


template <template <typename T_, typename A_ = std::allocator<T_> > class Ctr>
template < nicta::csv::CSVType P
         , nicta::csv::DelimiterType D
         >
typename boost::enable_if< boost::mpl::and_< boost::mpl::equal_to< boost::mpl::int_<P>
                                                                 , boost::mpl::int_<nicta::csv::usrdef>
                                                                 >
                                           , boost::mpl::equal_to< boost::mpl::int_<D>
                                                                 , boost::mpl::int_<nicta::csv::char_>
                                                                 >
                                           >
                         , bool
                         >::type
nicta::csv::csv< std::string
               , Ctr
               , typename boost::enable_if<boost::is_class<std::string> >::type
               > :: 
operator()(const char* arg, const char* delim, const char* literals) const throw ()
{
    // Do the parse:  `usrdef' - using delimiters at the character level
    // NOTE:  arg : { literals }

    //BOOST_STATIC_ASSERT(P != nicta::csv::boolean);
    //BOOST_STATIC_ASSERT(P != nicta::csv::binary);
    //BOOST_STATIC_ASSERT(P != nicta::csv::octal);
    //BOOST_STATIC_ASSERT(P != nicta::csv::hexadecimal);
    //BOOST_STATIC_ASSERT(P != nicta::csv::signed_int);
    //BOOST_STATIC_ASSERT(P != nicta::csv::unsigned_int);
    //BOOST_STATIC_ASSERT(P != nicta::csv::numeric);
    //BOOST_STATIC_ASSERT(P != nicta::csv::alpha);
    //BOOST_STATIC_ASSERT(P != nicta::csv::alphanum);
    //BOOST_STATIC_ASSERT(P != nicta::csv::anystr);

    if (!literals || !std::string(literals).size()) return false;

    std::string Arg(arg);
    std::string::const_iterator first = Arg.begin();
    std::string::const_iterator last  = Arg.end();
    boost::spirit::qi::rule<std::string::const_iterator, char()> delimiters = boost::spirit::qi::char_(" ");
    if (delim && std::string(delim).size())
    {
        delimiters = boost::spirit::qi::char_(delim);
    }
    boost::spirit::qi::rule<std::string::const_iterator, std::string()> rtypes = +boost::spirit::qi::char_(literals);

    // GCC will ICE on this code b/c op()() here is a template function:
    //boost::spirit::qi::rule<std::string::const_iterator, Ctr<std::string>(), boost::spirit::eol_type> rule = rtypes % +delimiters;
    //return boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol, this->ctr_) && (first == last);

    // ..Alternative:
    boost::spirit::qi::rule<std::string::const_iterator, boost::spirit::eol_type> rule =
         rtypes[boost::phoenix::push_back(boost::phoenix::ref(this->ctr_), boost::spirit::qi::_1)]
         >> *(+delimiters >> rtypes[boost::phoenix::push_back(boost::phoenix::ref(this->ctr_), boost::spirit::qi::_1)]);
    
    return boost::spirit::qi::phrase_parse(first, last, rule, boost::spirit::qi::eol) && (first == last);
}


template <template <typename T_, typename A_ = std::allocator<T_> > class Ctr>
template < nicta::csv::CSVType P
         , nicta::csv::DelimiterType D
         >
typename boost::enable_if< boost::mpl::and_< boost::mpl::equal_to< boost::mpl::int_<P>
                                                                 , boost::mpl::int_<nicta::csv::anystr>
                                                                 >
                                           , boost::mpl::equal_to< boost::mpl::int_<D>
                                                                 , boost::mpl::int_<nicta::csv::char_>
                                                                 >
                                           >
                         , bool
                         >::type
nicta::csv::csv< std::string
               , Ctr
               , typename boost::enable_if<boost::is_class<std::string> >::type
               > :: 
operator()(const char* arg, const char* delim, const char* literals) const throw ()
{
    // Do the parse:  `anystr' - using delimiters at the character level
    // NOTE:  if `literals = 0' then all printable characters are admitted
    // NOTE:  arg \ { literals }

    //BOOST_STATIC_ASSERT(P != nicta::csv::boolean);
    //BOOST_STATIC_ASSERT(P != nicta::csv::binary);
    //BOOST_STATIC_ASSERT(P != nicta::csv::octal);
    //BOOST_STATIC_ASSERT(P != nicta::csv::hexadecimal);
    //BOOST_STATIC_ASSERT(P != nicta::csv::signed_int);
    //BOOST_STATIC_ASSERT(P != nicta::csv::unsigned_int);
    //BOOST_STATIC_ASSERT(P != nicta::csv::numeric);
    //BOOST_STATIC_ASSERT(P != nicta::csv::alpha);
    //BOOST_STATIC_ASSERT(P != nicta::csv::alphanum);
    //BOOST_STATIC_ASSERT(P != nicta::csv::usrdef);

    std::string Arg(arg);
    std::string::const_iterator first = Arg.begin();
    std::string::const_iterator last  = Arg.end();

    std::string delimiters(" ");
    if (delim && std::string(delim).size())
    {
        delimiters = delim;
    }
    std::advance(first, Arg.find_first_not_of(delimiters, 0));
    std::string::const_iterator it = first;
    bool result = true;
    while (it != last)
    {
        it = std::find_first_of(first, last, delimiters.begin(), delimiters.end());
        if (first != it)
        {
            std::string s(first, it);
            result = nicta::csv::validate<P>()(s.c_str(), literals);
            if (!result)
            {
                return result;
            }
            this->ctr_.insert(this->ctr_.end(), s);
        }
        if (it != last)
        {
            first = ++it;
        }
    }
    return result;
}


template <template <typename T_, typename A_ = std::allocator<T_> > class Ctr>
template < nicta::csv::CSVType P
         , nicta::csv::DelimiterType D
         >
typename boost::disable_if< boost::mpl::or_< boost::mpl::equal_to< boost::mpl::int_<P>
                                                                 , boost::mpl::int_<nicta::csv::usrdef>
                                                                 >
                                           , boost::mpl::equal_to< boost::mpl::int_<P>
                                                                 , boost::mpl::int_<nicta::csv::anystr>
                                                                 >
                                           , boost::mpl::equal_to< boost::mpl::int_<D>
                                                                 , boost::mpl::int_<nicta::csv::char_>
                                                                 >
                                           >
                          , bool
                          >::type
nicta::csv::csv< std::string
               , Ctr
               , typename boost::enable_if<boost::is_class<std::string> >::type
               > :: 
operator()(const char* arg, const char* delim) const throw ()
{
    // Do the parse - using delimiters at the phrase level

    BOOST_STATIC_ASSERT(P != nicta::csv::boolean);
    BOOST_STATIC_ASSERT(P != nicta::csv::binary);
    BOOST_STATIC_ASSERT(P != nicta::csv::octal);
    BOOST_STATIC_ASSERT(P != nicta::csv::hexadecimal);
    BOOST_STATIC_ASSERT(P != nicta::csv::signed_int);
    BOOST_STATIC_ASSERT(P != nicta::csv::unsigned_int);
    BOOST_STATIC_ASSERT(P != nicta::csv::numeric);
    //BOOST_STATIC_ASSERT(P != nicta::csv::anystr);
    //BOOST_STATIC_ASSERT(P != nicta::csv::usrdef);

    std::string delimiter(" ");
    if (delim && std::string(delim).size())
    {
        delimiter = delim;
    }

    std::string t;
    std::string s(arg);
    bool result = true;
    do
    {
        std::string::size_type pos = s.find(delimiter, 0);
        if (pos == std::string::npos)
        {
            result = nicta::csv::validate<P>()(s.c_str());
            if (result && s.size())
            {
                this->ctr_.insert(this->ctr_.end(), s);
            }
            return result;
        }
        std::string::iterator it = s.begin();
        std::advance(it, pos + delimiter.size());

        std::string u(s, 0, pos);
        std::string v(it, s.end());
        s = u;
        t = v;
        result = true;
        if (s.size())
        {
            result = nicta::csv::validate<P>()(s.c_str());
            if (!result)
            {
                return result;
            }
            this->ctr_.insert(this->ctr_.end(), s);
        }
        s = t;
    }
    while (t.size());

    return result;
}


template <template <typename T_, typename A_ = std::allocator<T_> > class Ctr>
template < nicta::csv::CSVType P
         , nicta::csv::DelimiterType D
         >
typename boost::enable_if< boost::mpl::and_< boost::mpl::equal_to< boost::mpl::int_<P>
                                                                 , boost::mpl::int_<nicta::csv::usrdef>
                                                                 >
                                           , boost::mpl::equal_to< boost::mpl::int_<D>
                                                                 , boost::mpl::int_<nicta::csv::string_>
                                                                 >
                                           >
                         , bool
                         >::type
nicta::csv::csv< std::string
               , Ctr
               , typename boost::enable_if<boost::is_class<std::string> >::type
               > :: 
operator()(const char* arg, const char* delim, const char* literals) const throw ()
{
    // Do the parse:  `usrdef' - using delimiters at the phrase level
    // NOTE:  arg : { literals }

    //BOOST_STATIC_ASSERT(P != nicta::csv::boolean);
    //BOOST_STATIC_ASSERT(P != nicta::csv::binary);
    //BOOST_STATIC_ASSERT(P != nicta::csv::octal);
    //BOOST_STATIC_ASSERT(P != nicta::csv::hexadecimal);
    //BOOST_STATIC_ASSERT(P != nicta::csv::signed_int);
    //BOOST_STATIC_ASSERT(P != nicta::csv::unsigned_int);
    //BOOST_STATIC_ASSERT(P != nicta::csv::numeric);
    //BOOST_STATIC_ASSERT(P != nicta::csv::alpha);
    //BOOST_STATIC_ASSERT(P != nicta::csv::alphanum);
    //BOOST_STATIC_ASSERT(P != nicta::csv::anystr);

    if (!literals || !std::string(literals).size()) return false;

    std::string delimiter(" ");
    if (delim && std::string(delim).size())
    {
        delimiter = delim;
    }

    std::string t;
    std::string s(arg);
    bool result = true;
    do
    {
        std::string::size_type pos = s.find(delimiter, 0);
        if (pos == std::string::npos)
        {
            result = nicta::csv::validate<P>()(s.c_str(), literals);
            if (result && s.size())
            {
                this->ctr_.insert(this->ctr_.end(), s);
            }
            return result;
        }
        std::string::iterator it = s.begin();
        std::advance(it, pos + delimiter.size());

        std::string u(s, 0, pos);
        std::string v(it, s.end());
        s = u;
        t = v;
        result = true;
        if (s.size())
        {
            result = nicta::csv::validate<P>()(s.c_str(), literals);
            if (!result)
            {
                return result;
            }
            this->ctr_.insert(this->ctr_.end(), s);
        }
        s = t;
    }
    while (t.size());

    return result;
}


template <template <typename T_, typename A_ = std::allocator<T_> > class Ctr>
template < nicta::csv::CSVType P
         , nicta::csv::DelimiterType D
         >
typename boost::enable_if< boost::mpl::and_< boost::mpl::equal_to< boost::mpl::int_<P>
                                                                 , boost::mpl::int_<nicta::csv::anystr>
                                                                 >
                                           , boost::mpl::equal_to< boost::mpl::int_<D>
                                                                 , boost::mpl::int_<nicta::csv::string_>
                                                                 >
                                           >
                         , bool
                         >::type
nicta::csv::csv< std::string
               , Ctr
               , typename boost::enable_if<boost::is_class<std::string> >::type
               > :: 
operator()(const char* arg, const char* delim, const char* literals) const throw ()
{
    // Do the parse:  `anystr' - using delimiters at the phrase level
    // NOTE:  if `literals = 0' then all printable characters are admitted
    // NOTE:  arg \ { literals }

    //BOOST_STATIC_ASSERT(P != nicta::csv::boolean);
    //BOOST_STATIC_ASSERT(P != nicta::csv::binary);
    //BOOST_STATIC_ASSERT(P != nicta::csv::octal);
    //BOOST_STATIC_ASSERT(P != nicta::csv::hexadecimal);
    //BOOST_STATIC_ASSERT(P != nicta::csv::signed_int);
    //BOOST_STATIC_ASSERT(P != nicta::csv::unsigned_int);
    //BOOST_STATIC_ASSERT(P != nicta::csv::numeric);
    //BOOST_STATIC_ASSERT(P != nicta::csv::alpha);
    //BOOST_STATIC_ASSERT(P != nicta::csv::alphanum);
    //BOOST_STATIC_ASSERT(P != nicta::csv::usrdef);

    std::string delimiter(" ");
    if (delim && std::string(delim).size())
    {
        delimiter = delim;
    }

    std::string t;
    std::string s(arg);
    bool result = true;
    do
    {
        std::string::size_type pos = s.find(delimiter, 0);
        if (pos == std::string::npos)
        {
            result = nicta::csv::validate<P>()(s.c_str(), literals);
            if (result && s.size())
            {
                this->ctr_.insert(this->ctr_.end(), s);
            }
            return result;
        }
        std::string::iterator it = s.begin();
        std::advance(it, pos + delimiter.size());

        std::string u(s, 0, pos);
        std::string v(it, s.end());
        s = u;
        t = v;
        result = true;
        if (s.size())
        {
            result = nicta::csv::validate<P>()(s.c_str(), literals);
            if (!result)
            {
                return result;
            }
            this->ctr_.insert(this->ctr_.end(), s);
        }
        s = t;
    }
    while (t.size());

    return result;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
