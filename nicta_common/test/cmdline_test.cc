/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 Manfred Doudar, NICTA Ltd.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "cmdline_test.h"
#include "nicta_common/cmdline.h"

#include <map>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(CmdlineTest);


using namespace nicta;


void CmdlineTest :: setUp()
{
}


void CmdlineTest :: tearDown()
{
}


void CmdlineTest :: CommandLine()
{
    int argc = 7;
    char** argv = new char*[argc];
    for (int i = 0; i != argc; ++i)
    {
        argv[i] = new char[30];
    }
    strcpy(argv[0], "./program\0");
    strcpy(argv[1], "--numopt=123\0");
    strcpy(argv[2], "--aflag\0");
    strcpy(argv[3], "--alphaopt=abcd\0");
    strcpy(argv[4], "--numlist=2,23\0");
    strcpy(argv[5], "--alphanumlist=2a,2j3\0");
    strcpy(argv[6], "--opt3=2.5\0");


    std::string usage;
                usage += std::string("--numopt=<num> --alphaopt=<str> --numlist=<num-csv-list> --alphanumlist=<alphanum-csv-list> ");
                usage += std::string("--option=<string>[73,-9] [--aflag] [--optional=<uint>] ");
                usage += std::string("--opt1=<num> | --opt2=<string> | --opt3=<num> | --flag1 | --flag2 ");        // one of these is required
                usage += std::string("[--opt5=<num> | --opt6=<string> | --opt7=<num> | --flag3 | --flag4] ");      // these are optional

    nicta::util::cmdline::opt_map m(argc, argv, usage);

    for (int i = argc; i != 0; --i)
    {
        delete[] argv[i - 1];
    }
    delete[] argv;


    nicta::util::cmdline::opt_<nicta::util::cmdline::numeric>::parse(m, "--numopt");                  // alt: opt<numeric, required>::parse(m, "--numopt");             OR opt_xor1

    nicta::util::cmdline::opt_<nicta::util::cmdline::alphanum>::parse(m, "--alphaopt");               // alt: opt<alphanum, required>::parse(m, "--alphaopt");          OR opt_xor1

    nicta::util::cmdline::opt_<nicta::util::cmdline::numeric_list>::parse(m, "--numlist");            // alt: opt<numeric_list, required>::parse(m, "--numlist");       OR opt_xor1

    nicta::util::cmdline::opt_<nicta::util::cmdline::alphanum_list>::parse(m, "--alphanumlist");      // alt: opt<alphanum_list, required>::parse(m, "--alphanumlist);  OR opt_xor1

    nicta::util::cmdline::opt<nicta::util::cmdline::alphanum, nicta::util::cmdline::numeric_list>::parse(m, "--option", "73,-9");

    nicta::util::cmdline::opt_<nicta::util::cmdline::flag>::parse(m, "--aflag");                      // alt: opt<flag, optional>::parse(m, "--aflag");                 OR opt_xor1

    // if use opt1 instead, will get a return type of pair<ContainerModel, "option-name">
    nicta::util::cmdline::ContainerModel cm = nicta::util::cmdline::opt<nicta::util::cmdline::unsigned_int, nicta::util::cmdline::optional>::parse(m, "--optional");

    // opt_xor{5} b/c 5 possible options
    nicta::util::cmdline::xor_result cmr =
             nicta::util::cmdline::opt_xor5< nicta::util::cmdline::numeric           // --opt1=<num>
                                           , nicta::util::cmdline::alphanum          // --opt2=<string>
                                           , nicta::util::cmdline::numeric           // --opt3=<num>
                                           , nicta::util::cmdline::flag              // --flag1
                                           , nicta::util::cmdline::flag              // --flag2
                                           , nicta::util::cmdline::required          // ...one (and only one) of these options *must* be present
                                           >::parse(m, "--opt1", "--opt2", "--opt3", "--flag1", "--flag2");

    // opt_xor{5} b/c 5 possible options
    nicta::util::cmdline::xor_result cmo =
             nicta::util::cmdline::opt_xor5< nicta::util::cmdline::numeric           // --opt4=<num>
                                           , nicta::util::cmdline::alphanum          // --opt5=<string>
                                           , nicta::util::cmdline::numeric           // --opt6=<num>
                                           , nicta::util::cmdline::flag              // --flag3
                                           , nicta::util::cmdline::flag              // --flag4
                                           , nicta::util::cmdline::optional          // ...one (and only one) of these options *may* be present
                                           >::parse(m, "--opt5", "--opt6", "--opt7", "--flag3", "--flag4");

    bool ret = m.full_match();    // should call this to check we've parsed everything!
    CPPUNIT_ASSERT(ret == true);


    //unsigned int numopt = static_cast<unsigned int> (m.get<nicta::util::cmdline::numeric>("--numopt"));
    unsigned int numopt = m.get<nicta::util::cmdline::numeric, unsigned int>("--numopt");
    nicta::util::cmdline::alphanum::type& alphaopt = m.get<nicta::util::cmdline::alphanum>("--alphaopt");
    nicta::util::cmdline::numeric_list::type& vec1 = m.get<nicta::util::cmdline::numeric_list>("--numlist");
    nicta::util::cmdline::alphanum_list::type& vec2 = m.get<nicta::util::cmdline::alphanum_list>("--alphanumlist");

    // our argv did not contain a --option argument, so we default to a numeric_list of {73, -9}
    nicta::util::cmdline::numeric_list::type& vec3 = m.get<nicta::util::cmdline::numeric_list>("--option");

    // our argv did not contain the option argument [--optional=<uint>]
    CPPUNIT_ASSERT(static_cast<int> (cm) == nicta::util::cmdline::absent_);

    // --opt1=<num> | --opt2=<string> | --opt3=<num> | --flag1 | --flag2
    CPPUNIT_ASSERT(static_cast<int> (cmr.first) == nicta::util::cmdline::numeric_);
    CPPUNIT_ASSERT(std::string(cmr.second) == std::string("--opt3"));
    nicta::util::cmdline::numeric::type& opt3_num = m.get<nicta::util::cmdline::numeric>("--opt3");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, static_cast<double> (opt3_num), 1e-2);

    // [--opt5=<num> | --opt6=<string> | --opt7=<num> | --flag3 | --flag4]           ..none are present
    CPPUNIT_ASSERT(static_cast<int> (cmo.first) == nicta::util::cmdline::absent_);     // absent_ returned as nothing was parsed
    CPPUNIT_ASSERT(cmo.second.empty());                                                // empty, as nothing was parsed


    CPPUNIT_ASSERT(numopt == 123);
    CPPUNIT_ASSERT(alphaopt == "abcd");
    CPPUNIT_ASSERT(vec1[0] == 2);
    CPPUNIT_ASSERT(vec1[1] == 23);
    CPPUNIT_ASSERT(vec2[0] == "2a");
    CPPUNIT_ASSERT(vec2[1] == "2j3");
    CPPUNIT_ASSERT(vec3[0] == 73);
    CPPUNIT_ASSERT(vec3[1] == -9);

    bool aflag   = m.have_flag("--aflag");
    bool nopt    = m.have_key<nicta::util::cmdline::numeric>("--numopt");
    bool aopt    = m.have_key<nicta::util::cmdline::alphanum>("--alphaopt");
    bool nlist   = m.have_key<nicta::util::cmdline::numeric_list>("--numlist");
    bool anlist  = m.have_key<nicta::util::cmdline::alphanum_list>("--alphanumlist");
    bool nnlist  = m.have_key<nicta::util::cmdline::numeric_list>("--option");
    bool nolist  = m.have_key<nicta::util::cmdline::numeric>("--option");
    bool ooption = m.have_key<nicta::util::cmdline::unsigned_int>("--optional");

    CPPUNIT_ASSERT(aflag == true);
    CPPUNIT_ASSERT(nopt == true);
    CPPUNIT_ASSERT(aopt == true);
    CPPUNIT_ASSERT(nlist == true);
    CPPUNIT_ASSERT(anlist == true);
    CPPUNIT_ASSERT(nnlist == true);
    CPPUNIT_ASSERT(nolist == false);
    CPPUNIT_ASSERT(ooption == false);

    bool x1 = m.have_key<nicta::util::cmdline::numeric_list>("--alphanumlist");
    bool x2 = m.have_key<nicta::util::cmdline::numeric>("--alphanumlist");
    bool x3 = m.have_key<nicta::util::cmdline::alphanum>("--alphanumlist");
    bool x4 = m.have_flag("--alphanumlist");
 
    CPPUNIT_ASSERT(x1 == false);
    CPPUNIT_ASSERT(x2 == false);
    CPPUNIT_ASSERT(x3 == false);
    CPPUNIT_ASSERT(x4 == false);
}


void CmdlineTest :: CommandLineHelp()
{
    // NOTE:  we'll do the `cat' manpage as an example

    // use:  '\t'   for indent
    // use:  '\b'   for line-break
    // use:  '\n\n' for paragraph

    int argc = 2;
    char** argv = new char*[argc];
    for (int i = 0; i != argc; ++i)
    {
        argv[i] = new char[30];
    }
    strcpy(argv[0], "/bin/cat\0");
    strcpy(argv[1], "--help\0");


    std::string usage("[OPTION] [FILE]...");


    std::map<std::string, std::string> manpage;
    manpage["NAME"]               = std::string("concatenate files and print on the standard ouput");                // a one line description of the "program"
    manpage["SYNOPSIS"]           = usage;
    manpage["DESCRIPTION"]        = std::string("Concatenate FILE(s), or standard output, to standard output.");     // more detail here
    manpage["--show-all"]         = std::string("equivalent to --show-nonprinting --show-ends --show-tabs");         // listed under heading "OPTIONS"
    manpage["--number-non-blank"] = std::string("number nonblank output lines");                                     // listed under heading "OPTIONS"
    manpage["--e"]                = std::string("equivalent to --show-nonprinting --show-ends");                     // listed under heading "OPTIONS"
    manpage["--show-ends"]        = std::string("display $ at end of each line");                                    // listed under heading "OPTIONS"
    manpage["--number"]           = std::string("number all output lines");                                          // listed under heading "OPTIONS"
    manpage["--squeeze-blank"]    = std::string("never more than one single blank line");                            // listed under heading "OPTIONS"
    manpage["--t"]                = std::string("equivalent to --show-nonprinting --show-tabs");                     // listed under heading "OPTIONS"
    manpage["--show-tabs"]        = std::string("display TAB charachters as ^I");                                    // listed under heading "OPTIONS"
    manpage["--u"]                = std::string("(ignored)");                                                        // listed under heading "OPTIONS"
    manpage["--show-nonprinting"] = std::string("use ^ and M- notation, except for LFD and TAB");                    // listed under heading "OPTIONS"
    manpage["--help"]             = std::string("display this help and exit");                                       // listed under heading "OPTIONS"
    manpage["--version"]          = std::string("output version information and exit");                              // listed under heading "OPTIONS"
    manpage["MISCELLANEOUS"]      = std::string("With no FILE, or when FILE is -, read standard input.");

    std::string ex;
    ex  = std::string("cat f - g\b");
    ex += std::string("\tOutput f\'s contents, then standard input, then g\'s contents.\n\n");
    ex += std::string("cat\b");
    ex += std::string("\tCopy standard input to standard output.");

    manpage["EXAMPLES"]           = ex;
    manpage["AUTHOR"]             = std::string("Written by Torbjorn Granlund and Richard M. Stallman.");
    manpage["REPORTING BUGS"]     = std::string("Report bugs to <bug-coreutils@gnu.org>.");

    std::string cprite;
    cprite  = std::string("Copyright (C) 2006 Free Software Foundation, Inc.\bThis is free software. ");
    cprite += std::string("You may redistribute copies of it under the terms of the GNU General Public License <http://www.gnu.org/licenses/gpl.html>. ");
    cprite += std::string("There is NO WARRANTY, to the extent permitted by law.");

    manpage["COPYRIGHT"]          = cprite;

    std::string calso;
    calso  = std::string("The full documentation for cat is maintained as a Texinfo manual. ");
    calso += std::string("If the info and cat programs are properly installed at your site, the command\n\n");
    calso += std::string("\tinfo cat\n\n");
    calso += std::string("should give you access to the complete manual.");

    manpage["SEE ALSO"]           = calso;


    /* ***************************************************************************************************************************************** */
    //
    // at this point (b/c --help option given on command-line) WILL dump _either_ 'usage' string _or_ 'manpage' if available AND then exit !!
    //
    //nicta::util::cmdline::opt_map m(argc, argv, usage, manpage);
    //
    /* ***************************************************************************************************************************************** */

    for (int i = argc; i != 0; --i)
    {
        delete[] argv[i - 1];
    }
    delete[] argv;
}
