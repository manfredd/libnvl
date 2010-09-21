/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2005 Manfred Doudar.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#ifndef NICTA_EXCEPTION_H
#define NICTA_EXCEPTION_H

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>


namespace nicta {

    class nicta_exception : public std::exception
    {
    public:

        virtual ~nicta_exception() throw() { };

        explicit nicta_exception(const char* error);
        nicta_exception(const std::string& error,
                        const std::string& method = "",
                        const std::string& classname = "");

        virtual const char* what() const throw();
        virtual const std::string nice_what() const throw(); 
        virtual const std::string descriptor() const throw();

        virtual void terminate() const throw();

    
    private:

        std::string class_;
        std::string method_;
        std::string descriptor_;
        std::string error_;
    };

} // namespace nicta



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



inline
nicta::nicta_exception :: 
nicta_exception(const char* error)
: error_(error)
{
}


inline
nicta::nicta_exception :: 
nicta_exception(const std::string& error,
                const std::string& method,
                const std::string& classname)
: class_(classname)
, method_(method)
, descriptor_(error)
{
    if (classname.size() > 0)
    {
        this->error_  = classname;
        this->error_ += "::";
        this->error_ += method;
        this->error_ += " : ";
        this->error_ += error;
    }
    else if (method.size() > 0)
    {
        this->error_  = method;
        this->error_ += " : ";
        this->error_ += error;
    }
    else
    {
        this->error_  = error;
    }
}


inline
const char*
nicta::nicta_exception :: 
what() const throw()
{
    return this->error_.c_str();
}


inline
const std::string
nicta::nicta_exception :: 
nice_what() const throw()
{
    // Utility method - return a nice error string

    // Exception Raised.
    // Method:  class::method
    // Descriptor:  error_message

    std::string message = "\n\nException Raised.\nMethod:  ";
    message += this->class_;
    message += " :: ";
    message += this->method_;
    message += "\nDescriptor:  ";
    message += this->descriptor_;
    message += "\n\n";

    return message;
}


inline
const std::string
nicta::nicta_exception :: 
descriptor() const throw()
{
    return this->descriptor_;
}


inline
void
nicta::nicta_exception :: 
terminate() const throw()
{
    // Utility method - push nice error string to stderr and terminate

    std::cerr << this->nice_what();
    exit(1);
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif
