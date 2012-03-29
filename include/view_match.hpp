
#ifndef NOTESPY_VIEW_MATCH_HPP_INCLUDED_20090716
#define NOTESPY_VIEW_MATCH_HPP_INCLUDED_20090716
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

#include "document.hpp"
#include "view_entry.hpp"

namespace lotus { namespace notes {

  class view_match
  {
  public:
    typedef LNINT count_type;
    typedef LNVFNavigator navigator_type;

    view_entry entry() { return entry_; }
    document document() { return entry_.get_document(); }
   
    navigator_type& navigator() {
      if (!nav_)
        nav_.reset(new navigator_type());
      return *nav_;
    }

    /// Get the next entry in the view. Starts by returning the first entry.
    view_entry next() {
      detail::throw_warning(nav_->GotoNext(&entry_.native()));
      return entry_;
    }

    count_type count;
  private:
    view_entry entry_;
    boost::shared_ptr<navigator_type> nav_;
  };


} // namespace notes
} // namespace lotus

#endif // NOTESPY_VIEW_MATCH_HPP_INCLUDED_20090716

