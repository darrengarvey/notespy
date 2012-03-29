
#ifndef NOTESPY_AGENT_HPP_INCLUDED_20090716
#define NOTESPY_AGENT_HPP_INCLUDED_20090716
//
// Copyright (c) Darren Garvey 2009.
//
// A python interface for Lotus Notes.
//

#include <string>
#include <lncppapi.h>
#include <boost/shared_ptr.hpp>

#include "note.hpp"
#include "datetime.hpp"

namespace lotus { namespace notes {

  class agent
    : public note<LNAgent>
  {
  public:
    agent() : base_type() { }

    std::string get_name() { return std::string(note_->GetName()); }

    bool get_enabled() { return note_->GetIsEnabled(); }
    void set_enabled(bool flag) { note_->SetIsEnabled(flag); }

    /*
    datetime last_run_time() { 
      datetime dt;
      dt.native() = run_data_.GetRunDatetime();
      return dt;
    }

    std::string run_log() {
      return std::string(run_data_.GetRunLog());
    }

  private:
    LNAgentRunData run_data_;
    */
  };

} // namespace notes
} // namespace lotus

#endif // NOTESPY_AGENT_HPP_INCLUDED_20090716

