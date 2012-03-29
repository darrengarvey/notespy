
#ifndef NOTESPY_SESSION_HPP_INCLUDED_20090716
#define NOTESPY_SESSION_HPP_INCLUDED_20090716
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

  /// A Notes session.
  /**
   * This must be initialised before any other object.
   */
  class session
    : boost::noncopyable
  {
  public:
    typedef LNNotesSession native_type;
    
    session()
    {
      // Initialize the Notes API.
      if (session_.Init())
        throw std::runtime_error("Unable to initialize the Notes API");
    }
    
    session(const char* id_file)
    {
      // Initialize the Notes API.
      detail::throw_error(session_.Init(id_file));
    }
    
    ~session() { session_.Term(); }
    
    const char* mail_server() { return session_.GetMailServer(); }
    const char* data_dir() { return session_.GetDataDirectory(); }
    const char* user_name() { return session_.GetUserName(); }
    
    bool replicate(const char* server_name) {
      return !session_.Replicate(server_name);
    }
    
    std::string execute(const char* server, const char* cmd) {
      LNString response;
      if (!session_.ExecuteServer(server, cmd, &response))
        return (const char*)response;
      else
        return "<FailedToExecuteOnServer>";
    }

    native_type& native() { return session_; }
  private:
      native_type session_;
  };


} // namespace notes
} // namespace lotus

#endif // NOTESPY_SESSION_HPP_INCLUDED_20090716

