
#ifndef NOTESPY_DOCUMENT_HPP_INCLUDED_20090716
#define NOTESPY_DOCUMENT_HPP_INCLUDED_20090716
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
#include "note.hpp"
#include "detail/util.hpp"

namespace lotus { namespace notes {

  class document
    : public note<LNDocument>
  {
  public:
    document() { }

    /// Create a new document.
    /**
     * @param db The database to create the document into.
     * @param form The form used for the document.
     *
     * Note: You still have to save the document in order for it
     * to appear in the database.
     * @see save()
     */
    document(database& db, const char* form = "")
    {
      detail::throw_error(
        form == "" ?
            db.native().CreateDocument(note_.get()) :
            db.native().CreateDocument(note_.get(), form));
    }

    void remove()
    {
      detail::throw_error(note_->GetDatabase().DeleteDocument(note_.get()));
    }

    bool is_response() { return note_->IsResponse(); }

    /// Send the note.
    /**
     * The note must have at least a SendTo field specified for this to
     * work. Use send_to to set the recipients to send this to.
     */
    void send(bool store_form = false)
    {
      detail::throw_error(note_->Send(store_form));
    }

    std::string get_form() { 
      LNString form_name;
      detail::throw_error(note_->GetForm(&form_name));
      return std::string(form_name);
    }
    void set_form(std::string const& new_form) {
      note_->SetForm(LNString(new_form.c_str())); }
  };


} // namespace notes
} // namespace lotus

#endif // NOTESPY_DOCUMENT_HPP_INCLUDED_20090716

