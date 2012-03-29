
#ifndef NOTESPY_FORMULA_HPP_INCLUDED_20090716
#define NOTESPY_FORMULA_HPP_INCLUDED_20090716
//
// Copyright (c) Darren Garvey 2009.
//
// A python interface for Lotus Notes.
//

#include <iostream>
#include <string>
#include <lncppapi.h>

#include "item.hpp"
#include "detail/util.hpp"

namespace lotus { namespace notes {

  class formula
  {
  public:
    typedef LNFormula native_type;

    formula(std::string const& code) : code_(code.c_str()) { }
    formula() : code_() { }

    void set_text(const char* str) {
      LNString fstring(str);
      LNINT error_offset(0);
      LNINT error_length(0);

      LNSTATUS status
        = code_.SetText(fstring, &error_offset, &error_length);
      if (LNIsError(status))
      {
        std::string err("Error compiling Lotus Notes formula: \n\"\"\"\n");
        err += fstring.GetTextPtr();
        err += "\n\"\"\"\nError at: \"";
        err += std::string(fstring.GetTextPtr() + error_offset, error_length);
        err += "\"";
        throw std::runtime_error(err);
      }
    }

    std::string get_text() {
      return std::string(code_.GetText());
    }

    void set_code(std::string const& str) {
      detail::throw_error(
        code_.SetCode(str.c_str(), str.length()));
    }

    std::string get_code() {
      return std::string(code_.GetCode(), code_.GetCodeLength());
    }

    /// Get the size of the compiled code in bytes.
    /**
     * This is useful to check how compact your formula code is.
     */
    unsigned size() {
      return static_cast<unsigned>(code_.GetCodeLength());
    }

    item eval()
    {
        item result;
        detail::throw_error(
            code_.Evaluate(&result.native()));
        return result;
    }

    /*
    item eval()
    {
        item result;
        detail::throw_error(
            code_.Evaluate(&result.native()));
        std::cout<< "Formula compiled ok." << std::endl;
        return result;
    }
    */

    native_type native() { return code_; }

  private:
    native_type code_;
  };

} // namespace notes
} // namespace lotus

#endif // NOTESPY_FORMULA_HPP_INCLUDED_20090716

