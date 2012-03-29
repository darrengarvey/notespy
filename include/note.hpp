
#ifndef NOTESPY_NOTE_HPP_INCLUDED_20090726
#define NOTESPY_NOTE_HPP_INCLUDED_20090726
//
// Copyright (c) Darren Garvey 2009.
//
// A python interface for Lotus Notes.
//

#include <iostream>
#include <string>
#include <lncppapi.h>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>


#include "unid.hpp"
#include "item.hpp"
#include "datetime.hpp"
#include "database.hpp"
#include "detail/util.hpp"

namespace lotus { namespace notes {

  template<typename NativeType>
  class note
  {
  public:
    typedef NativeType native_type;
    typedef note<native_type> base_type;

    note()
      : note_(new native_type())
    {
    }

    /// Tests whether the note is currently open.
    bool is_open() { return note_->IsOpen(); }

    /// Tests whether this note object is uninitialized.
    /**
     * An uninitialized note object is not associated with any note.
     */
    bool is_null() { return note_->IsNull(); }

    bool has_item(const char* name) { return note_->HasItem(name); }
    void open() { detail::throw_error(note_->Open()); }
    void close() { detail::throw_error(note_->Close()); }
    void save() { detail::throw_error(note_->Save()); }
    std::string get_alias() { return std::string(note_->GetAlias()); }
    int size() { return note_->GetSize(); }

    virtual void remove()
    {
      detail::throw_error(note_->GetDatabase().DeleteNote(note_.get()));
    }

    unid unid() { return note_->GetUniversalID(); }

    void set(std::string const& name, float value)
    {
      LNNumbers item;
      item.SetValue(value);
      detail::throw_error(note_->CreateItem(name.c_str(), item));
    }

    void set(std::string const& name, datetime& value)
    {
      LNDatetimes item;
      item.SetValue(value.native());
      detail::throw_error(note_->CreateItem(name.c_str(), item));
    }

    void set(std::string const& name, std::string const& value)
    {
      LNText item;
      item.SetValue(LNString(value.c_str()));
      detail::throw_error(note_->CreateItem(name.c_str(), item));
    }

    std::string get_text(std::string const& name)
    {
      item i;
      detail::throw_error(note_->GetItem(name.c_str(), &i.native()));
      return i.get_value();
    }

    item get_item(std::string const& name)
    {
      item i;
      note_->GetItem(name.c_str(), &i.native());
      return i;
    }
    
    bool del_item(std::string const& name)
    {
      return note_->DeleteItem(name.c_str());
    }
    
    datetime created() {
      return note_->GetCreated();
    }

    datetime last_accessed() {
      return note_->GetLastAccessed();
    }

    datetime last_modified() {
      return note_->GetLastModified();
    }

    native_type& native() { return *note_; }
    
  protected:
    boost::shared_ptr<native_type> note_;
  };


} // namespace notes
} // namespace lotus

#endif // NOTESPY_NOTE_HPP_INCLUDED_20090726

