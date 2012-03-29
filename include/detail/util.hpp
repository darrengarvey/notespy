
#ifndef NOTESPY_UTIL_HPP_INCLUDED_20090716
#define NOTESPY_UTIL_HPP_INCLUDED_20090716
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


namespace lotus { namespace notes { namespace detail {

  inline void throw_error(LNSTATUS status) {
    if (LNIsError(status))
        throw status;
  }

  inline void throw_warning(LNSTATUS status) {
    if (LNIsError(status) || LNIsWarning(status))
        throw status;
  }

} // namespace detail
} // namespace notes
} // namespace lotus

#endif // NOTESPY_UTIL_HPP_INCLUDED_20090716

