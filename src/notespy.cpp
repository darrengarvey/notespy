// Copyright (c) Darren Garvey 2009.
//
// A python interface for Lotus Notes.
//

#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/enum.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/operators.hpp>
//#include <boost/python/str.hpp>
#include <boost/python/exception_translator.hpp>
#include <boost/python/return_arg.hpp>
#include <iostream>
#include <string>
#include <lncppapi.h>
#include <boost/shared_ptr.hpp>

#include "datetime.hpp"
#include "item.hpp"
#include "unid.hpp"
#include "session.hpp"
#include "view.hpp"
#include "view_match.hpp"
#include "database.hpp"
#include "document.hpp"
#include "user.hpp"
#include "formula.hpp"
#include "document_collection.hpp"
#include "agent.hpp"
#include "agent_collection.hpp"

//namespace { // Avoid cluttering the global namespace.

namespace lotus { namespace notes {

# define ERR_BUF_SIZE 512                // Error message string buffer size

  /// The string type used by the Notes C++ API.
  typedef LNString string;

  /// Pass-through wrapper.
  inline boost::python::object
  pass_through(boost::python::object& o) { return o; }

  /// Handle exceptions from the Notes API.
  /**
   * See `register_exception_handler<LNSTATUS>` for when this
   * function is set as a custom exception handler.
   */
  void exception_translator(LNSTATUS const& noteserror)
  {
    switch (noteserror)
    {
    case LNWARN_NOT_FOUND:
      PyErr_SetString(PyExc_StopIteration, "No more entries.");
      break;
    default:
      char ErrorBuf[ERR_BUF_SIZE];
      LNGetErrorMessage(noteserror, ErrorBuf, ERR_BUF_SIZE);
      // Use the Python 'C' API to set up an exception object
      PyErr_SetString(PyExc_RuntimeError, ErrorBuf);
    }
  }

  std::ostream& operator<<(std::ostream& os,datetime dt)
  {
    os<< dt.get_text();
    return os;
  }
 
  /*************** PYTHON RELATED BITS FROM HERE ON ***************/


  /// Set up function pointers for overloads of (member) functions.
  /**
   * These function pointers are needed to set up the overloads in the
   * boost::python::class_<> instances below.
   *
   * They all end up in the lotus::notes namespace.
   */

  view_match
    (lotus::notes::view::*find_keys)
      (boost::python::object)
        = &lotus::notes::view::find;

  view_match
    (lotus::notes::view::*find_key)
      (const char*)
        = &lotus::notes::view::find;

  void
    (lotus::notes::document::*doc_set_str)
      (std::string const&, std::string const&)
        = &lotus::notes::document::set;
  void
    (lotus::notes::document::*doc_set_float)
      (std::string const&, float)
        = &lotus::notes::document::set;
  void
    (lotus::notes::document::*doc_set_datetime)
      (std::string const&, datetime&)
         = &lotus::notes::document::set;

 BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(database_open_member_overloads, open, 1, 2)
 BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(database_copy_member_overloads, copy, 1, 2)
 BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(document_send_member_overloads, send, 0, 1)
 BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(view_open_member_overloads, open, 0, 3)

} // namespace notes
} // namespace lotus


BOOST_PYTHON_MODULE(notes)
{
    using namespace boost::python;
    namespace notes = lotus::notes;
    
    // Errors are checked by this wrapper library, so don't throw from
    // the Notes API.
    LNSetThrowAllErrors (FALSE);

    // Make sure we handle exceptions from the Notes API and make them 
    // readable in Python.
    register_exception_translator<LNSTATUS>(notes::exception_translator);

    // Set up a python class for a Notes session.
    class_<notes::session, boost::noncopyable>("Session", no_init)
        .def(init<>())
        .def(init<const char*>())
        .def("replicate", &notes::session::replicate)
        .def("execute", &notes::session::execute)
        .add_property("user_name", &notes::session::user_name)
        .add_property("mail_server", &notes::session::mail_server)
        .add_property("data_dir", &notes::session::data_dir)
        ;

    // Set up a python class for a Notes database.
    class_<notes::database>("Database")
        .def(init<std::string const&, optional<std::string const&> >())
        .def("open", &notes::database::open
                   , notes::database_open_member_overloads())
        .def("close", &notes::database::close)
        //.def("get", &notes::database::get)
        .def("search", &notes::database::search)
        .def("get_view", &notes::database::get_view)
        .def("is_open", &notes::database::is_open)
        .def("is_ft_indexed", &notes::database::is_ft_indexed)
        .def("is_local", &notes::database::is_local)
        .def("is_remote", &notes::database::is_remote)
        .def("update_ft_index", &notes::database::update_ft_index)
        .def("copy", &notes::database::copy
                   , notes::database_copy_member_overloads())
        .add_property("docs", &notes::database::get_documents)
        .add_property("agents", &notes::database::get_agents)
        .add_property("used", &notes::database::get_percentage_used)
        .add_property("path", &notes::database::get_path)
        .add_property("filename", &notes::database::get_filename)
        .add_property("size", &notes::database::get_size)
        .add_property("server", &notes::database::get_server)
        .add_property("replica_id", &notes::database::get_replica_id)
        .add_property("inherits_from", &notes::database::inherits_from)
        .add_property("port", &notes::database::port)
        .add_property("last_modified", &notes::database::last_modified)
        .add_property("last_ft_indexed", &notes::database::last_ft_indexed)
        //.add_property("type", &notes::database::get_type)
        .add_property("title", &notes::database::get_title
                             , &notes::database::set_title)
        .add_property("template_name", &notes::database::get_template_name
                                     , &notes::database::set_template_name)
        .add_property("hide_design", &notes::database::get_hide_design)
        ;
        

    // Set up a python class for a Notes document.
    class_<notes::document>("Document")
        //.def("get", &notes::document::get)
        .def(init<notes::database&, optional<const char*> >
                ( (arg("db"), arg("form")=NULL) ))
        .def("set", notes::doc_set_float)
        .def("set", notes::doc_set_datetime)
        .def("set", notes::doc_set_str)
        .def("get", &notes::document::get_text)
        .def("open", &notes::document::open)
        .def("close", &notes::document::close)
        .def("send", &notes::document::send
                   , notes::document_send_member_overloads())
        .def("save", &notes::document::save)
        .def("remove", &notes::document::remove)
        .def("del_item", &notes::document::del_item)
        .def("is_open", &notes::document::is_open)
        .def("has_item", &notes::document::has_item)
        .def("is_response", &notes::document::is_response)
        .add_property("unid", &notes::document::unid)
        .add_property("size", &notes::document::size)
        .add_property("created", &notes::document::created)
        .add_property("last_accessed", &notes::document::last_accessed)
        .add_property("last_modified", &notes::document::last_modified)
        .add_property("form", &notes::document::get_form
                            , &notes::document::set_form)
        ;

    class_<notes::agent>("Agent")
        .add_property("name", &notes::agent::get_name)
        .def("open", &notes::agent::open)
        .def("close", &notes::agent::close)
        .def("save", &notes::agent::save)
        .def("remove", &notes::agent::remove)
        .def("del_item", &notes::agent::del_item)
        .def("is_open", &notes::agent::is_open)
        .def("has_item", &notes::agent::has_item)
        .add_property("unid", &notes::agent::unid)
        .add_property("size", &notes::agent::size)
        .add_property("alias", &notes::agent::get_alias)
        .add_property("created", &notes::agent::created)
        .add_property("last_accessed", &notes::agent::last_accessed)
        .add_property("last_modified", &notes::agent::last_modified)
        .add_property("enabled", &notes::agent::get_enabled
                               , &notes::agent::set_enabled)
        ;

    class_<notes::agent_collection>("AgentCollection")
        .def("__iter__", &notes::pass_through)
        .def("next", &notes::agent_collection::next)
        .add_property("count", &notes::agent_collection::count)
        ;

    class_<notes::document_collection>("DocCollection")
        .def("__iter__", &notes::pass_through)
        .def("next", &notes::document_collection::next)
        .add_property("count", &notes::document_collection::count)
        ;

    class_<notes::view>("View")
        .def("refresh", &notes::view::refresh)
        .def("open", &notes::view::open
                   , notes::view_open_member_overloads())
        .def("close", &notes::view::close)
        .def("find", notes::find_key)
        .def("find", notes::find_keys)
        .def("first", &notes::view::first)
        .def("next", &notes::view::next)
        .def("__iter__", &notes::pass_through)
        .add_property("count", &notes::view::count)
        .add_property("entries", &notes::view::get_entries)
        .add_property("name", &notes::view::get_name
                             , &notes::view::set_name)
        .add_property("alias", &notes::view::get_alias
                             , &notes::view::set_alias)
        .add_property("cache_size", &notes::view::get_cache_size
                                  , &notes::view::set_cache_size)
        .add_property("comment", &notes::view::get_comment
                                  , &notes::view::set_comment)
        ;

    class_<notes::view_entry>("ViewEntry")
        .def("is_category", &notes::view_entry::is_category)
        .def("is_response", &notes::view_entry::is_response)
        .def("is_main_topic", &notes::view_entry::is_main_topic)
        .def("is_total", &notes::view_entry::is_total)
        .def("is_unread", &notes::view_entry::is_unread)
        .def("is_valid", &notes::view_entry::is_valid)
        .add_property("document", &notes::view_entry::get_document)
        .add_property("child_count", &notes::view_entry::child_count)
        .add_property("sibling_count", &notes::view_entry::sibling_count)
        .add_property("descendent_count", &notes::view_entry::descendent_count)
        .add_property("ft_search_score", &notes::view_entry::ft_search_score)
        .add_property("doc_level", &notes::view_entry::doc_level)
        ;

    class_<notes::view_match>("ViewMatch")
        .def("next", &notes::view_match::next)
        .def("__iter__", &notes::pass_through)
        .def_readonly("count", &notes::view_match::count)
        .add_property("entry", &notes::view_match::entry)
        .add_property("document", &notes::view_match::document)
        ;

    class_<notes::string>("String");

    class_<notes::unid>("Unid", init<>())
        .def(init<const char*>())
        .def("__str__", &notes::unid::str)
        ;
     
    /// Set up a python class for a Notes item.
    class_<notes::item>("Item")
        .def("type", &notes::item::type)
        .def("__str__", &notes::item::get_value)
        .add_property("value", &notes::item::get_value
                             , &notes::item::set_value)
        ;

    /// Set up a python class for a Notes name.
    class_<notes::name>("Name", no_init)
        .def(init<notes::string const&>())
        .def("__str__", &notes::name::get_name)
        .add_property("cn", &notes::name::get_cn)
        .add_property("abbr", &notes::name::get_abbr)
        ;

    /// Set up a python class representing the current user.
    class_<notes::user>("User")
        .add_property("name", &notes::user::get_name)
        .add_property("mail_db", &notes::user::get_mail_db)
        .add_property("mail_server", &notes::user::get_mail_server)
        ;

    class_<notes::formula>("Formula")
        .def(init<std::string const&>())
        .def("eval", &notes::formula::eval)
        .def("__str__", &notes::formula::get_text)
        .add_property("size", &notes::formula::size)
        .add_property("text", &notes::formula::get_text
                            , &notes::formula::set_text)
        ;
     
    class_<notes::datetime>("Datetime", init<std::string const&>())
        .def("__str__", &notes::datetime::get_text)
        .def("adjust", &notes::datetime::adjust
                          , return_self<>()
                          , (arg("hours")=0, arg("mins")=0, arg("secs")=0,
                             arg("hundredths")=0,
                             arg("days")=0, arg("months")=0, arg("years")=0))
        .def("adjust_time", &notes::datetime::adjust_time
			  , return_self<>()
			  , (arg("hour")=0, arg("min")=0, arg("sec")=0, arg("hundredth")=0))
        .def("adjust_date", &notes::datetime::adjust_date
			  , return_self<>()
			  , (arg("year")=0, arg("month")=0, arg("day")=0))
        .add_property("text", &notes::datetime::get_text
                            , &notes::datetime::set_text)
        .add_property("second", &notes::datetime::get_second
                              , &notes::datetime::set_second)
        .add_property("minute", &notes::datetime::get_minute
                              , &notes::datetime::set_minute)
        .add_property("hour", &notes::datetime::get_hour
                            , &notes::datetime::set_hour)
        .add_property("day", &notes::datetime::get_day
                           , &notes::datetime::set_day)
        .add_property("month", &notes::datetime::get_month
                             , &notes::datetime::set_month)
        .add_property("year", &notes::datetime::get_year
                            , &notes::datetime::set_year)
        .add_property("day_of_week", &notes::datetime::day_of_week)
        .add_property("day_of_year", &notes::datetime::day_of_year)
        ;

    enum_<notes::view::open_flags>("view.open_flags")
      .value("default", notes::view::default_)
      .value("rebuild_index", notes::view::rebuild_index)
      .value("no_update", notes::view::no_update)
      .value("no_create", notes::view::no_create)
      .value("use_unread_list", notes::view::use_unread_list)
      .value("design_only", notes::view::design_only)
      ;

    enum_<notes::weekday>("weekday")
      .value("sunday", notes::sunday)
      .value("monday", notes::monday)
      .value("tuesday", notes::tuesday)
      .value("wednesday", notes::wednesday)
      .value("thursday", notes::thursday)
      .value("friday", notes::friday)
      .value("saturday", notes::saturday)
      ;

    enum_<notes::item::types>("item.types")
      .value("any", notes::item::any)
      .value("action", notes::item::action)
      .value("collation", notes::item::collation)
      .value("datetimeranges", notes::item::datetimeranges)
      .value("datetimes", notes::item::datetimes)
      .value("error", notes::item::error)
      .value("fonttable", notes::item::fonttable)
      .value("formula", notes::item::formula)
      .value("highlights", notes::item::highlights)
      .value("html", notes::item::html)
      .value("icon", notes::item::icon)
      .value("lsobject", notes::item::lsobject)
      .value("notelinks", notes::item::notelinks)
      .value("noterefs", notes::item::noterefs)
      .value("numbers", notes::item::numbers)
      .value("object", notes::item::object)
      .value("query", notes::item::query)
      .value("richtext", notes::item::richtext)
      .value("sched_list", notes::item::sched_list)
      .value("seal", notes::item::seal)
      .value("sealdata", notes::item::sealdata)
      .value("seal_list", notes::item::seal_list)
      .value("signature", notes::item::signature)
      .value("text", notes::item::text)
      .value("unavailable", notes::item::unavailable)
      .value("unknown", notes::item::unknown)
      .value("unsupported", notes::item::unsupported)
      .value("userdata", notes::item::userdata)
      .value("userid", notes::item::userid)
      .value("view_format", notes::item::view_format)
      .value("viewmap_dataset", notes::item::viewmap_dataset)
      .value("viewmap_layout", notes::item::viewmap_layout)
      .value("worksheet_data", notes::item::worksheet_data)
      ;

}
