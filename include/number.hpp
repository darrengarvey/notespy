
#ifndef NOTESPY_NUMBER_HPP_INCLUDED_20090716
#define NOTESPY_NUMBER_HPP_INCLUDED_20090716
//
// Copyright (c) Darren Garvey 2009.
//
// A python interface for Lotus Notes.
//

#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/enum.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/exception_translator.hpp>
#include <iostream>
#include <string>
#include <lncppapi.h>
#include <boost/shared_ptr.hpp>


namespace lotus { namespace notes {

    class number
    {
    public:
      typedef LNNUMBER native_type;

      /// IS THIS NECESSARY?

      native_type& native() { return num_; }

    private:
      native_type num_;
    };

} // namespace notes
} // namespace lotus

#endif // NOTESPY_NUMBER_HPP_INCLUDED_20090716

