
#ifndef NOTESPY_ITEM_HPP_INCLUDED_20090716
#define NOTESPY_ITEM_HPP_INCLUDED_20090716
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

  class item
  {
  public:
    typedef LNItem native_type;
    enum types;
        
    item() : item_(new LNItem()) {}
    
    types type() { return static_cast<types>(item_->GetType()); }
    
    /*
    std::string val() {
      LNString buf;
      detail::throw_error(item_->GetText(&buf));
      return std::string(buf);
    }
    */
    
    std::string get_value() {
      LNString buf;
      if (type() == text)
        detail::throw_error(LNText(*item_).GetText(&buf));
      else
      if (type() == numbers)
        detail::throw_error(LNNumbers(*item_).GetText(&buf));
      else
      if (type() == datetimes)
        detail::throw_error(LNDatetimes(*item_).GetText(&buf));
      return std::string(buf);
    }
    
    void set_value(std::string const& new_value) {
      if (type() == text)
        detail::throw_error(LNText(*item_).SetValue(new_value.c_str()));
      /*
      else
      if (type() == numbers)
        detail::throw_error(LNNumbers(*item_).SetValue(new_value.c_str()));
      else
      if (type() == datetimes)
        detail::throw_error(LNDatetimes(*item_).SetValue(new_value.c_str()));
        */
    }
    
    native_type& native() { return *item_; }

    enum types {
        any = LNITEMTYPE_ANY
      , action = LNITEMTYPE_ACTION
      , collation = LNITEMTYPE_COLLATION
      , datetimeranges = LNITEMTYPE_DATETIMERANGES //(obsolete, defined for compatibility only)
      , datetimes = LNITEMTYPE_DATETIMES
      , error = LNITEMTYPE_ERROR
      , fonttable = LNITEMTYPE_FONTTABLE
      , formula = LNITEMTYPE_FORMULA
      , highlights = LNITEMTYPE_HIGHLIGHTS
      , html = LNITEMTYPE_HTML
      , icon = LNITEMTYPE_ICON
      , lsobject = LNITEMTYPE_LSOBJECT
      , notelinks = LNITEMTYPE_NOTELINKS
      , noterefs = LNITEMTYPE_NOTEREFS
      , numbers = LNITEMTYPE_NUMBERS
      , object = LNITEMTYPE_OBJECT
      , query = LNITEMTYPE_QUERY
      , richtext = LNITEMTYPE_RICHTEXT
      , sched_list = LNITEMTYPE_SCHED_LIST
      , seal = LNITEMTYPE_SEAL
      , sealdata = LNITEMTYPE_SEALDATA
      , seal_list = LNITEMTYPE_SEAL_LIST
      , signature = LNITEMTYPE_SIGNATURE
      , text = LNITEMTYPE_TEXT
      , unavailable = LNITEMTYPE_UNAVAILABLE
      , unknown = LNITEMTYPE_UNKNOWN
      , unsupported = LNITEMTYPE_UNSUPPORTED
      , userdata = LNITEMTYPE_USERDATA
      , userid = LNITEMTYPE_USERID
      , view_format = LNITEMTYPE_VIEW_FORMAT
      , viewmap_dataset = LNITEMTYPE_VIEWMAP_DATASET
      , viewmap_layout = LNITEMTYPE_VIEWMAP_LAYOUT
      , worksheet_data = LNITEMTYPE_WORKSHEET_DATA
    };


  private:
    boost::shared_ptr<native_type> item_;
  };



} // namespace notes
} // namespace lotus

#endif // NOTESPY_ITEM_HPP_INCLUDED_20090716

