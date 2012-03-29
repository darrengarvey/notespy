
#ifndef NOTESPY_DOCUMENT_COLLECTION_HPP_INCLUDED_20090716
#define NOTESPY_DOCUMENT_COLLECTION_HPP_INCLUDED_20090716
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

namespace lotus { namespace notes {

  class document_collection
  {
  public:
    typedef LNDocumentArray native_type;

    document_collection() : col_(new native_type()), pos_(0) { }

    document next() {
      if (pos_ < 0 || pos_ >= col_->GetCount())
        throw LNWARN_NOT_FOUND;
      doc_.native() = (*col_)[pos_++];
      doc_.open();
      return doc_;
    }

    unsigned count() { return col_->GetCount(); }

    native_type& native() { return *col_; }
  private:
    LNINT pos_;
    document doc_;
    boost::shared_ptr<native_type> col_;
  };

} // namespace notes
} // namespace lotus

#endif // NOTESPY_DOCUMENT_COLLECTION_HPP_INCLUDED_20090716

