
#ifndef NOTESPY_WEEKDAY_HPP_INCLUDED_20090716
#define NOTESPY_WEEKDAY_HPP_INCLUDED_20090716
//
// Copyright (c) Darren Garvey 2009.
//
// A python interface for Lotus Notes.
//

#include <iostream>
#include <string>
#include <lncppapi.h>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include "detail/util.hpp"

namespace lotus { namespace notes {

  enum weekday {
      sunday = LNDAYOFWEEK_SUNDAY
    , monday = LNDAYOFWEEK_MONDAY
    , tuesday = LNDAYOFWEEK_TUESDAY
    , wednesday = LNDAYOFWEEK_WEDNESDAY
    , thursday = LNDAYOFWEEK_THURSDAY
    , friday = LNDAYOFWEEK_FRIDAY
    , saturday = LNDAYOFWEEK_SATURDAY
  };

  class datetime
  {
  public:
    
    typedef LNDatetime native_type;
    
    datetime() : dt_() {}
    datetime(native_type& other) : dt_(other) {}
    datetime(native_type* other) : dt_(*other) {}
    datetime(std::string const& datestring) {
      if (boost::to_lower_copy(datestring) == "now")
          dt_ = LNGetSession()->GetCurrentDatetime();
      else
        dt_ = datestring.c_str();
    }
    
    void adjust(signed hour, signed minute, signed second, signed hundredth
               , signed day, signed month, signed year)
    {
      detail::throw_error(
        dt_.AdjustDate(LNSINT(month), LNSINT(day), LNSINT(year)) ||
        dt_.AdjustTime(LNSINT(hour), LNSINT(minute)
                      , LNSINT(second), LNSINT(hundredth)));
    }

    void adjust_date(signed year, signed month, signed day)
    {
      detail::throw_error(
        dt_.AdjustDate(LNSINT(month), LNSINT(day), LNSINT(year)));
      //return *this;
    }

    void adjust_time(signed hour, signed minute, signed second
                    , signed hundredth)
    {
      detail::throw_error(
        dt_.AdjustTime(LNSINT(hour), LNSINT(minute)
                      , LNSINT(second), LNSINT(hundredth)));
      //return *this;
    }

    /// sub-second resolution doesn't appear to be supported on Notes v8.0.2.
    //unsigned get_hundreth() { return dt_.GetHundreth(); }
    //void set_hundreth(unsigned new_val) { dt_.SetHundreth(new_val); }
    unsigned get_second() { return dt_.GetSecond(); }
    bool set_second(unsigned new_val) { return !dt_.SetSecond(new_val); }
    unsigned get_minute() { return dt_.GetMinute(); }
    bool set_minute(unsigned new_val) { return !dt_.SetMinute(new_val); }
    unsigned get_hour() { return dt_.GetHour(); }
    bool set_hour(unsigned new_val) { return !dt_.SetHour(new_val); }
    unsigned get_day() { return dt_.GetDay(); }
    bool set_day(unsigned new_val) { return !dt_.SetDay(new_val); }
    unsigned get_month() { return dt_.GetMonth(); }
    bool set_month(unsigned new_val) { return !dt_.SetMonth(new_val); }
    unsigned get_year() { return dt_.GetYear(); }
    bool set_year(unsigned new_val) { return !dt_.SetYear(new_val); }
    
    weekday day_of_week() { return static_cast<weekday>(dt_.GetDayOfWeek()); }
    unsigned day_of_year() { return dt_.GetDayOfYear(); }
    std::string get_text() {
      LNString datetext;
      detail::throw_error(dt_.GetText(&datetext));
      return std::string(datetext);
    }
    void set_text(const char* new_datetime) {
      dt_ = new_datetime;
    }
    
    native_type& native() { return dt_; }
    
  private:
    native_type dt_;
  };
  

} // namespace notes
} // namespace lotus

#endif // NOTESPY_WEEKDAY_HPP_INCLUDED_20090716
