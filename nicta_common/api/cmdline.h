/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NICTA_UTIL_CMDLINE_H
#define NICTA_UTIL_CMDLINE_H


#include "nicta_common/csv.h"
#include "nicta_common/detail/max_arity.h"
#include "nicta_common/nicta_exception.h"

#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/string.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/void.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/punctuation/paren.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/ref.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <cstdlib>
#include <initializer_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace nicta {
namespace util {
namespace cmdline {

    enum ContainerModel
    {
        absent_            =  0 ,        // returned when optional option was not invoked (and consequently not present) on the command-line
        flag_              =  1 ,        // option parsed was a flag
        boolean_           =  2 ,        // option parsed was a bool                         "true" | "false" | "T" | "F" | "1" | "0"           * [case-insensitive]
        binary_            =  3 ,        // option parsed was a binary number                +-01.0-1p+-0-9<b>
        octal_             =  4 ,        // option parsed was an octal number                +-<0>0-7.0-7p+-0-9<o>
        hexadecimal_       =  5 ,        // option parsed was a hexadecimal number           +-<0x>0-9a-fA-F.0-9a-fA-Fp+-0-9<h>
        signed_int_        =  6 ,        // option parsed was a signed integer               +-0123456789
        unsigned_int_      =  7 ,        // option parsed was an unsigned integer            +0123456789
        numeric_           =  8 ,        // option parsed was a floating point number        +-.0123456789                                      * [incl decmial pt]
        alpha_             =  9 ,        // option parsed was an alphabetical string         a-zA-Z
        alphanum_          = 10 ,        // option parsed was an alpha-numeric string        a-zA-Z+-0123456789                                 * [does NOT incl decimal pt]
        usrdef_            = 11 ,        // user-specified printable characters
        anystr_            = 12 ,        // all printable characters, modulo a literal-set
        boolean_list_      = 13 ,        // option parsed was a boolean list
        binary_list_       = 14 ,        // option parsed was a binary integer list
        octal_list_        = 15 ,        // option parsed was an octal integer list
        hexadecimal_list_  = 16 ,        // option parsed was a hexadecimal integer list
        signed_int_list_   = 17 ,        // option parsed was a signed integer list
        unsigned_int_list_ = 18 ,        // option parsed was an unsigned integer list
        numeric_list_      = 19 ,        // option parsed was a floating point numeric list
        alpha_list_        = 20 ,        // option parsed was an alphabetic (string) list
        alphanum_list_     = 21 ,        // option parsed was an alpha-numeric (string) list
        usrdef_list_       = 22 ,        // user-specified printable characters modulo user-specified delimiters
        anystr_list_       = 23          // all printable characters, modulo a literal-set, and modulo user-specified delimiters (we do modulo delims first)
    };


    struct required          // used to indicate that an argument IS *required* AND WILL NOT take on a default (fiducial) value
    {
        typedef void type;
    };
    struct optional          // used to indicate that an argument IS *optional* AND WILL NOT take on a default (fiducial) value
    {
        typedef void type;
    };
    struct preparse          // used for the raw underlying map
    {
        typedef std::string type;
    };
    struct flag : boost::mpl::void_
    {
        typedef boost::mpl::void_ attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::flag_> model;
        typedef boost::mpl::vector_c<char, 'f','l','a','g'> model_name;

        flag() throw ()
        { }

        explicit flag(const char* s) throw ()
        : value(s)
        { }

        explicit flag(const std::string& s) throw ()
        : value(s)
        { }

        bool operator<(const nicta::util::cmdline::flag& flag) const throw ()
        {
            return this->value < flag.value;
        }

        bool operator==(const nicta::util::cmdline::flag& flag) const throw ()
        {
            return this->value == flag.value;
        }

        std::string value;
    };
    struct boolean : nicta::csv::csv_type<nicta::csv::boolean>
    {
        typedef boost::mpl::int_<nicta::csv::boolean> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::boolean_> model;
        typedef boost::mpl::vector_c<char, 'b','o','o','l','e','a','n'> model_name;

        boolean() throw ()
        : value(true)
        { }

        type value;
    };
    struct binary : nicta::csv::csv_type<nicta::csv::binary>
    {
        typedef boost::mpl::int_<nicta::csv::binary> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::binary_> model;
        typedef boost::mpl::vector_c<char, 'b','i','n','a','r','y'> model_name;

        binary() throw ()
        : value(0)
        { }

        type value;
    };
    struct octal : nicta::csv::csv_type<nicta::csv::octal>
    {
        typedef boost::mpl::int_<nicta::csv::octal> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::octal_> model;
        typedef boost::mpl::vector_c<char, 'o','c','t','a','l'> model_name;

        octal() throw ()
        : value(0)
        { }

        type value;
    };
    struct hexadecimal : nicta::csv::csv_type<nicta::csv::hexadecimal>
    {
        typedef boost::mpl::int_<nicta::csv::hexadecimal> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::hexadecimal_> model;
        typedef boost::mpl::vector_c<char, 'h','e','x','a','d','e','c','i','m','a','l'> model_name;

        hexadecimal() throw ()
        : value(0)
        { }

        type value;
    };
    struct signed_int : nicta::csv::csv_type<nicta::csv::signed_int>
    {
        typedef boost::mpl::int_<nicta::csv::signed_int> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::signed_int_> model;
        typedef boost::mpl::vector_c<char, 's','i','g','n','e','d','_','i','n','t'> model_name;

        signed_int() throw ()
        : value(0)
        { }

        type value;
    };
    struct unsigned_int : nicta::csv::csv_type<nicta::csv::unsigned_int>
    {
        typedef boost::mpl::int_<nicta::csv::unsigned_int> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::unsigned_int_> model;
        typedef boost::mpl::vector_c<char, 'u','n','s','i','g','n','e','d','_','i','n','t'> model_name;

        unsigned_int() throw ()
        : value(0)
        { }

        type value;
    };
    struct numeric : nicta::csv::csv_type<nicta::csv::numeric>
    {
        typedef boost::mpl::int_<nicta::csv::numeric> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::numeric_> model;
        typedef boost::mpl::vector_c<char, 'n','u','m','e','r','i','c'> model_name;

        numeric() throw ()
        : value(0)
        { }

        type value;
    };
    struct alpha : nicta::csv::csv_type<nicta::csv::alpha>
    {
        typedef boost::mpl::int_<nicta::csv::alpha> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::alpha_> model;
        typedef boost::mpl::vector_c<char, 'a','l','p','h','a'> model_name;

        alpha() throw ()
        { }

        explicit alpha(const char* s) throw ()
        : value(s)
        { }

        explicit alpha(const std::string& s) throw ()
        : value(s)
        { }

        type value;
    };
    struct alphanum : nicta::csv::csv_type<nicta::csv::alphanum>
    {
        typedef boost::mpl::int_<nicta::csv::alphanum> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::alphanum_> model;
        typedef boost::mpl::vector_c<char, 'a','l','p','h','a','n','u','m'> model_name;

        alphanum() throw ()
        { }

        explicit alphanum(const char* s) throw ()
        : value(s)
        { }

        explicit alphanum(const std::string& s) throw ()
        : value(s)
        { }

        type value;
    };
    struct usrdef : nicta::csv::csv_type<nicta::csv::usrdef>
    {
        typedef boost::mpl::int_<nicta::csv::usrdef> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::usrdef_> model;
        typedef boost::mpl::vector_c<char, 'u','s','r','d','e','f'> model_name;

        usrdef() throw ()
        { }

        explicit usrdef(const char* s) throw ()
        : value(s)
        { }

        explicit usrdef(const std::string& s) throw ()
        : value(s)
        { }

        type value;
    };
    struct anystr : nicta::csv::csv_type<nicta::csv::anystr>
    {
        typedef boost::mpl::int_<nicta::csv::anystr> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::anystr_> model;
        typedef boost::mpl::vector_c<char, 'a','n','y','s','t','r'> model_name;

        anystr() throw ()
        { }

        explicit anystr(const char* s) throw ()
        : value(s)
        { }

        explicit anystr(const std::string& s) throw ()
        : value(s)
        { }

        type value;
    };
    struct boolean_list
    {
        typedef boost::mpl::int_<nicta::csv::boolean> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::boolean_list_> model;
        typedef boost::mpl::vector_c<char, 'b','o','o','l','e','a','n','_','l','i','s','t'> model_name;

        typedef std::vector<nicta::util::cmdline::boolean::type> type;
        type value;
    };
    struct binary_list
    {
        typedef boost::mpl::int_<nicta::csv::binary> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::binary_list_> model;
        typedef boost::mpl::vector_c<char, 'b','i','n','a','r','y','_','l','i','s','t'> model_name;

        typedef std::vector<nicta::util::cmdline::binary::type> type;
        type value;
    };
    struct octal_list
    {
        typedef boost::mpl::int_<nicta::csv::octal> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::octal_list_> model;
        typedef boost::mpl::vector_c<char, 'o','c','t','a','l','_','l','i','s','t'> model_name;

        typedef std::vector<nicta::util::cmdline::octal::type> type;
        type value;
    };
    struct hexadecimal_list
    {
        typedef boost::mpl::int_<nicta::csv::hexadecimal> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::hexadecimal_list_> model;
        typedef boost::mpl::vector_c<char, 'h','e','x','a','d','e','c','i','m','a','l','_','l','i','s','t'> model_name;

        typedef std::vector<nicta::util::cmdline::hexadecimal::type> type;
        type value;
    };
    struct signed_int_list
    {
        typedef boost::mpl::int_<nicta::csv::signed_int> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::signed_int_list_> model;
        typedef boost::mpl::vector_c<char, 's','i','g','n','e','d','_','i','n','t','_','l','i','s','t'> model_name;

        typedef std::vector<nicta::util::cmdline::signed_int::type> type;
        type value;
    };
    struct unsigned_int_list
    {
        typedef boost::mpl::int_<nicta::csv::unsigned_int> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::unsigned_int_list_> model;
        typedef boost::mpl::vector_c<char, 'u','n','s','i','g','n','e','d','_','i','n','t','_','l','i','s','t'> model_name;

        typedef std::vector<nicta::util::cmdline::unsigned_int::type> type;
        type value;
    };
    struct numeric_list
    {
        typedef boost::mpl::int_<nicta::csv::numeric> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::numeric_list_> model;
        typedef boost::mpl::vector_c<char, 'n','u','m','e','r','i','c','_','l','i','s','t'> model_name;

        typedef std::vector<nicta::util::cmdline::numeric::type> type;
        type value;
    };
    struct alpha_list
    {
        typedef boost::mpl::int_<nicta::csv::alpha> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::alpha_list_> model;
        typedef boost::mpl::vector_c<char, 'a','l','p','h','a','_','l','i','s','t'> model_name;

        typedef std::vector<nicta::util::cmdline::alpha::type> type;
        type value;
    };
    struct alphanum_list
    {
        typedef boost::mpl::int_<nicta::csv::alphanum> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::alphanum_list_> model;
        typedef boost::mpl::vector_c<char, 'a','l','p','h','a','n','u','m','_','l','i','s','t'> model_name;

        typedef std::vector<nicta::util::cmdline::alphanum::type> type;
        type value;
    };
    struct usrdef_list
    {
        typedef boost::mpl::int_<nicta::csv::usrdef> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::usrdef_list_> model;
        typedef boost::mpl::vector_c<char, 'u','s','r','d','e','f','_','l','i','s','t'> model_name;

        typedef std::vector<nicta::util::cmdline::usrdef::type> type;
        type value;
    };
    struct anystr_list
    {
        typedef boost::mpl::int_<nicta::csv::anystr> attribute;
        typedef boost::mpl::int_<nicta::util::cmdline::anystr_list_> model;
        typedef boost::mpl::vector_c<char, 'a','n','y','s','t','r','_','l','i','s','t'> model_name;

        typedef std::vector<nicta::util::cmdline::anystr::type> type;
        type value;
    };



    /* forward declaration */

    template < typename T
             , typename U
             , typename Enable
             >
    struct opt;



    class opt_map : private std::set<nicta::util::cmdline::flag>
                  , private std::map<std::string, nicta::util::cmdline::boolean>
                  , private std::map<std::string, nicta::util::cmdline::binary>
                  , private std::map<std::string, nicta::util::cmdline::octal>
                  , private std::map<std::string, nicta::util::cmdline::hexadecimal>
                  , private std::map<std::string, nicta::util::cmdline::signed_int>
                  , private std::map<std::string, nicta::util::cmdline::unsigned_int>
                  , private std::map<std::string, nicta::util::cmdline::numeric>
                  , private std::map<std::string, nicta::util::cmdline::alpha>
                  , private std::map<std::string, nicta::util::cmdline::alphanum>
                  , private std::map<std::string, nicta::util::cmdline::usrdef>
                  , private std::map<std::string, nicta::util::cmdline::anystr>
                  , private std::map<std::string, nicta::util::cmdline::boolean_list>
                  , private std::map<std::string, nicta::util::cmdline::binary_list>
                  , private std::map<std::string, nicta::util::cmdline::octal_list>
                  , private std::map<std::string, nicta::util::cmdline::hexadecimal_list>
                  , private std::map<std::string, nicta::util::cmdline::signed_int_list>
                  , private std::map<std::string, nicta::util::cmdline::unsigned_int_list>
                  , private std::map<std::string, nicta::util::cmdline::numeric_list>
                  , private std::map<std::string, nicta::util::cmdline::alpha_list>
                  , private std::map<std::string, nicta::util::cmdline::alphanum_list>
                  , private std::map<std::string, nicta::util::cmdline::usrdef_list>
                  , private std::map<std::string, nicta::util::cmdline::anystr_list>
                  , private std::map<std::string, nicta::util::cmdline::preparse::type>
    {
    public:

        opt_map(int argc,
                char** argv,
                const std::string& usage
               ) throw (nicta::nicta_exception)
        : pgmname_(opt_map::strip_name_(argv[0]))
        , usage_(std::string("SYNOPSIS       \n") + std::string(7, ' ') + this->pgmname_ + std::string("  ") + std::string(usage))
        {
            // Constructor
            // NOTE:  if '--help' flag given on command-line, dump the help and exit

            std::string spc7(7, ' ');
            std::string spc9(9, ' ');
            std::string::size_type syn = std::string("SYNOPSIS       \n").size() + 1;
            std::string::size_type idx = syn;
            std::string::size_type id0 = syn;
            do
            {
                idx = this->usage_.find('\n', idx);
                id0 = this->usage_.find("\n\n", id0);
                if (idx != std::string::npos)
                {
                    if ((id0 != std::string::npos) && (id0 == idx) && (id0 != this->usage_.size() - 2))
                    {
                        // id0 == idx and '\n\n' exists

                        idx += 2;
                        this->usage_.insert(idx, std::string(spc9.size() + this->pgmname_.size(), ' '));
                        this->usage_.replace(idx + spc7.size(), this->pgmname_.size(), this->pgmname_);
                        id0 = idx;
                    }
                    else
                    {
                        this->usage_.insert(++idx, std::string(spc9.size() + this->pgmname_.size(), ' '));
                    }
                }
            }
            while (idx != std::string::npos);
            id0 = idx = 0;

            for (int i = 1; i != argc; ++i)
            {
                std::string s(argv[i]);
                if (s.find_first_of('=') == std::string::npos)
                {
                    // flag option
                    if (s.size() < 2 || s.substr(0, 2) != "--")
                    {
                        throw nicta::nicta_exception(this->usage_, "opt_map", "opt_map");
                    }
                    if (s == "--help")
                    {
                        std::cerr << std::endl << this->usage_ << std::endl << std::endl;
                        exit(1);
                    }
                    this->std::set<nicta::util::cmdline::flag>::insert(nicta::util::cmdline::flag(s));
                }
                else
                {
                    // first pass - put in map<str, str> - upto user to then correctly parse!

                    // key-value option
                    if (this->std::map<std::string, nicta::util::cmdline::preparse::type>::find(s) != this->std::map<std::string, nicta::util::cmdline::preparse::type>::end())
                    {
                        // inserting duplicate key
                        throw nicta::nicta_exception(this->usage_, "opt_map", "opt_map");
                    }
                    this->std::map<std::string, nicta::util::cmdline::preparse::type>::insert(std::make_pair(s.substr(0, s.find_first_of('=')),
                                                                                                             (s.substr(s.find_first_of('=') + 1, std::string::npos))));
                }
            }
        }


        opt_map(int argc,
                char** argv,
                const std::string& usage,
                std::map<std::string, std::string>& manpage
               ) throw (nicta::nicta_exception)
        : pgmname_(opt_map::strip_name_(argv[0]))
        , usage_(std::string("SYNOPSIS       \n") + std::string(7, ' ') + this->pgmname_ + std::string("  ") + std::string(usage))
        , help_(manpage)
        {
            // Constructor
            // NOTE:  if '--help' flag given on command-line, dump the help and exit

            std::string spc7(7, ' ');
            std::string spc9(9, ' ');
            std::string::size_type syn = std::string("SYNOPSIS       \n").size() + 1;
            std::string::size_type idx = syn;
            std::string::size_type id0 = syn;
            do
            {
                idx = this->usage_.find('\n', idx);
                id0 = this->usage_.find("\n\n", id0);
                if (idx != std::string::npos)
                {
                    if ((id0 != std::string::npos) && (id0 == idx) && (id0 != this->usage_.size() - 2))
                    {
                        // id0 == idx and '\n\n' exists

                        idx += 2;
                        this->usage_.insert(idx, std::string(spc9.size() + this->pgmname_.size(), ' '));
                        this->usage_.replace(idx + spc7.size(), this->pgmname_.size(), this->pgmname_);
                        id0 = idx;
                    }
                    else
                    {
                        this->usage_.insert(++idx, std::string(spc9.size() + this->pgmname_.size(), ' '));
                    }
                }
            }
            while (idx != std::string::npos);
            id0 = idx = 0;

            for (int i = 1; i != argc; ++i)
            {
                std::string s(argv[i]);
                if (s.find_first_of('=') == std::string::npos)
                {
                    // flag option
                    if (s.size() < 2 || s.substr(0, 2) != "--")
                    {
                        throw nicta::nicta_exception(this->usage_, "opt_map", "opt_map");
                    }
                    if (s == "--help")
                    {
                        if (this->help_.empty())
                        {
                            std::cerr << std::endl << this->usage_ << std::endl << std::endl;
                            exit(1);
                        }
                        this->manpage_();    // exits when done
                    }
                    this->std::set<nicta::util::cmdline::flag>::insert(nicta::util::cmdline::flag(s));
                }
                else
                {
                    // first pass - put in map<str, str> - upto user to then correctly parse!

                    // key-value option
                    if (this->std::map<std::string, nicta::util::cmdline::preparse::type>::find(s) != this->std::map<std::string, nicta::util::cmdline::preparse::type>::end())
                    {
                        // inserting duplicate key
                        throw nicta::nicta_exception(this->usage_, "opt_map", "opt_map");
                    }
                    this->std::map<std::string, nicta::util::cmdline::preparse::type>::insert(std::make_pair(s.substr(0, s.find_first_of('=')),
                                                                                                             (s.substr(s.find_first_of('=') + 1, std::string::npos))));
                }
            }
        }


        bool full_match() const throw ()
        {
            // Return 'true' if this->std::map<string, string> is empty ..aka, we've appropriately parsed all command-line args
            // NOTE:  users **SHOULD** call this method once the opt_map has been completely 'parsed'

            if (this->std::map<std::string, nicta::util::cmdline::preparse::type>::empty())
            {
                return true;
            }
            return false;
        }


        template <typename T>
        typename boost::disable_if< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::preparse>
                                                   , boost::is_same<T, nicta::util::cmdline::flag>
                                                   >
                                  , typename T::type &
                                  >::type
        get(const char* key) throw (nicta::nicta_exception)
        {
            // Return value associated with a key; throw if non-extant

            typename std::map<std::string, T>::iterator it = this->template std::map<std::string, T>::find(std::string(key));
            if (it == this->std::map<std::string, T>::end())
            {
                // no known key
                throw nicta::nicta_exception(this->usage_, "get", "opt_map");
            }
            return it->second.value;
        }


        template < typename T
                 , typename U
                 >
        typename boost::disable_if< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::preparse>
                                                                    , boost::is_same<T, nicta::util::cmdline::flag>
                                                                    >
                                                   , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::binary_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::octal_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::hexadecimal_list>
                                                                    >
                                                   , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::unsigned_int_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::numeric_list>
                                                                    >
                                                   , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha>
                                                                    , boost::is_same<T, nicta::util::cmdline::alphanum>
                                                                    , boost::is_same<T, nicta::util::cmdline::usrdef>
                                                                    , boost::is_same<T, nicta::util::cmdline::anystr>
                                                                    >
                                                   , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::alphanum_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::usrdef_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::anystr_list>
                                                                    >
                                                   >
                                  , U
                                  >::type
        get(const char* key) throw (nicta::nicta_exception)
        {
            // Return value associated with a key, casting to type U; throw if non-extant

            typename std::map<std::string, T>::iterator it = this->template std::map<std::string, T>::find(std::string(key));
            if (it == this->std::map<std::string, T>::end())
            {
                // no known key
                throw nicta::nicta_exception(this->usage_, "get", "opt_map");
            }
            return static_cast<U> (it->second.value);
        }


        template <typename T>
        typename boost::disable_if< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::preparse>
                                                   , boost::is_same<T, nicta::util::cmdline::flag>
                                                   >
                                  , typename T::type &
                                  >::type
        get(const std::string& key) const throw (nicta::nicta_exception)
        {
            // Return value associated with a key; throw if non-extant

            typename std::map<std::string, T>::const_iterator it = this->template std::map<std::string, T>::find(key);
            if (it == this->std::map<std::string, T>::end())
            {
                // no known key
                throw nicta::nicta_exception(this->usage_, "get", "opt_map");
            }
            return it->second.value;
        }


        template < typename T
                 , typename U
                 >
        typename boost::disable_if< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::preparse>
                                                                    , boost::is_same<T, nicta::util::cmdline::flag>
                                                                    >
                                                   , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::binary_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::octal_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::hexadecimal_list>
                                                                    >
                                                   , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::unsigned_int_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::numeric_list>
                                                                    >
                                                   , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha>
                                                                    , boost::is_same<T, nicta::util::cmdline::alphanum>
                                                                    , boost::is_same<T, nicta::util::cmdline::usrdef>
                                                                    , boost::is_same<T, nicta::util::cmdline::anystr>
                                                                    >
                                                   , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::alphanum_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::usrdef_list>
                                                                    , boost::is_same<T, nicta::util::cmdline::anystr_list>
                                                                    >
                                                   >
                                  , U
                                  >::type
        get(const std::string& key) const throw (nicta::nicta_exception)
        {
            // Return value associated with a key, casting to type U; throw if non-extant

            typename std::map<std::string, T>::const_iterator it = this->template std::map<std::string, T>::find(key);
            if (it == this->std::map<std::string, T>::end())
            {
                // no known key
                throw nicta::nicta_exception(this->usage_, "get", "opt_map");
            }
            return static_cast<U> (it->second.value);
        }


        template <typename T>
        typename boost::enable_if< typename boost::is_same<T, nicta::util::cmdline::preparse>::type
                                 , typename T::type &
                                 >::type
        get(const char* key) throw (nicta::nicta_exception)
        {
            // Return value associated with a key; throw if non-extant

            typename std::map<std::string, typename T::type>::iterator it = this->template std::map<std::string, typename T::type>::find(std::string(key));
            if (it == this->std::map<std::string, typename T::type>::end())
            {
                // no known key
                throw nicta::nicta_exception(this->usage_, "get", "opt_map");
            }
            return it->second;
        }


        template <typename T>
        typename boost::enable_if< typename boost::is_same<T, nicta::util::cmdline::preparse>::type
                                 , typename T::type const &
                                 >::type
        get(const std::string& key) const throw (nicta::nicta_exception)
        {
            // Return value associated with a key; throw if non-extant

            typename std::map<std::string, typename T::type>::const_iterator it = this->template std::map<std::string, typename T::type>::find(key);
            if (it == this->std::map<std::string, typename T::type>::end())
            {
                // no known key
                throw nicta::nicta_exception(this->usage_, "get", "opt_map");
            }
            return it->second;
        }


        template <typename T>
        typename boost::disable_if< typename boost::is_same<T, nicta::util::cmdline::flag>::type
                                  , void
                                  >::type
        insert(const std::pair<std::string, T>& p) throw (nicta::nicta_exception)
        {
            // Insert a key-value pair *uniquely* into a map; throw if key already exists

            if (this->template std::map<std::string, T>::find(p.first) != this->std::map<std::string, T>::end())
            {
                // inserting duplicate key
                throw nicta::nicta_exception(this->usage_, "insert", "opt_map");
            }
            this->std::map< std::string, T>::insert(p);
        }


        void insert(const char* flag) throw ()
        {
            // Insert flag *uniquely* into set; we *DO NOT* throw if flag already exists

            this->std::set<nicta::util::cmdline::flag>::insert(nicta::util::cmdline::flag(flag));
        }


        template <typename T>
        typename boost::disable_if< typename boost::is_same<T, nicta::util::cmdline::flag>::type
                                  , void
                                  >::type
        erase_key(const char* key) throw (nicta::nicta_exception)
        {
            // Erase an option [--key=value] from the map `T'
            // Throws if an option is not found in map `T'
            // WARNING:  if user is holding a ref to key/value they will be left with a dangling reference!

            if (this->template have_key<T>(key))
            {
                this->std::map<std::string, T>::erase(std::string(key));
            }
        }


        bool erase_flag(const char* flag) throw ()
        {
            // Erase a flag [--flag] from the map, and return `true'
            // If a flag is not found, `false' is returned

            if (this->have_flag(flag))
            {
                return !!this->std::set<nicta::util::cmdline::flag>::erase(nicta::util::cmdline::flag(flag));
            }
            return false;
        }


        template <typename T>
        typename boost::disable_if< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::preparse>
                                                   , boost::is_same<T, nicta::util::cmdline::flag>
                                                   >
                                  , bool
                                  >::type
        have_key(const char* key) const throw ()
        {
            // Query if key exists:  --key=value

            return this->template std::map<std::string, T>::find(std::string(key)) != this->std::map<std::string, T>::end();
        }


        template <typename T>
        typename boost::disable_if< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::preparse>
                                                   , boost::is_same<T, nicta::util::cmdline::flag>
                                                   >
                                  , bool
                                  >::type
        have_key(const std::string& key) const throw ()
        {
            // Query if key exists:  --key=value

            return this->template std::map<std::string, T>::find(key) != this->std::map<std::string, T>::end();
        }


        template <typename T>
        typename boost::enable_if< typename boost::is_same<T, nicta::util::cmdline::preparse>::type
                                 , bool
                                 >::type
        have_key(const char* key) const throw ()
        {
            // Query if key exists:  --key=value

            return this->template std::map<std::string, preparse::type>::find(std::string(key)) != this->std::map<std::string, preparse::type>::end();
        }


        template <typename T>
        typename boost::enable_if< typename boost::is_same<T, nicta::util::cmdline::preparse>::type
                                 , bool
                                 >::type
        have_key(const std::string& key) const throw ()
        {
            // Query if key exists:  --key=value

            return this->template std::map<std::string, preparse::type>::find(key) != this->std::map<std::string, preparse::type>::end();
        }


        bool have_flag(const char* flag) const throw ()
        {
            // Query if flag exists:  --flag

            return this->std::set<nicta::util::cmdline::flag>::find(nicta::util::cmdline::flag(flag)) != this->std::set<nicta::util::cmdline::flag>::end();
        }


        bool have_flag(const std::string& flag) const throw ()
        {
            // Query if flag exists:  --flag

            return this->std::set<nicta::util::cmdline::flag>::find(nicta::util::cmdline::flag(flag)) != this->std::set<nicta::util::cmdline::flag>::end();
        }


        const std::string& usage() const throw ()
        {
            // Return the 'usage' help-string

            return this->usage_;
        }


    private:

        void manpage_() const throw ()
        {
            // Dump help, appropriately formatted (akin to a manpage) - then exit!
            //
            // NOTE:  common vars include:  { NAME ; SYNOPSIS ; DESCRIPTION ; DEFAULTS ; OPTIONS ; PROMPTS ; SECURITY ; ENVIRONMENT ; ENVIRONMENT VARIABLES ;  }
            //                              { FILES ; MISCELLANEOUS ; MISCELLANY ; RETURN VALUE ; ERRORS ; WARNINGS ; EXAMPLES ; NOTES ; BUGS ;     }
            //                              { AUTHOR ; REPORTING BUGS ; COPYRIGHT ; SEE ALSO ; HISTORY ;                                            }
            //
            //        ..any other vars will be output last in lexographic order (of what remains in the help_ map)
            //
            // use:  '\t'   for indent
            // use:  '\b'   for line-break
            // use:  '\n\n' for paragraph
            //
            // NOTE:  this will "eat" usage_, and the map help_ !!
            //

            std::string spc7(7, ' ');
            std::string spc9(9, ' ');
            std::string spc14(14, ' ');
            std::map<std::string, std::string>::iterator it;
            std::cerr << std::endl << std::endl << std::endl << std::endl;;

            std::cerr << "NAME" << std::endl << spc7 << this->pgmname_;
            it = this->help_.find("NAME");
            if (it != this->help_.end() && !it->second.empty())
            {
                std::cerr << " - ";

                // align & output text
                std::string::size_type idx = 0;
                do
                {
                    idx = it->second.find('\n', idx);
                    if (idx != std::string::npos)
                    {
                        if (idx + 1 < it->second.size())
                        {
                            std::string::size_type idx2 = it->second.find('\n', idx + 1);
                            if (idx2 != std::string::npos && (idx2 == idx + 1))
                            {
                                // preserve paragraphs

                                it->second.replace(idx2, 1, " ");
                                idx += 2;
                                if (idx >= it->second.size())
                                {
                                    break;
                                }
                                continue;
                            }
                        }
                        it->second.replace(idx, 1, " ");
                    }
                }
                while (idx != std::string::npos);

                nicta::util::cmdline::anystr_list::type vec;
                nicta::csv::csv<nicta::util::cmdline::anystr_list::type::value_type, std::vector> p(vec);
                bool result = p.operator()<nicta::csv::anystr>(it->second.c_str(), " ");
                if (!result)
                {
                    exit(1);    // parse failed
                }
                unsigned int count = spc7.size() + this->pgmname_.size() + 3;
                for (nicta::util::cmdline::anystr_list::type::iterator jt = vec.begin(); jt != vec.end(); ++jt)
                {
                    count += jt->size() + 1;
                    if (count > 81u)
                    {
                        count = spc7.size() + this->pgmname_.size() + 3;
                        std::cerr << std::endl << spc7 + std::string(this->pgmname_.size() + 3, ' ');
                    }
                    std::cerr << *jt << " ";

                    if (jt->find('\n', 0) != std::string::npos)
                    {
                        count = spc7.size() + this->pgmname_.size() + 3;
                        std::cerr << std::endl << spc7 + std::string(this->pgmname_.size() + 3, ' ');
                    }
                }

                this->help_.erase(it);
            }
            std::cerr << std::endl << std::endl;


            it = this->help_.find("SYNOPSIS");
            if (it != this->help_.end() && !it->second.empty())
            {
                std::cerr << "SYNOPSIS" << std::endl << spc7 << this->pgmname_ + "  ";
                std::string::size_type id0 = 0;
                std::string::size_type idx = 0;
                do
                {
                    idx = it->second.find('\n', idx);
                    id0 = it->second.find("\n\n", id0);
                    if (idx != std::string::npos)
                    {
                        if ((id0 != std::string::npos) && (id0 == idx) && (id0 != it->second.size() - 2))
                        {
                            // id0 == idx and '\n\n' exists

                            idx += 2;
                            it->second.insert(idx, std::string(spc9.size() + this->pgmname_.size(), ' '));
                            it->second.replace(idx + spc7.size(), this->pgmname_.size(), this->pgmname_);
                            id0 = idx;
                        }
                        else
                        {
                            it->second.insert(++idx, std::string(spc9.size() + this->pgmname_.size(), ' '));
                        }
                    }
                }
                while (idx != std::string::npos);
                std::cerr << it->second;
                this->help_.erase(it);
            }
            else
            {
                std::cerr << this->usage_;
            }
            std::cerr << std::endl << std::endl;


            it = this->help_.find("DESCRIPTION");
            if (it != this->help_.end())
            {
                std::cerr << "DESCRIPTION" << std::endl;
                this->text_align80_(it->second, spc7.size());
                this->help_.erase(it);
                std::cerr << std::endl << std::endl;
            }


            it = this->help_.find("DEFAULTS");
            if (it != this->help_.end())
            {
                std::cerr << "DEFAULTS" << std::endl;
                this->text_align80_(it->second, spc7.size());
                this->help_.erase(it);
                std::cerr << std::endl << std::endl;
            }


            bool have_opts = false;
            it = this->help_.find("OPTIONS");
            if (it != this->help_.end())
            {
                have_opts = true;
                std::cerr << "OPTIONS" << std::endl;
                this->text_align80_(it->second, spc7.size());
                this->help_.erase(it);
                std::cerr << std::endl;
            }
            bool once = false;
            for (std::map<std::string, std::string>::iterator jt = this->help_.begin(); jt != this->help_.end(); ++jt)
            {
                if (jt->first.find("--", 0) == 0)
                {
                    if (!have_opts)
                    {
                        once = true;
                        have_opts = true;
                        std::cerr << "OPTIONS";
                    }
                    if (once)
                    {
                        once = false;
                        std::cerr << std::endl << spc7 << jt->first;
                    }
                    else
                    {
                        std::cerr << std::endl << std::endl << spc7 << jt->first;
                    }
                    this->text_align80_(jt->second, spc14.size(), true);
                    this->help_.erase(jt->first);
                }
            }
            if (have_opts)
            {
                std::cerr << std::endl << std::endl;
            }

            const std::list<std::string> li = boost::assign::list_of ("PROMPTS") ("SECURITY") ("ENVIRONMENT") ("ENVIRONMENT VARIABLES") ("FILES") ("MISCELLANEOUS")
                                                                     ("MISCELLANY") ("RETURN VALUE") ("ERRORS") ("WARNINGS") ("EXAMPLES") ("NOTES") ("BUGS")
                                                                     ("AUTHOR") ("REPORTING BUGS") ("COPYRIGHT") ("SEE ALSO") ("HISTORY");
            for (std::list<std::string>::const_iterator jt = li.begin(); jt != li.end(); ++jt)
            {
                it = this->help_.find(*jt);
                if (it != this->help_.end())
                {
                    std::cerr << *jt << std::endl;
                    this->text_align80_(it->second, spc7.size());
                    this->help_.erase(it);
                    std::cerr << std::endl << std::endl;
                }
            }

            // output the remainder of the help..
            for (std::map<std::string, std::string>::iterator jt = this->help_.begin(); jt != this->help_.end(); ++jt)
            {
                std::cerr << jt->first << std::endl;
                this->text_align80_(jt->second, spc7.size());
                this->help_.erase(jt->first);
                std::cerr << std::endl << std::endl;
            }
            std::cerr << std::endl << std::endl;

            exit(1);    // terminate!
        }


        void text_align80_(std::string& s, unsigned int alignment = 7u, bool options = false) const throw ()
        {
            // Left justify text, indented 'alignment spaces' from left, and output with line breaks to get approx 80 character width

            std::string::size_type idx = 0;
            do
            {
                idx = s.find('\n', idx);
                if (idx != std::string::npos)
                {
                    if (idx + 1 < s.size())
                    {
                        std::string::size_type idx2 = s.find('\n', idx + 1);
                        if (idx2 != std::string::npos && (idx2 == idx + 1))
                        {
                            // preserve paragraphs

                            s.replace(idx2, 1, " ");
                            idx += 2;
                            if (idx >= s.size())
                            {
                                break;
                            }
                            continue;
                        }
                    }
                    s.replace(idx, 1, " ");
                }
            }
            while (idx != std::string::npos);
            idx = 0;
            do
            {
                idx = s.find('\b', idx);
                if (idx != std::string::npos)
                {
                    s.insert(idx, " ");
                    idx+=2;
                }
            }
            while(idx != std::string::npos);
            nicta::util::cmdline::anystr_list::type vec;
            nicta::csv::csv<nicta::util::cmdline::anystr_list::type::value_type, std::vector> p(vec);
            bool result = p.operator()<nicta::csv::anystr>(s.c_str(), " ");
            if (!result)
            {
                exit(1);    // parse failed
            }
            if (options)
            {
                std::cerr << std::endl;
            }

            bool retab = false;
            std::cerr << std::string(alignment, ' ');
            unsigned int count = static_cast<unsigned int> (alignment);
            for (nicta::util::cmdline::anystr_list::type::iterator jt = vec.begin(); jt != vec.end(); ++jt)
            {
                idx = jt->find('\t', 0);
                if (!idx)
                {
                    retab = true;
                    jt->replace(idx, 1, "");
                    count += static_cast<unsigned int> (alignment);
                    std::cerr << std::string(alignment, ' ');
                    idx = 0;
                }
                idx = jt->find('\b', 0);
                if (!idx)
                {
                    jt->replace(idx, 1, "");
                    count = static_cast<unsigned int> (alignment);
                    std::cerr << std::endl << std::string(alignment, ' ');
                    idx = 0;
                    if ((*jt)[idx] == '\t')
                    {
                        retab = true;
                        (*jt)[idx] = ' ';
                        count += static_cast<unsigned int> (alignment);
                        std::cerr<<std::string(alignment - 1, ' ');
                    }
                }

                count += jt->size() + 1u;
                if (count > 81u)
                {
                    count = static_cast<unsigned int> (alignment);
                    std::cerr << std::endl << std::string(alignment, ' ');
                    if (retab)
                    {
                        count += static_cast<unsigned int> (alignment);
                        std::cerr << std::string(alignment, ' ');
                    }
                }
                std::cerr << " " << *jt;

                if (jt->find('\n', 0) != std::string::npos)
                {
                    count = static_cast<unsigned int> (alignment);
                    std::cerr << std::endl << std::string(alignment, ' ');
                }
            }
        }


        static
        std::string
        strip_name_(const char* name)
        {
            // Strip off any path info on a file-string, returning the file-name only

            return std::string(boost::filesystem::path(name).filename());
        }


        const std::string pgmname_;
        mutable std::string usage_;
        mutable std::map<std::string, std::string> help_;

        template < typename T
                 , typename U
                 , typename Enabler
                 >
        friend struct nicta::util::cmdline::opt;
    };




    // ---- 8>< ----------------------------------------------- 8>< --------------------------------------------------- ><8 ---- //
    // WARNING:  if try to insert a default when a param by that name already exists - will get an error (unless it is a flag)!  //
    // ---- ><8 ----------------------------------------------- ><8 --------------------------------------------------- 8>< ---- //


    template < typename T
             , typename U
             , typename Enable = void
             >
    struct opt
    { };


    // ----


    template <typename U>
    struct opt < nicta::util::cmdline::required
               , U
               , boost::enable_if<boost::false_type>
               >
    { };


    // ----


    template <typename U>
    struct opt < nicta::util::cmdline::optional
               , U
               , boost::enable_if<boost::false_type>
               >
    { };


    // ----


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::flag>
                                                            , boost::is_same<U, nicta::util::cmdline::optional>
                                                            , boost::mpl::not_<boost::is_same<U, nicta::util::cmdline::required> >    // makes no sense for flags to be mandatory!
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m, const char* arg) throw ()
        {
            // Parses optional option of form:  --flag
            // Default:                  none

            std::string s(arg);
            if (!m.have_flag(s))
            {
                return nicta::util::cmdline::absent_;
            }
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::flag>
                                                            , boost::is_same<U, nicta::util::cmdline::flag>
                                                            , boost::mpl::not_<boost::is_same<U, nicta::util::cmdline::required> >    // makes no sense for flags to be mandatory!
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --flag
            // Default:                --flag

            std::string s(arg);
            if (!m.have_flag(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                std::string fid(fiducial);
                if (fid.size() < 2 || fid.substr(0, 2) != "--")
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                m.insert(fiducial);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::flag>
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            , boost::mpl::not_<boost::is_same<U, nicta::util::cmdline::required> >    // makes no sense for flags to be mandatory!
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --flag
            // Default:                --option=U  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }

            std::string s(arg);
            if (!m.have_flag(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                U e;
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, e.value))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, e);
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::flag>
                                                            , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<U, nicta::util::cmdline::alphanum>
                                                                             >
                                                            , boost::mpl::not_<boost::is_same<U, nicta::util::cmdline::required> >    // makes no sense for flags to be mandatory!
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --flag
            // Default:                --option=U  ={ alpha, alphanum }

            std::string s(arg);
            if (!m.have_flag(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::flag>
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef>
                                                            , boost::mpl::not_<boost::is_same<U, nicta::util::cmdline::required> >    // makes no sense for flags to be mandatory!
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --flag
            // Default:                --option=U  ={ usrdef }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !literals)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::flag>
                                                            , boost::is_same<U, nicta::util::cmdline::anystr>
                                                            , boost::mpl::not_<boost::is_same<U, nicta::util::cmdline::required> >    // makes no sense for flags to be mandatory!
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --flag
            // Default:                --option=U  ={ anystr }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::flag>
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            , boost::mpl::not_<boost::is_same<U, nicta::util::cmdline::required> >    // makes no sense for flags to be mandatory!
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --flag
            // Default:                --option=U1,U2,..,Un  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }

            std::string s(arg);
            if (!m.have_flag(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::flag>
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef_list>
                                                            , boost::mpl::not_<boost::is_same<U, nicta::util::cmdline::required> >    // makes no sense for flags to be mandatory!
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --flag
            // Default:                --option=U1,U2,..,Un  ={ usrdef_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !literals || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::flag>
                                                            , boost::is_same<U, nicta::util::cmdline::anystr_list>
                                                            , boost::mpl::not_<boost::is_same<U, nicta::util::cmdline::required> >    // makes no sense for flags to be mandatory!
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --flag
            // Default:                --option=U1,U2,..,Un  ={ anystr_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    // ----


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::required>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m, const char* arg) throw (nicta::nicta_exception)
        {
            // Parses mandatory option of form:  --option=T  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }
            // Default:                          none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                throw nicta::nicta_exception(std::string("expected missing option  ") + s, "parse", "opt");
            }
            T e;
            s = m.get<nicta::util::cmdline::preparse>(arg);
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(s.c_str(), e.value))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), e);
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::optional>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m, const char* arg) throw (nicta::nicta_exception)
        {
            // Parses optional option of form:  --option=T  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }
            // Default:                         none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                return nicta::util::cmdline::absent_;
            }
            T e;
            s = m.get<nicta::util::cmdline::preparse>(arg);
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(s.c_str(), e.value))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), e);
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::flag>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }
            // Default:                --flag

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                std::string fid(fiducial);
                if (fid.size() < 2 || fid.substr(0, 2) != "--")
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                m.insert(fiducial);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            T e;
            s = m.get<nicta::util::cmdline::preparse>(arg);
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(s.c_str(), e.value))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), e);
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }
            // Default:                --option=U  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                U e;
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, e.value))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, e);
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            T e;
            s = m.get<nicta::util::cmdline::preparse>(arg);
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(s.c_str(), e.value))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), e);
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<U, nicta::util::cmdline::alphanum>
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }
            // Default:                --option=U  ={ alpha, alphanum }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            T e;
            s = m.get<nicta::util::cmdline::preparse>(arg);
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(s.c_str(), e.value))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), e);
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }
            // Default:                --option=U  ={ usrdef }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !literals)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            T e;
            s = m.get<nicta::util::cmdline::preparse>(arg);
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(s.c_str(), e.value))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), e);
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::anystr>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }
            // Default:                --option=U  ={ anystr }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            T e;
            s = m.get<nicta::util::cmdline::preparse>(arg);
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(s.c_str(), e.value))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), e);
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {

        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T            ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }
            // Default:                --option=U1,U2,..,Un  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            T e;
            s = m.get<nicta::util::cmdline::preparse>(arg);
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(s.c_str(), e.value))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), e);
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }
            // Default:                --option=U  ={ usrdef_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !literals || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option-list failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            T e;
            s = m.get<nicta::util::cmdline::preparse>(arg);
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(s.c_str(), e.value))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), e);
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::anystr_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }
            // Default:                --option=U  ={ anystr_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option-list failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            T e;
            s = m.get<nicta::util::cmdline::preparse>(arg);
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(s.c_str(), e.value))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), e);
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    // ----


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<T, nicta::util::cmdline::alphanum>
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::required>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m, const char* arg) throw (nicta::nicta_exception)
        {
            // Parses mandatory option of form:  --option=T  ={ alpha, alphanum }
            // Default:                          none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                throw nicta::nicta_exception(std::string("expected missing option  ") + s, "parse", "opt");
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str()))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<T, nicta::util::cmdline::alphanum>
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::optional>
                                                            >
                                          >::type
               >
    {
        static
        typename boost::enable_if< boost::is_same<U, nicta::util::cmdline::optional>
                                 , nicta::util::cmdline::ContainerModel
                                 >::type
        parse(nicta::util::cmdline::opt_map& m, const char* arg) throw (nicta::nicta_exception)
        {
            // Parses optional option of form:  --option=T  ={ alpha, alphanum }
            // Default:                         none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                return nicta::util::cmdline::absent_;
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str()))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<T, nicta::util::cmdline::alphanum>
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::flag>
                                                            >
                                          >::type
               >
    {
        static
        typename boost::enable_if< boost::is_same<U, nicta::util::cmdline::flag>
                                 , nicta::util::cmdline::ContainerModel
                                 >::type
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ alpha, alphanum }
            // Default:                --flag

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                std::string fid(fiducial);
                if (fid.size() < 2 || fid.substr(0, 2) != "--")
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                m.insert(fiducial);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str()))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<T, nicta::util::cmdline::alphanum>
                                                                             >
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ alpha, alphanum }
            // Default:                --option=U  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                U e;
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, e.value))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, e);
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str()))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<T, nicta::util::cmdline::alphanum>
                                                                             >
                                                            , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<U, nicta::util::cmdline::alphanum>
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ alpha, alphanum }
            // Default:                --option=U  ={ alpha, alphanum }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str()))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<T, nicta::util::cmdline::alphanum>
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ alpha, alphanum }
            // Default:                --option=U  ={ usrdef }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !literals)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str()))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<T, nicta::util::cmdline::alphanum>
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::anystr>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ alpha, alphanum }
            // Default:                --option=U  ={ anystr }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str()))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<T, nicta::util::cmdline::alphanum>
                                                                             >
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T            ={ alpha, alphanum }
            // Default:                --option=U1,U2,..,Un  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str()))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<T, nicta::util::cmdline::alphanum>
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T            ={ alpha, alphanum }
            // Default:                --option=U1,U2,..,Un  ={ usrdef_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !literals || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str()))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<T, nicta::util::cmdline::alphanum>
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::anystr_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T           ={ alpha, alphanum }
            // Default:                --option=U1,U2,..Un  ={ anystr_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str()))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    // ----


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef>
                                                            , boost::is_same<U, nicta::util::cmdline::required>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses mandatory option of form:  --option=T  ={ usrdef }
            // Default:                          none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                throw nicta::nicta_exception(std::string("expected missing option  ") + s, "parse", "opt");
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef>
                                                            , boost::is_same<U, nicta::util::cmdline::optional>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses mandatory option of form:  --option=T  ={ usrdef }
            // Default:                         none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                return nicta::util::cmdline::absent_;
            }
            if (!nicta::csv::validate<T::attribute::vlue>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef>
                                                            , boost::is_same<U, nicta::util::cmdline::flag>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ usrdef }
            // Default:                --flag

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                std::string fid(fiducial);
                if (fid.size() < 2 || fid.substr(0, 2) != "--")
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                m.insert(fiducial);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef>
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ usrdef }
            // Default:                --option=U  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                U e;
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, e.value))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, e);
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef>
                                                            , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<U, nicta::util::cmdline::alphanum>
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ usrdef }
            // Default:                --option=U  ={ alpha, alphanum }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef>
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ usrdef }
            // Default:                --option=U  ={ usrdef }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !literals)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef>
                                                            , boost::is_same<U, nicta::util::cmdline::anystr>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* usr_literals = "0-9a-zA-Z.*/_+-",
              const char* any_literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ usrdef }
            // Default:                --option=U  ={ anystr }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, any_literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), usr_literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef>
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T            ={ usrdef }
            // Default:                --option=U1,U2,..,Un  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option-list failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef>
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T            ={ usrdef }
            // Default:                --option=U1,U2,..,Un  ={ usrdef_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !literals || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef>
                                                            , boost::is_same<U, nicta::util::cmdline::anystr_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* usr_literals = "0-9a-zA-Z.*/_+-",
              const char* any_literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T            ={ usrdef }
            // Default:                --option=U1,U2,..,Un  ={ anystr_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, any_literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), usr_literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    // ----


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr>
                                                            , boost::is_same<U, nicta::util::cmdline::required>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses mandatory option of form:  --option=T  ={ anystr }
            // Default:                          none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                throw nicta::nicta_exception(std::string("expected missing option  ") + s, "parse", "opt");
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr>
                                                            , boost::is_same<U, nicta::util::cmdline::optional>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses optional option of form:  --option=T  ={ anystr }
            // Default:                         none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                return nicta::util::cmdline::absent_;
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr>
                                                            , boost::is_same<U, nicta::util::cmdline::flag>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ anystr }
            // Default:                --flag

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                std::string fid(fiducial);
                if (fid.size() < 2 || fid.substr(0, 2) != "--")
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                m.insert(fiducial);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr>
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ anystr }
            // Default:                --option=U  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                U e;
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, e.value))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, e);
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr>
                                                            , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<U, nicta::util::cmdline::alphanum>
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ anystr }
            // Default:                --option=U  ={ alpha, alphanum }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr>
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* usr_literals = "0-9a-zA-Z.*/_+-",
              const char* any_literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ anystr }
            // Default:                --option=U  ={ usrdef }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !usr_literals)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, usr_literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), any_literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr>
                                                            , boost::is_same<U, nicta::util::cmdline::anystr>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T  ={ anystr }
            // Default:                --option=U  ={ anystr }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, static_cast<const char *> (0)))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr>
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T            ={ anystr }
            // Default:                --option=U1,U2,..,Un  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr>
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* usr_literals = "0-9a-zA-Z.*/_+-",
              const char* any_literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T            ={ anystr }
            // Default:                --option=U1,U2,..,Un  ={ usrdef_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !usr_literals || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, usr_literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), any_literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr>
                                                            , boost::is_same<U, nicta::util::cmdline::anystr_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T            ={ anystr }
            // Default:                --option=U1,U2,..,Un  ={ anystr_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (T::attribute::value)>()(m.get<nicta::util::cmdline::preparse>(arg).c_str(), literals))
            {
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            std::pair<std::string, T> p(std::string(arg), T(m.get<nicta::util::cmdline::preparse>(arg).c_str()));
            m.insert(p);
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    // ----


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::required>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* delimiters = ",;:"
             ) throw (nicta::nicta_exception)
        {
            // Parses mandatory option of form:  --option=T1,T2,..,Tn  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                            signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }
            // Default:                          none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                throw nicta::nicta_exception(std::string("expected missing option  ") + s, "parse", "opt");
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::optional>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* delimiters = ",;:"
             ) throw (nicta::nicta_exception)
        {
            // Parses optional option of form:  --option=T1,T2,..,Tn  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                           signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }
            // Default:                         --none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                return nicta::util::cmdline::absent_;
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::flag>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }
            // Default:                --flag

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                std::string fid(fiducial);
                if (fid.size() < 2 || fid.substr(0, 2) != "--")
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                m.insert(fiducial);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }
            // Default:                --option=U            ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                U e;
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, e.value))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, e);
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<U, nicta::util::cmdline::alphanum>
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }
            // Default:                --option=U            ={ alpha, alphanum }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }
            // Default:                --option=U            ={ usrdef }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !literals)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::anystr>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }
            // Default:                --option=U            ={ anystr }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {

        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }
            // Default:                --option=U1,U2,..,Un  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }
            // Default:                --option=U1,U2,..,Un  ={ usrdef_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !literals || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<T, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<T, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            , boost::is_same<U, nicta::util::cmdline::anystr_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }
            // Default:                --option=U1,U2,..,Un  ={ anystr_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    // ----


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef_list>
                                                            , boost::is_same<U, nicta::util::cmdline::required>
                                                            >
                                          >::type
               >
    {
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses mandatory option of form:  --option=T1,T2,..,Tn  ={ usrdef_list }
            // Default:                          none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                throw nicta::nicta_exception(std::string("expected missing option  ") + s, "parse", "opt");
            }
            if (!literals || !delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef_list>
                                                            , boost::is_same<U, nicta::util::cmdline::optional>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses optional option of form:  --option=T1,T2,..,Tn  ={ usrdef_list }
            // Default:                         none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                return nicta::util::cmdline::absent_;
            }
            if (!literals || !delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef_list>
                                                            , boost::is_same<U, nicta::util::cmdline::flag>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ usrdef_list }
            // Default:                --flag

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                std::string fid(fiducial);
                if (fid.size() < 2 || fid.substr(0, 2) != "--")
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                m.insert(fiducial);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!literals || !delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef_list>
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ usrdef_list }
            // Default:                --option=U            ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                U e;
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, e.value))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, e);
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!literals || !delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef_list>
                                                            , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<U, nicta::util::cmdline::alphanum>
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ usrdef_list }
            // Default:                --option=U            ={ alpha, alphanum }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!literals || !delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef_list>
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ usrdef_list }
            // Default:                --option=U            ={ usrdef }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !literals)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!literals || !delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef_list>
                                                            , boost::is_same<U, nicta::util::cmdline::anystr>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* usr_literals = "0-9a-zA-Z.*/_+-",
              const char* any_literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ usrdef_list }
            // Default:                --option=U            ={ anystr }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, any_literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!usr_literals || !delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, usr_literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef_list>
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ usrdef_list }
            // Default:                --option=U1,U2,..,Un  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!literals || !delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef_list>
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ usrdef_list }
            // Default:                --option=U1,U2,..,Un  ={ usrdef_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !literals || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!literals || !delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::usrdef_list>
                                                            , boost::is_same<U, nicta::util::cmdline::anystr_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* usr_literals = "0-9a-zA-Z.*/_+-",
              const char* any_literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ usrdef_list }
            // Default:                --option=U1,U2,..,Un  ={ anystr_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, any_literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!usr_literals || !delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, usr_literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    // ----


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr_list>
                                                            , boost::is_same<U, nicta::util::cmdline::required>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses mandatory option of form:  --option=T1,T2,..,Tn  ={ anystr_list }
            // Default:                          none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                throw nicta::nicta_exception(std::string("expected missing option  ") + s, "parse", "opt");
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr_list>
                                                            , boost::is_same<U, nicta::util::cmdline::optional>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ anystr_list }
            // Default:                         none

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                return nicta::util::cmdline::absent_;
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr_list>
                                                            , boost::is_same<U, nicta::util::cmdline::flag>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ anystr_list }
            // Default:                --flag

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                std::string fid(fiducial);
                if (fid.size() < 2 || fid.substr(0, 2) != "--")
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                m.insert(fiducial);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr_list>
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ anystr_list }
            // Default:                --option=U            ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                U e;
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, e.value))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, e);
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr_list>
                                                            , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha>
                                                                             , boost::is_same<U, nicta::util::cmdline::alphanum>
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ anystr_list }
            // Default:                --option=U            ={ alpha, alphanum }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr_list>
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* usr_literals = "0-9a-zA-Z.*/_+-",
              const char* any_literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ anystr_list }
            // Default:                --option=U            ={ usrdef }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !usr_literals)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, usr_literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, any_literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr_list>
                                                            , boost::is_same<U, nicta::util::cmdline::anystr>
                                                            >
                                          >::type
               >
    {
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ anystr_list }
            // Default:                --option=U            ={ anystr }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                if (!nicta::csv::validate<static_cast<nicta::csv::CSVType> (U::attribute::value)>()(fiducial, literals))
                {
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                std::pair<std::string, U> p(s, U(fiducial));
                m.insert(p);
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr_list>
                                                            , boost::mpl::or_< boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::boolean_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::binary_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::octal_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::hexadecimal_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::signed_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::unsigned_int_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::numeric_list>
                                                                                              >
                                                                             , boost::mpl::or_< boost::is_same<U, nicta::util::cmdline::alpha_list>
                                                                                              , boost::is_same<U, nicta::util::cmdline::alphanum_list>
                                                                                              >
                                                                             >
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ anystr_list }
            // Default:                --option=U1,U2,..,Un  ={ boolean_list, binary_list, octal_list, hexadecimal_list,
            //                                                  signed_int_list, unsigned_int_list, numeric_list, alpha_list, alphanum_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr_list>
                                                            , boost::is_same<U, nicta::util::cmdline::usrdef_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* usr_literals = "0-9a-zA-Z.*/_+-",
              const char* any_literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ anystr_list }
            // Default:                --option=U1,U2,..,Un  ={ usrdef_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !usr_literals || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, usr_literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, any_literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    template <typename T, typename U>
    struct opt < T
               , U
               , typename boost::enable_if< boost::mpl::and_< boost::is_same<T, nicta::util::cmdline::anystr_list>
                                                            , boost::is_same<U, nicta::util::cmdline::anystr_list>
                                                            >
                                          >::type
               >
    {
        static
        nicta::util::cmdline::ContainerModel
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* fiducial = 0,
              const char* delimiters = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // Parses option of form:  --option=T1,T2,..,Tn  ={ anystr_list }
            // Default:                --option=U1,U2,..,Un  ={ anystr_list }

            std::string s(arg);
            if (!m.have_key<nicta::util::cmdline::preparse>(s))
            {
                if (!fiducial || !delimiters)
                {
                    throw nicta::nicta_exception(m.usage_, "parse", "opt");
                }
                typename U::type& vec = m.std::map<std::string, U>::operator[](s).value;
                nicta::csv::csv<typename U::type::value_type, std::vector> p(vec);
                bool result = p.template operator()<static_cast<nicta::csv::CSVType> (U::attribute::value)>(fiducial, delimiters, literals);
                if (!result)
                {
                    m.std::map<std::string, U>::erase(std::string(arg));
                    throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename U::model_name>::value) + " option failed to parse", "parse", "opt");
                }
                return static_cast<nicta::util::cmdline::ContainerModel> (U::model::value);
            }
            if (!delimiters)
            {
                throw nicta::nicta_exception(m.usage_, "parse", "opt");
            }
            s = m.get<nicta::util::cmdline::preparse>(arg);
            typename T::type& vec = m.std::map<std::string, T>::operator[](arg).value;
            nicta::csv::csv<typename T::type::value_type, std::vector> p(vec);
            bool result = p.template operator()<static_cast<nicta::csv::CSVType> (T::attribute::value)>(s.c_str(), delimiters, literals);
            if (!result)
            {
                m.std::map<std::string, T>::erase(std::string(arg));
                throw nicta::nicta_exception(std::string(boost::mpl::c_str<typename T::model_name>::value) + " option failed to parse", "parse", "opt");
            }
            m.std::map<std::string, nicta::util::cmdline::preparse::type>::erase(std::string(arg));
            return static_cast<nicta::util::cmdline::ContainerModel> (T::model::value);
        }
    };


    // ----


    /* forward declaration */

    template < typename T                     // intended as a short-circuit FOR --flags (which by nature are usually *optional*), AND *required* other --options - ONLY!
             , typename Enable = void
             >
    struct opt_
    { };


    // ----


    template <typename T>
    struct opt_ < T
                , typename boost::enable_if<boost::is_same<T, nicta::util::cmdline::flag> >::type
                > : public opt< T
                              , nicta::util::cmdline::optional
                              , typename boost::enable_if<boost::is_same<T, nicta::util::cmdline::flag> >::type
                              >
    {
        // NOTE:  allows us to forgo unnecessary 2nd template parameter on flag options when there is no default

        // Parses optional option of form:  --flag
        // Default:                  none
    };


    template <typename T>
    struct opt_ < T
                , typename boost::disable_if<boost::is_same<T, nicta::util::cmdline::flag> >::type
                > : public opt< T
                              , nicta::util::cmdline::required
                              , typename boost::disable_if<boost::is_same<T, nicta::util::cmdline::flag> >::type
                              >
    {
        // NOTE:  allows us to forgo unnecessary 2nd template parameter on non-flag options when there is no default (and hence required)

        // Parses mandatory option of form:  --option=T  ={ boolean, binary, octal, hexadecimal, signed_int, unsigned_int, numeric, alpha, alphanum, usrdef, anystr,
        //                                                  boolean_list, binary_list, octal_list, hexadecimal_list, signed_int_list, unsigned_int_list, numeric_list,
        //                                                  alpha_list, alphanum_list, usrdef_list, anystr_list }
        // Default:                          none
    };


    // ----


        typedef std::pair<nicta::util::cmdline::ContainerModel, std::string> xor_result;


    // ----


    /* forward declaration */

    template < typename T
             , typename U
             , typename Enable = void
             >
    struct opt_xor1;


    // ----


    template <typename T, typename U>
    struct opt_xor1 < T
                    , U
                    , typename boost::enable_if< boost::mpl::and_< typename boost::is_same<U, nicta::util::cmdline::required>::type
                                                                 , typename boost::is_same<T, nicta::util::cmdline::flag>::type
                                                                 >
                                               >::type
                    > : public opt<T, U>
    {
        // effectively an alias for opt<T, U>
    };


    template <typename T, typename U>
    struct opt_xor1 < T
                    , U
                    , typename boost::enable_if< boost::mpl::and_< typename boost::is_same<U, nicta::util::cmdline::optional>::type
                                                                 , typename boost::is_same<T, nicta::util::cmdline::flag>::type
                                                                 >
                                               >::type
                    > : public opt<T, U>
    {
        // effectively an alias for opt<T, U>

        static
        nicta::util::cmdline::xor_result
        parse(nicta::util::cmdline::opt_map& m, const char* arg) throw (nicta::nicta_exception)
        {
            // NOTE:  unlike opt_xor{n} with n >= 2, if opt<T, U>::parse returns absent_, we still get the option-named-string (instead of an empty string)!

            return std::make_pair(nicta::util::cmdline::opt<T, U>::parse(m, arg), std::string(arg));
        }
    };


    // Restriction:  unfortunately, boost::mpl::or_ & boost::mpl::and_ take upto 5 template.params maximum
    template <typename T, typename U>
    struct opt_xor1 < T
                    , U
                    , typename boost::enable_if< boost::mpl::or_< boost::mpl::and_< boost::mpl::or_< typename boost::is_same<U, nicta::util::cmdline::optional>::type
                                                                                                   , typename boost::is_same<U, nicta::util::cmdline::required>::type
                                                                                                   >
                                                                                  , boost::mpl::or_< typename boost::is_same<T, nicta::util::cmdline::boolean>::type
                                                                                                   , typename boost::is_same<T, nicta::util::cmdline::signed_int>::type
                                                                                                   , typename boost::is_same<T, nicta::util::cmdline::unsigned_int>::type
                                                                                                   , typename boost::is_same<T, nicta::util::cmdline::numeric>::type
                                                                                                   >
                                                                                  >
                                                                , boost::mpl::and_< boost::mpl::or_< typename boost::is_same<U, nicta::util::cmdline::optional>::type
                                                                                                   , typename boost::is_same<U, nicta::util::cmdline::required>::type
                                                                                                   >
                                                                                  , boost::mpl::or_< typename boost::is_same<T, nicta::util::cmdline::binary>::type
                                                                                                   , typename boost::is_same<T, nicta::util::cmdline::octal>::type
                                                                                                   , typename boost::is_same<T, nicta::util::cmdline::hexadecimal>::type
                                                                                                   >
                                                                                  >
                                                                , boost::mpl::and_< boost::mpl::or_< typename boost::is_same<U, nicta::util::cmdline::optional>::type
                                                                                                   , typename boost::is_same<U, nicta::util::cmdline::required>::type
                                                                                                   >
                                                                                  , boost::mpl::or_< typename boost::is_same<T, nicta::util::cmdline::alpha>::type
                                                                                                   , typename boost::is_same<T, nicta::util::cmdline::alphanum>::type
                                                                                                   >
                                                                                  >
                                                                >
                                               >::type
                    > : public opt<T, U>
    {
        // effectively an alias for opt<T, U>

        static
        nicta::util::cmdline::xor_result
        parse(nicta::util::cmdline::opt_map& m, const char* arg) throw (nicta::nicta_exception)
        {
            // NOTE:  unlike opt_xor{n} with n >= 2, if opt<T, U>::parse returns absent_, we still get the option-named-string (instead of an empty string)!

            return std::make_pair(nicta::util::cmdline::opt<T, U>::parse(m, arg), std::string(arg));
        }
    };


    template <typename T, typename U>
    struct opt_xor1 < T
                    , U
                    , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< typename boost::is_same<U, nicta::util::cmdline::optional>::type
                                                                                  , typename boost::is_same<U, nicta::util::cmdline::required>::type
                                                                                  >
                                                                 , typename boost::is_same<T, nicta::util::cmdline::usrdef>::type
                                                                 >
                                               >::type
                    > : public opt<T, U>
    {
        // effectively an alias for opt<T, U>

        static
        nicta::util::cmdline::xor_result
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // NOTE:  unlike opt_xor{n} with n >= 2, if opt<T, U>::parse returns absent_, we still get the option-named-string (instead of an empty string)!

            return std::make_pair(nicta::util::cmdline::opt<T, U>::parse(m, arg, literals), std::string(arg));
        }
    };


    template <typename T, typename U>
    struct opt_xor1 < T
                    , U
                    , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< typename boost::is_same<U, nicta::util::cmdline::optional>::type
                                                                                  , typename boost::is_same<U, nicta::util::cmdline::required>::type
                                                                                  >
                                                                 , typename boost::is_same<T, nicta::util::cmdline::anystr>::type
                                                                 >
                                               >::type
                    > : public opt<T, U>
    {
        // effectively an alias for opt<T, U>

        static
        nicta::util::cmdline::xor_result
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // NOTE:  unlike opt_xor{n} with n >= 2, if opt<T, U>::parse returns absent_, we still get the option-named-string (instead of an empty string)!

            return std::make_pair(nicta::util::cmdline::opt<T, U>::parse(m, arg, literals), std::string(arg));
        }
    };


    // Restriction:  unfortunately, boost::mpl::or_ & boost::mpl::and_ take upto 5 template.params maximum
    template <typename T, typename U>
    struct opt_xor1 < T
                    , U
                    , typename boost::enable_if< boost::mpl::or_< boost::mpl::and_< boost::mpl::or_< typename boost::is_same<U, nicta::util::cmdline::optional>::type
                                                                                                   , typename boost::is_same<U, nicta::util::cmdline::required>::type
                                                                                                   >
                                                                                  , boost::mpl::or_< typename boost::is_same<T, nicta::util::cmdline::boolean_list>::type
                                                                                                   , typename boost::is_same<T, nicta::util::cmdline::signed_int_list>::type
                                                                                                   , typename boost::is_same<T, nicta::util::cmdline::unsigned_int_list>::type
                                                                                                   , typename boost::is_same<T, nicta::util::cmdline::numeric_list>::type
                                                                                                   >
                                                                                  >
                                                                , boost::mpl::and_< boost::mpl::or_< typename boost::is_same<U, nicta::util::cmdline::optional>::type
                                                                                                   , typename boost::is_same<U, nicta::util::cmdline::required>::type
                                                                                                   >
                                                                                  , boost::mpl::or_< typename boost::is_same<T, nicta::util::cmdline::binary_list>::type
                                                                                                   , typename boost::is_same<T, nicta::util::cmdline::octal_list>::type
                                                                                                   , typename boost::is_same<T, nicta::util::cmdline::hexadecimal_list>::type
                                                                                                   >
                                                                                  >
                                                                , boost::mpl::and_< boost::mpl::or_< typename boost::is_same<U, nicta::util::cmdline::optional>::type
                                                                                                   , typename boost::is_same<U, nicta::util::cmdline::required>::type
                                                                                                   >
                                                                                  , boost::mpl::or_< typename boost::is_same<T, nicta::util::cmdline::alpha_list>::type
                                                                                                   , typename boost::is_same<T, nicta::util::cmdline::alphanum_list>::type
                                                                                                   >
                                                                                  >
                                                                >
                                               >::type
                    > : public opt<T, U>
    {
        // effectively an alias for opt<T, U>

        static
        nicta::util::cmdline::xor_result
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* delim = ",;:"
             ) throw (nicta::nicta_exception)
        {
            // NOTE:  unlike opt_xor{n} with n >= 2, if opt<T, U>::parse returns absent_, we still get the option-named-string (instead of an empty string)!

            return std::make_pair(nicta::util::cmdline::opt<T, U>::parse(m, arg, delim), std::string(arg));
        }
    };


    template <typename T, typename U>
    struct opt_xor1 < T
                    , U
                    , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< typename boost::is_same<U, nicta::util::cmdline::optional>::type
                                                                                  , typename boost::is_same<U, nicta::util::cmdline::required>::type
                                                                                  >
                                                                 , typename boost::is_same<T, nicta::util::cmdline::usrdef_list>::type
                                                                 >
                                               >::type
                    > : public opt<T, U>
    {
        // effectively an alias for opt<T, U>

        static
        nicta::util::cmdline::xor_result
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* delim = ",;:",
              const char* literals = "0-9a-zA-Z.*/_+-"
             ) throw (nicta::nicta_exception)
        {
            // NOTE:  unlike opt_xor{n} with n >= 2, if opt<T, U>::parse returns absent_, we still get the option-named-string (instead of an empty string)!

            return std::make_pair(nicta::util::cmdline::opt<T, U>::parse(m, arg, delim, literals), std::string(arg));
        }
    };


    template <typename T, typename U>
    struct opt_xor1 < T
                    , U
                    , typename boost::enable_if< boost::mpl::and_< boost::mpl::or_< typename boost::is_same<U, nicta::util::cmdline::optional>::type
                                                                                  , typename boost::is_same<U, nicta::util::cmdline::required>::type
                                                                                  >
                                                                 , typename boost::is_same<T, nicta::util::cmdline::anystr_list>::type
                                                                 >
                                               >::type
                    > : public opt<T, U>
    {
        // effectively an alias for opt<T, U>

        static
        nicta::util::cmdline::xor_result
        parse(nicta::util::cmdline::opt_map& m,
              const char* arg,
              const char* delim = ",;:",
              const char* literals = 0
             ) throw (nicta::nicta_exception)
        {
            // NOTE:  unlike opt_xor{n} with n >= 2, if opt<T, U>::parse returns absent_, we still get the option-named-string (instead of an empty string)!

            return std::make_pair(nicta::util::cmdline::opt<T, U>::parse(m, arg, delim, literals), std::string(arg));
        }
    };


    // ----


#define TMP_PARAM_CHECK(z, n, T) BOOST_STATIC_ASSERT(( !boost::is_same<T ## n, nicta::util::cmdline::optional>::value ));                               \
                                 BOOST_STATIC_ASSERT(( !boost::is_same<T ## n, nicta::util::cmdline::required>::value ));
/**/



# define OPERATOR(z, n, _)                                                                                                                              \
                                                                                                                                                        \
    template < BOOST_PP_ENUM_PARAMS(n, typename R)                                                                                                      \
             , typename U                                                                                                                               \
             >                                                                                                                                          \
    class opt_xor ## n                                /* NOTE:  opt_xor{n} ; where (`n' >= 2), and `n' is the number of possible options/flags */       \
    {                                                                                                                                                   \
    public:                                                                                                                                             \
                                                                                                                                                        \
        static                                                                                                                                          \
        typename boost::enable_if< boost::mpl::or_< typename boost::is_same<U, nicta::util::cmdline::optional>::type                                    \
                                                  , typename boost::is_same<U, nicta::util::cmdline::required>::type                                    \
                                                  >                                                                                                     \
                                 , nicta::util::cmdline::xor_result                                                                                     \
                                 >::type                                                                                                                \
        parse(nicta::util::cmdline::opt_map& m,                                                                                                         \
              BOOST_PP_ENUM_PARAMS(n, const char* arg),                                                                                                 \
              const char* delim = ",;:",                                                                                                                \
              const char* usr_literals = "0-9a-zA-Z.*/_+-",                                                                                             \
              const char* any_literals = 0                                                                                                              \
             ) throw (nicta::nicta_exception)                                                                                                           \
        {                                                                                                                                               \
            /* Parses option of form:   --option=R1 | --option=R2 | ... | --flag1 | --flag2 | ...                                              */       \
            /* Parses option of form:  [--option=R1 | --option=R2 | ... | --flag1 | --flag2 | ...]                                             */       \
            /*                                                                                                                                 */       \
            /* 'delim' are for delimiters of `_list' type options                                                                              */       \
            /* 'usr_literals' (collectively) are for if have a `usrdef' or `usrdef_list' arg present                                           */       \
            /* 'any_literals' (collectively) are for if have a `anystr' or `anystr_list' arg present                                           */       \
            /*                                                                                                                                 */       \
            /* R_i == nicta::util::cmdline:: { flag ; boolean ; signed_int ; unsigned_int ; numeric ; usrdef ; anystr ;                      } */       \
            /*                               { binary ; octal ; hexadecimal ; binary_list ; octal_list ; hexadecimal_list                    } */       \
            /*                               { boolean_list ; signed_int_list ; unsigned_int_list ; numeric_list ; usrdef_list ; anystr_list } */       \
            /*                                                                                                                                 */       \
            /* NOTE:  each R_i corresponds to arg_i                                                                                            */       \
            /*        (Eg. template.param R2 corresponds to arg2 ..which means that arg2 will be one of the listed R_i, aka R2) !!             */       \
            /*                                                                                                                                 */       \
            /* NOTE:  call of opt_xor{n} needs **at least** THREE template.params...                                                           */       \
            /*         - R_i:  2 or more (obviously for OR-like options, we need 2 or more, hence the `n' in opt_xor{n})                       */       \
            /*         - U  :  1 of either:-  { nicta::util::cmdline::optional  -or-  nicta::util::cmdline::required }                         */       \
            /*                                                                                                                                 */       \
            /* NOTE:  R_i will *collectively* be either of  { nicta::util::cmdline::optional  -or-  nicta::util::cmdline::required }           */       \
            /*                                                                                                                                 */       \
                                                                                                                                                        \
            typedef boost::mpl::vector<BOOST_PP_ENUM_PARAMS(n, R)> vtype;                                                                               \
                                                                                                                                                        \
            /*nicta::util::cmdline::opt_xor ## n<BOOST_PP_ENUM_PARAMS(n, R), U>::ftor_ f(BOOST_PP_ENUM_PARAMS(n, arg));*/                               \
            ftor_ f(BOOST_PP_ENUM_PARAMS(n, arg));                                                                                                      \
                                                                                                                                                        \
            boost::mpl::for_each<vtype>(boost::bind(boost::ref(f),                                                                                      \
                                                    ::_1,                                                                                               \
                                                    boost::ref(m),                                                                                      \
                                                    boost::ref(delim),                                                                                  \
                                                    boost::ref(usr_literals),                                                                           \
                                                    boost::ref(any_literals)));                                                                         \
            return f.full_match_(m.usage());                                                                                                            \
        }                                                                                                                                               \
                                                                                                                                                        \
                                                                                                                                                        \
    private:                                                                                                                                            \
                                                                                                                                                        \
        struct ftor_                                                                                                                                    \
        {                                                                                                                                               \
            typedef void result_type;                                                                                                                   \
                                                                                                                                                        \
                                                                                                                                                        \
            ftor_(BOOST_PP_ENUM_PARAMS(n, const char * arg))                                                                                            \
            : cm_(nicta::util::cmdline::absent_)                                                                                                        \
            , arg_({BOOST_PP_ENUM_PARAMS(n, arg)})                                                                                                      \
            , it_(this->arg_.begin())                                                                                                                   \
            {                                                                                                                                           \
                /* Constructor */                                                                                                                       \
            }                                                                                                                                           \
                                                                                                                                                        \
                                                                                                                                                        \
            template <typename T>                                                                                                                       \
            typename boost::enable_if< boost::mpl::or_< boost::mpl::or_< typename boost::is_same<T, nicta::util::cmdline::flag>::type                   \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::boolean>::type                \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::binary>::type                 \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::octal>::type                  \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::hexadecimal>::type            \
                                                                       >                                                                                \
                                                      , boost::mpl::or_< typename boost::is_same<T, nicta::util::cmdline::signed_int>::type             \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::unsigned_int>::type           \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::numeric>::type                \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::alpha>::type                  \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::alphanum>::type               \
                                                                       >                                                                                \
                                                      >                                                                                                 \
                                     , result_type                                                                                                      \
                                     >::type                                                                                                            \
             operator()(T &,                           /* necessary tag-forwarding, so can communicate the type `T' from  boost::mpl::for_each */       \
                        nicta::util::cmdline::opt_map& m,                                                                                               \
                        const char*& delimiters,                                                                                                        \
                        const char*& usr_literals,                                                                                                      \
                        const char*& any_literals                                                                                                       \
                       ) throw (nicta::nicta_exception)                                                                                                 \
            {                                                                                                                                           \
                if (this->it_ == this->arg_.end())                                                                                                      \
                {                                                                                                                                       \
                    throw nicta::nicta_exception("container index out of bounds", "op()", BOOST_PP_STRINGIZE(opt_xor ## n));                            \
                }                                                                                                                                       \
                nicta::util::cmdline::ContainerModel cm = nicta::util::cmdline::opt<T, nicta::util::cmdline::optional>::parse(m, *this->it_);           \
                                                                                                                                                        \
                if (cm != nicta::util::cmdline::absent_)                                                                                                \
                {                                                                                                                                       \
                    if (this->cm_ == nicta::util::cmdline::absent_)                                                                                     \
                    {                                                                                                                                   \
                        /* we've found an option (or flag) */                                                                                           \
                                                                                                                                                        \
                        this->cm_     = cm;                                                                                                             \
                        this->option_ = std::string(*this->it_);                                                                                        \
                    }                                                                                                                                   \
                    else                                                                                                                                \
                    {                                                                                                                                   \
                        /* more than one option (or flag) was found! */                                                                                 \
                                                                                                                                                        \
                        throw nicta::nicta_exception(m.usage(), "op()", BOOST_PP_STRINGIZE(opt_xor ## n));                                              \
                    }                                                                                                                                   \
                }                                                                                                                                       \
                ++this->it_;                                                                                                                            \
            }                                                                                                                                           \
                                                                                                                                                        \
                                                                                                                                                        \
            template <typename T>                                                                                                                       \
            typename boost::enable_if< boost::mpl::or_< boost::mpl::or_< typename boost::is_same<T, nicta::util::cmdline::boolean_list>::type           \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::binary_list>::type            \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::octal_list>::type             \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::hexadecimal_list>::type       \
                                                                       >                                                                                \
                                                      , boost::mpl::or_< typename boost::is_same<T, nicta::util::cmdline::signed_int_list>::type        \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::unsigned_int_list>::type      \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::numeric_list>::type           \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::alpha_list>::type             \
                                                                       , typename boost::is_same<T, nicta::util::cmdline::alphanum_list>::type          \
                                                                       >                                                                                \
                                                      >                                                                                                 \
                                     , result_type                                                                                                      \
                                     >::type                                                                                                            \
             operator()(T &,                           /* necessary tag-forwarding, so can communicate the type `T' from  boost::mpl::for_each */       \
                        nicta::util::cmdline::opt_map& m,                                                                                               \
                        const char*& delimiters,                                                                                                        \
                        const char*& usr_literals,                                                                                                      \
                        const char*& any_literals                                                                                                       \
                       ) throw (nicta::nicta_exception)                                                                                                 \
            {                                                                                                                                           \
                if (this->it_ == this->arg_.end())                                                                                                      \
                {                                                                                                                                       \
                    throw nicta::nicta_exception("container index out of bounds", "op()", BOOST_PP_STRINGIZE(opt_xor ## n));                            \
                }                                                                                                                                       \
                nicta::util::cmdline::ContainerModel cm = nicta::util::cmdline::opt<T, nicta::util::cmdline::optional>::parse(m,                        \
                                                                                                                              *this->it_,               \
                                                                                                                              delimiters);              \
                                                                                                                                                        \
                if (cm != nicta::util::cmdline::absent_)                                                                                                \
                {                                                                                                                                       \
                    if (this->cm_ == nicta::util::cmdline::absent_)                                                                                     \
                    {                                                                                                                                   \
                        /* we've found an option (or flag) */                                                                                           \
                                                                                                                                                        \
                        this->cm_     = cm;                                                                                                             \
                        this->option_ = std::string(*this->it_);                                                                                        \
                    }                                                                                                                                   \
                    else                                                                                                                                \
                    {                                                                                                                                   \
                        /* more than one option (or flag) was found! */                                                                                 \
                                                                                                                                                        \
                        throw nicta::nicta_exception(m.usage(), "op()", BOOST_PP_STRINGIZE(opt_xor ## n));                                              \
                    }                                                                                                                                   \
                }                                                                                                                                       \
                ++this->it_;                                                                                                                            \
            }                                                                                                                                           \
                                                                                                                                                        \
                                                                                                                                                        \
            template <typename T>                                                                                                                       \
            typename boost::enable_if< boost::mpl::or_< typename boost::is_same<T, nicta::util::cmdline::usrdef>::type                                  \
                                                      , typename boost::is_same<T, nicta::util::cmdline::anystr>::type                                  \
                                                      >                                                                                                 \
                                     , result_type                                                                                                      \
                                     >::type                                                                                                            \
             operator()(T &,                           /* necessary tag-forwarding, so can communicate the type `T' from  boost::mpl::for_each */       \
                        nicta::util::cmdline::opt_map& m,                                                                                               \
                        const char*& delimiters,                                                                                                        \
                        const char*& usr_literals,                                                                                                      \
                        const char*& any_literals                                                                                                       \
                       ) throw (nicta::nicta_exception)                                                                                                 \
            {                                                                                                                                           \
                if (this->it_ == this->arg_.end())                                                                                                      \
                {                                                                                                                                       \
                    throw nicta::nicta_exception("container index out of bounds", "op()", BOOST_PP_STRINGIZE(opt_xor ## n));                            \
                }                                                                                                                                       \
                nicta::util::cmdline::ContainerModel cm;                                                                                                \
                if (boost::is_same<T, nicta::util::cmdline::usrdef>::type::value)                                                                       \
                {                                                                                                                                       \
                    cm = nicta::util::cmdline::opt<T, nicta::util::cmdline::optional>::parse(m,                                                         \
                                                                                             *this->it_,                                                \
                                                                                             delimiters,                                                \
                                                                                             usr_literals);                                             \
                }                                                                                                                                       \
                else                                                                                                                                    \
                {                                                                                                                                       \
                    cm = nicta::util::cmdline::opt<T, nicta::util::cmdline::optional>::parse(m,                                                         \
                                                                                             *this->it_,                                                \
                                                                                             delimiters,                                                \
                                                                                             any_literals);                                             \
                }                                                                                                                                       \
                                                                                                                                                        \
                if (cm != nicta::util::cmdline::absent_)                                                                                                \
                {                                                                                                                                       \
                    if (this->cm_ == nicta::util::cmdline::absent_)                                                                                     \
                    {                                                                                                                                   \
                        /* we've found an option (or flag) */                                                                                           \
                                                                                                                                                        \
                        this->cm_     = cm;                                                                                                             \
                        this->option_ = std::string(*this->it_);                                                                                        \
                    }                                                                                                                                   \
                    else                                                                                                                                \
                    {                                                                                                                                   \
                        /* more than one option (or flag) was found! */                                                                                 \
                                                                                                                                                        \
                        throw nicta::nicta_exception(m.usage(), "op()", BOOST_PP_STRINGIZE(opt_xor ## n));                                              \
                    }                                                                                                                                   \
                }                                                                                                                                       \
                ++this->it_;                                                                                                                            \
            }                                                                                                                                           \
                                                                                                                                                        \
                                                                                                                                                        \
            template <typename T>                                                                                                                       \
            typename boost::enable_if< boost::mpl::or_< typename boost::is_same<T, nicta::util::cmdline::usrdef_list>::type                             \
                                                      , typename boost::is_same<T, nicta::util::cmdline::anystr_list>::type                             \
                                                      >                                                                                                 \
                                     , result_type                                                                                                      \
                                     >::type                                                                                                            \
             operator()(T &,                           /* necessary tag-forwarding, so can communicate the type `T' from  boost::mpl::for_each */       \
                        nicta::util::cmdline::opt_map& m,                                                                                               \
                        const char*& delimiters,                                                                                                        \
                        const char*& usr_literals,                                                                                                      \
                        const char*& any_literals                                                                                                       \
                       ) throw (nicta::nicta_exception)                                                                                                 \
            {                                                                                                                                           \
                if (this->it_ == this->arg_.end())                                                                                                      \
                {                                                                                                                                       \
                    throw nicta::nicta_exception("container index out of bounds", "op()", BOOST_PP_STRINGIZE(opt_xor ## n));                            \
                }                                                                                                                                       \
                nicta::util::cmdline::ContainerModel cm;                                                                                                \
                if (boost::is_same<T, nicta::util::cmdline::usrdef_list>::type::value)                                                                  \
                {                                                                                                                                       \
                    cm = nicta::util::cmdline::opt<T, nicta::util::cmdline::optional>::parse(m,                                                         \
                                                                                             *this->it_,                                                \
                                                                                             delimiters,                                                \
                                                                                             usr_literals);                                             \
                }                                                                                                                                       \
                else                                                                                                                                    \
                {                                                                                                                                       \
                    cm = nicta::util::cmdline::opt<T, nicta::util::cmdline::optional>::parse(m,                                                         \
                                                                                             *this->it_,                                                \
                                                                                             delimiters,                                                \
                                                                                             any_literals);                                             \
                }                                                                                                                                       \
                                                                                                                                                        \
                if (cm != nicta::util::cmdline::absent_)                                                                                                \
                {                                                                                                                                       \
                    if (this->cm_ == nicta::util::cmdline::absent_)                                                                                     \
                    {                                                                                                                                   \
                        /* we've found an option (or flag) */                                                                                           \
                                                                                                                                                        \
                        this->cm_     = cm;                                                                                                             \
                        this->option_ = std::string(*this->it_);                                                                                        \
                    }                                                                                                                                   \
                    else                                                                                                                                \
                    {                                                                                                                                   \
                        /* more than one option (or flag) was found! */                                                                                 \
                                                                                                                                                        \
                        throw nicta::nicta_exception(m.usage(), "op()", BOOST_PP_STRINGIZE(opt_xor ## n));                                              \
                    }                                                                                                                                   \
                }                                                                                                                                       \
                ++this->it_;                                                                                                                            \
            }                                                                                                                                           \
                                                                                                                                                        \
                                                                                                                                                        \
            nicta::util::cmdline::xor_result                                                                                                            \
            full_match_(const std::string& usage) const throw (nicta::nicta_exception)                                                                  \
            {                                                                                                                                           \
                /* Return the match as:  pair<ContainerModel, "--option_name"> */                                                                       \
                                                                                                                                                        \
                if (boost::is_same<U, nicta::util::cmdline::required>::value && this->cm_ == nicta::util::cmdline::absent_)                             \
                {                                                                                                                                       \
                    throw nicta::nicta_exception(usage, "full_match()", BOOST_PP_STRINGIZE(opt_xor ## n));                                              \
                }                                                                                                                                       \
                return std::make_pair(this->cm_, this->option_);                                                                                        \
            }                                                                                                                                           \
                                                                                                                                                        \
                                                                                                                                                        \
        private:                                                                                                                                        \
                                                                                                                                                        \
            nicta::util::cmdline::ContainerModel cm_;                                                                                                   \
            std::string option_;                                                                                                                        \
                                                                                                                                                        \
            std::vector<const char *> arg_;                                                                                                             \
            std::vector<const char *>::const_iterator it_;                                                                                              \
        };                                                                                                                                              \
                                                                                                                                                        \
                                                                                                                                                        \
        BOOST_PP_REPEAT(n, TMP_PARAM_CHECK, R)                                                                                                          \
    };
    /**/


#define BOOST_PP_LOCAL_MACRO(n) OPERATOR(_, n, _)
#define BOOST_PP_LOCAL_LIMITS (2, NICTA_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
#undef OPERATOR


#undef TMP_PARAM_CHECK


} } } // namespace nicta::util::cmdline


#endif
