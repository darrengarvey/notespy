
#ifndef NOTESPY_NAME_HPP_INCLUDED_20090716
#define NOTESPY_NAME_HPP_INCLUDED_20090716
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

#include "detail/util.hpp"

namespace lotus { namespace notes {

  class name
  {
  public:
    name(LNString const& name_str)
      : name_(std::string(name_str))
    {
    }

    std::string get_name() {
      return name_;
    }

    std::string get_cn() {
      LNString str;
      detail::throw_error(
        LNGetSession()->CanonicalizeName(name_.c_str(), &str));
      return std::string(str);
    }

    std::string get_abbr() {
      LNString str;
      detail::throw_error(
        LNGetSession()->AbbreviateName(name_.c_str(), &str));
      return std::string(str);
    }

  private:
    std::string name_;
  };

} // namespace notes
} // namespace lotus

#endif // NOTESPY_NAME_HPP_INCLUDED_20090716

