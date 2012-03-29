
#ifndef NOTESPY_DATABASE_HPP_INCLUDED_20090716
#define NOTESPY_DATABASE_HPP_INCLUDED_20090716
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

#include "datetime.hpp"
#include "unid.hpp"

namespace lotus { namespace notes {

  class view;
  class document;
  class document_collection;
  class agent_collection;

  /// A Notes database.
  /**
   * This class is essentially a wrapper around the C++ API class
   * `LNDatabase`. The main purpose of this is to provide an interface
   * that maps to Python quite nicely.
   */
  class database
  {
  public:
    typedef LNDatabase native_type;

    database();
    database(std::string const& path, std::string const& server = "");

    ~database();
    
    /// Open the database given the `path` in the data directory.
    bool open(std::string const& path, std::string const& server = "");
    
    /// Close the database.
    bool close();
    
    unsigned get_size();
    std::string get_server();
    std::string get_path();
    std::string get_filename();
    double get_percentage_used();
    
    document_collection search(std::string const& search_string);
    document_collection get_documents();
    agent_collection get_agents();

    // The below causes Notes to crash. Something like this happens:
    //   Thread=[1594:0002-111C]
    //   Stack base=0x0021EA28, Stack size = -1688 bytes
    //   PANIC: LookupHandle: handle out of range
    //document get(unid id);
    
    bool is_open();
    bool is_local();
    bool is_remote();
    bool is_ft_indexed();
    void update_ft_index();
    datetime last_modified();
    datetime last_ft_indexed();

    const char* inherits_from();
    const char* port() const;
    //const char* get_type() { return db_.GetType(); }
    
    std::string get_title();
    void        set_title(std::string const& t);

    const char* get_template_name();
    void        set_template_name(const char* t);

    bool get_hide_design();
    void set_hide_design(bool flags);
    
    std::string get_replica_id();
    
    /// Get a view by name and open it if found.
    view get_view(const char* view_name);
    
    bool copy(const char* new_path, const char* new_server = "");

    native_type& native() { return *db_; }

  private:
    boost::shared_ptr<native_type> db_;
  };

} // namespace notes
} // namespace lotus

#endif // NOTESPY_DATABASE_HPP_INCLUDED_20090716

