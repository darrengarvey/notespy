
#ifndef NOTESPY_UNID_HPP_INCLUDED_20090716
#define NOTESPY_UNID_HPP_INCLUDED_20090716
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

  class unid
  {
  public:
    typedef LNUniversalID native_type;

    unid() : unid_() { }
    unid(const char* id) : unid_(id) { }
    unid(UNID* id) : unid_(id) { }
    std::string str() { return std::string(unid_.GetText()); }
    bool operator()() { return unid_.IsNull(); }
    
    native_type& native() { return unid_; }
  private:
    native_type unid_;
  };
  
} // namespace notes
} // namespace lotus

#endif // NOTESPY_UNID_HPP_INCLUDED_20090716

