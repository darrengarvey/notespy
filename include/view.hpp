
#ifndef NOTESPY_VIEW_HPP_INCLUDED_20090716
#define NOTESPY_VIEW_HPP_INCLUDED_20090716
//
// Copyright (c) Darren Garvey 2009.
//
// A python interface for Lotus Notes.
//

#include <string>
#include <lncppapi.h>
#include <boost/python/object.hpp>
#include <boost/python/list.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/shared_ptr.hpp>

#include "document.hpp"
#include "view_entry.hpp"
#include "view_match.hpp"
//#include "view_navigator.hpp"
#include "detail/util.hpp"

namespace lotus { namespace notes {

  class view
  {
  public:
    typedef LNViewFolder native_type;

    /// The types of flags the view can be opened with.
    enum open_flags
    {
        /**
         * Default options. Do not rebuild index or update collection before
         * opening it, create collection if it doesn't exist, or associate
         * collection with user-specified unread list.
         */
        default_ = LNVFOPENFLAGS_DEFAULT
        // Rebuild index before opening collection.
      , rebuild_index = LNVFOPENFLAGS_REBUILD_INDEX
        // Don't update collection before opening it.
      , no_update = LNVFOPENFLAGS_NO_UPDATE
        /**
         * Don't create collection if it doesn't exist; return an error
         * instead.
         */
      , no_create = LNVFOPENFLAGS_NO_CREATE
        // Associate collection with user-specified unread list.
      , use_unread_list = LNVFOPENFLAGS_USE_UNREAD_LIST
        /**
         * Open view/folder for access to design properties, but don't open
         * collection of entries. If you specify this flag, you will be
         * unable to navigate or search the view or folder or access its
         * entries. Use this flag if you intend only to access or modify the
         * view/folder's design, since opening the collection involves some
         * performance overhead. If you initially open the view/folder with
         * this flag (used implicitly when you create a new view folder), and
         * then decide you want to access the entries in the view/folder, you
         * can simply call LNViewFolder::Open again, but without that flag.
         * (Note that you may need to call Save first if you've made changes
         * to the view/folder design.)
         */
      , design_only = LNVFOPENFLAGS_DESIGN_ONLY 
    };
    
    view() : view_(new native_type()) {}

    /// Destructor.
    /**
     * Don't close the view in the destructor. This is ok, because
     * this class is supposed to be a lightweight wrapper around the
     * LNViewFolder type. It's fine to copy this class around and 
     * won't be very expensive to do.
     */
    ~view() { }

    void open(std::string const& name = ""
             , std::string const& server = ""
             , open_flags flags = default_)
    {
      if (name.empty())
        detail::throw_error(view_->Open());
      else
        detail::throw_error(
            view_->Open(flags, name.c_str(), server.c_str()));
    }
    
    void close() {
      detail::throw_error(view_->Close());
    }

    /// Refresh the view index.
    bool refresh() { return !view_->Refresh(); }
    
    /// Get the number of entries in the view.
    unsigned count() { return view_->GetEntryCount(); }

    
    /// Find a document given a set of keys.
    /**
     * The keys are matched against each column value, in order.
     *
     * An exception will be thrown if the number of keys exceeds the number
     * of indexed columns in the view.
     */
    view_match find(boost::python::object kl)
    {
      boost::python::stl_input_iterator<std::string> iter(kl), end;
      LNVFFindKeys keys;
      for (; iter != end; ++iter) {
        keys.Append(iter->c_str());
      }
      return find_impl(keys);
    }
    
    /// Find a document given a key.
    view_match find(const char* key)
    {
      return find_impl(key);
    }

    /// Get the first entry in the view.
    view_entry first() {
      view_entry entry;
      detail::throw_error(view_->GotoFirst(&entry.native()));
      return entry;
    }

    /// Get the first entry in the view.
    view_entry next() {
      view_entry entry;
      detail::throw_error(view_->GotoNext(&entry.native()));
      return entry;
    }

    view_match get_entries() {
      view_match vm;
      detail::throw_error(view_->GetEntries(&vm.navigator()));
      return vm;
    }

    // Get and set the maximum number of entries that can be in the
    // view/folder cache.
    unsigned get_cache_size() { return view_->GetCacheSize(); }
    void set_cache_size(unsigned max_entries) {
      detail::throw_error(
        view_->SetCacheSize(max_entries));
    }
    
    /// Get and set the comment for the view.
    std::string get_comment() { return std::string(view_->GetComment()); }
    void set_comment(std::string const& cmt)
    {
      detail::throw_error(
        view_->SetComment(cmt.c_str()));
    }

    /// Get and set the view name.
    void set_name(std::string const& new_name) {
      detail::throw_error(view_->SetName(new_name.c_str()));
    }
    const char* get_name() { return view_->GetName(); }
    
    /// Get and set the view alias.
    void set_alias(std::string const& new_alias) {
      detail::throw_error(view_->SetAlias(new_alias.c_str()));
    }
    std::string get_alias() { return std::string(view_->GetAlias()); }
    
    native_type& native() { return *view_; }
  private:

    /// Actually do the work of looking up an entry in a view.
    template<typename KeyType>
    view_match find_impl(KeyType const& key)
    {
      view_match match;
      detail::throw_error(
        view_->Find(key,
                    &match.entry().native(),
                    &match.count,
                    &match.navigator()));
      return match;
    }
    
    boost::shared_ptr<native_type> view_;
  };


} // namespace notes
} // namespace lotus

#endif // NOTESPY_VIEW_HPP_INCLUDED_20090716

