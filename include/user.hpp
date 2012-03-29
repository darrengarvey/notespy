
#ifndef NOTESPY_USER_HPP_INCLUDED_20090716
#define NOTESPY_USER_HPP_INCLUDED_20090716
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

#include "name.hpp"
#include "database.hpp"
#include "detail/util.hpp"

namespace lotus { namespace notes {

  class user
  {
  public:

    /// Get the current user's name.
    name get_name() {
      return LNGetSession()->GetUserName();
    }

    /// Get the current user's mail server.
    name get_mail_server() {
      return LNGetSession()->GetMailServer();
    }
    
    /// Get the current user's mail file (which is a database).
    database get_mail_db()
    {
      database db;
      detail::throw_error(
        LNGetSession()->GetMailDatabase(&db.native()));
      return db;
    }
  };

} // namespace notes
} // namespace lotus

#endif // NOTESPY_USER_HPP_INCLUDED_20090716

