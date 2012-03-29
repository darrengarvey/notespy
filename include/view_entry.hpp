
#ifndef NOTESPY_VIEW_ENTRY_HPP_INCLUDED_20090716
#define NOTESPY_VIEW_ENTRY_HPP_INCLUDED_20090716
//
// Copyright (c) Darren Garvey 2009.
//
// A python interface for Lotus Notes.
//

#include <lncppapi.h>
#include <boost/shared_ptr.hpp>

#include "document.hpp"

namespace lotus { namespace notes {

  class view_entry
  {
  public:
    typedef LNVFEntry native_type;

    view_entry() : entry_(new native_type()), valid_(false) { }

    document get_document() {
      if (doc_.is_open()) return doc_;
      detail::throw_error(entry_->GetDocument(&doc_.native()));
      return doc_;
    }
    
    unsigned child_count() { return entry_->GetChildCount(); }
    unsigned sibling_count() { return entry_->GetSiblingCount(); }
    unsigned descendent_count() { return entry_->GetDescendantCount(); }
    unsigned ft_search_score() { return entry_->GetFTSearchScore(); }

    unsigned doc_level() { return entry_->GetDocLevel(); }
    
    bool is_category() { return entry_->IsCategory(); }
    bool is_response() { return entry_->IsResponse(); }
    bool is_main_topic() { return entry_->IsMainTopic(); }
    bool is_total() { return entry_->IsTotal(); }
    bool is_unread() { return entry_->IsUnread(); }
    bool is_valid() { return valid_; }
      
    native_type& native() { return *entry_; }
  private:
    boost::shared_ptr<native_type> entry_;
    document doc_;
    bool valid_;
  };

} // namespace notes
} // namespace lotus

#endif // NOTESPY_VIEW_ENTRY_HPP_INCLUDED_20090716

