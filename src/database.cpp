
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

#include "database.hpp"
#include "document.hpp"
#include "formula.hpp"
#include "document_collection.hpp"
#include "agent_collection.hpp"
#include "view.hpp"
#include "detail/util.hpp"

namespace lotus { namespace notes {

  /// A Notes database.
  /**
   * This class is essentially a wrapper around the C++ API class
   * `LNDatabase`. The main purpose of this is to provide an interface
   * that maps to Python quite nicely.
   */
    database::database()
      : db_(new native_type())
    {
      if (LNGetSession() == 0)
        throw std::runtime_error(
          "You must construct a Session before attempting to do anything else.");
    }
    
    database::database(std::string const& path, std::string const& server)
      : db_(new native_type())
    {
      if (LNGetSession() == 0)
        throw std::runtime_error(
          "You must construct a Session before attempting to do anything else.");
      if (!open(path, server))
        throw std::runtime_error(
          "Could not open database: '" + server + "|" + path + "'");
    }

    database::~database()
    {
      try {
          if (db_->IsOpen())
            db_->Close();
      } catch(...) {
        // LNDatabase::Close() can throw.
      }
    }
    
    /// Open the database given the `path` in the data directory.
    bool database::open(std::string const& path, std::string const& server)
    {
      if (path.empty())
        throw std::runtime_error("Empty path");
      detail::throw_error(
          LNGetSession()->GetDatabase(path.c_str(), &native(), server.c_str()));
      detail::throw_error(db_->Open());
      return true;
    }
    
    /// Close the database.
    bool database::close() { return db_->Close() == 0; }
    
    unsigned database::get_size() { return db_->GetSize(); }
    /// Get the name of the server.
    std::string database::get_server() {
      std::string srv(db_->GetServer());
      return srv.empty() ? "Local" : srv;
    }
    std::string database::get_path() {
      return std::string(db_->GetFilepath());
    }
    std::string database::get_filename() {
      return std::string(db_->GetFilename());
    }
    double database::get_percentage_used() {
      return double(db_->GetPercentageUsed());
    }
    
    document_collection database::search(std::string const& search_string)
    {
      document_collection col;
      //formula f(search_string);
      detail::throw_error(db_->Search(search_string.c_str(), &col.native()));
      return col;
    }
    document_collection database::get_documents()
    {
      document_collection col;
      detail::throw_error(db_->GetDocuments(&col.native()));
      return col;
    }
    agent_collection database::get_agents()
    {
      agent_collection col;
      detail::throw_error(db_->GetAgents(&col.native()));
      return col;
    }
    /*
    document database::get(unid id)
    {
      document doc;
      detail::throw_error(
        db_->GetDocument(&id.native(), &doc.native()));
      return doc;
    }
    */
    
    bool database::is_open() { return db_->IsOpen(); }
    bool database::is_local() { return db_->IsLocal(); }
    bool database::is_remote() { return db_->IsRemote(); }
    bool database::is_ft_indexed() { return db_->IsFTIndexed(); }
    void database::update_ft_index() {
      detail::throw_error(db_->UpdateFTIndex());
    }
    datetime database::last_modified() { return db_->GetLastModified(); }
    datetime database::last_ft_indexed() {
      datetime dt;
      detail::throw_error(
        db_->GetLastFTIndexed(&dt.native()));
      return dt;
    }

    const char* database::inherits_from() { return db_->GetInheritsFromTemplateName(); }
    const char* database::port() const { return db_->GetPort(); }
    //const char* get_type() { return db_->GetType(); }
    

    //
    // Get and set the database title.
    //
    std::string database::get_title() {
      return std::string(db_->GetTitle());
    }
    void database::set_title(std::string const& t) {
      detail::throw_error(db_->SetTitle(t.c_str()));
    }

    //
    // Get and set the database template name.
    //
    const char* database::get_template_name() { return db_->GetTemplateName(); }
    void database::set_template_name(const char* t) { 
      detail::throw_error(db_->SetTemplateName(t));
    }

    //
    // Get and set the toggle for hiding the database design.
    // (note: unhiding the design doesn't always seem to work, so the setter
    //        function is commented out. I don't see any reason to use it.
    //
    bool database::get_hide_design() { return db_->GetHideDesign(); }
    //void database::set_hide_design(bool flags) {
    //  detail::throw_error(db_->SetHideDesign(flags));
    //}
    
    std::string database::get_replica_id()
    {
      TIMEDATE* replicaID;
      // Get the replica ID from the database.
      replicaID = db_->GetReplicaID();

      // Convert the replica ID from a TIMEDATE to a string.
      char replicaIDString[18];
      sprintf(replicaIDString, "%08X:%08X", replicaID->Innards[1], replicaID->Innards[0]);
      return replicaIDString;
    }
    
    /// Get a view by name and open it if found.
    view database::get_view(const char* view_name)
    {
      view v;
      detail::throw_error(db_->GetViewFolder(view_name, &v.native()));
      v.open();
      return v;
    }
    
    bool database::copy(const char* new_path, const char* new_server)
    {
      detail::throw_error(
        LNGetSession()->CreateDatabaseCopy(native(), new_path, new_server));
      return true;
    }

} // namespace notes
} // namespace lotus

