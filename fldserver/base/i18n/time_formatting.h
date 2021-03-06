// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Basic time formatting methods.  These methods use the current locale
// formatting for displaying the time.

#ifndef BASE_I18N_TIME_FORMATTING_H_
#define BASE_I18N_TIME_FORMATTING_H_

#include "fldserver/base/compiler_specific.h"
#include "fldserver/base/i18n/base_i18n_export.h"
#include "fldserver/fldserver_config.h"

#include <string>

#if 0
#include "unicode/timezone.h"
#endif  // 0

namespace base
{
class Time;
class TimeDelta;

// Argument type used to specify the hour clock type.
enum HourClockType
{
    k12HourClock,  // Uses 1-12. e.g., "3:07 PM"
    k24HourClock,  // Uses 0-23. e.g., "15:07"
};

// Argument type used to specify whether or not to include AM/PM sign.
enum AmPmClockType
{
    kDropAmPm,  // Drops AM/PM sign. e.g., "3:07"
    kKeepAmPm,  // Keeps AM/PM sign. e.g., "3:07 PM"
};

// Should match UMeasureFormatWidth in measfmt.h; replicated here to avoid
// requiring third_party/icu dependencies with this file.
enum DurationFormatWidth
{
    DURATION_WIDTH_WIDE,    // "3 hours, 7 minutes"
    DURATION_WIDTH_SHORT,   // "3 hr, 7 min"
    DURATION_WIDTH_NARROW,  // "3h 7m"
    DURATION_WIDTH_NUMERIC  // "3:07"
};

// Date formats from third_party/icu/source/i18n/unicode/udat.h. Add more as
// necessary.
enum DateFormat
{
    // November 2007
    DATE_FORMAT_YEAR_MONTH,
    // Tuesday, 7 November
    DATE_FORMAT_MONTH_WEEKDAY_DAY,
};

// Returns the time of day, e.g., "3:07 PM".
CORE_EXPORT std::u16string
TimeFormatTimeOfDay(const Time& time);

// Returns the time of day in 24-hour clock format with millisecond accuracy,
// e.g., "15:07:30.568"
CORE_EXPORT std::u16string
TimeFormatTimeOfDayWithMilliseconds(const Time& time);

// Returns the time of day in the specified hour clock type. e.g.
// "3:07 PM" (type == k12HourClock, ampm == kKeepAmPm).
// "3:07"    (type == k12HourClock, ampm == kDropAmPm).
// "15:07"   (type == k24HourClock).
CORE_EXPORT std::u16string
TimeFormatTimeOfDayWithHourClockType(const Time& time, HourClockType type, AmPmClockType ampm);

// Returns a shortened date, e.g. "Nov 7, 2007"
CORE_EXPORT std::u16string
TimeFormatShortDate(const Time& time);

// Returns a numeric date such as 12/13/52.
CORE_EXPORT std::u16string
TimeFormatShortDateNumeric(const Time& time);

// Returns a numeric date and time such as "12/13/52 2:44:30 PM".
CORE_EXPORT std::u16string
TimeFormatShortDateAndTime(const Time& time);

#if 0
// Returns a month and year, e.g. "November 2007"
// Note: If `time_zone` is non-null, the time will be formatted in the provided
// time zone. Otherwise, it will default to local time.
CORE_EXPORT std::u16string
TimeFormatMonthAndYear(const Time& time, const icu::TimeZone* time_zone = nullptr);
#else
// Returns a month and year, e.g. "November 2007"
CORE_EXPORT std::u16string
TimeFormatMonthAndYear(const Time& time);
#endif  // 0

// Returns a numeric date and time with time zone such as
// "12/13/52 2:44:30 PM PST".
CORE_EXPORT std::u16string
TimeFormatShortDateAndTimeWithTimeZone(const Time& time);

// Formats a time in a friendly sentence format, e.g.
// "Monday, March 6, 2008 2:44:30 PM".
CORE_EXPORT std::u16string
TimeFormatFriendlyDateAndTime(const Time& time);

// Formats a time in a friendly sentence format, e.g.
// "Monday, March 6, 2008".
CORE_EXPORT std::u16string
TimeFormatFriendlyDate(const Time& time);

// Formats a time using a skeleton to produce a format for different locales
// when an unusual time format is needed, e.g. "Feb. 2, 18:00".
//
// See http://userguide.icu-project.org/formatparse/datetime for details.
CORE_EXPORT std::u16string
TimeFormatWithPattern(const Time& time, const char* pattern);

// Formats a time duration of hours and minutes into various formats, e.g.,
// "3:07" or "3 hours, 7 minutes", and returns true on success. See
// DurationFormatWidth for details.
//
// Please don't use width = DURATION_WIDTH_NUMERIC when the time duration
// can possibly be larger than 24h, as the hour value will be cut below 24
// after formatting.
// TODO(crbug.com/675791): fix function output when width =
// DURATION_WIDTH_NUMERIC.
CORE_EXPORT bool
TimeDurationFormat(const TimeDelta time,
                   const DurationFormatWidth width,
                   std::u16string* out) WARN_UNUSED_RESULT;

// Formats a time duration of hours, minutes and seconds into various formats,
// e.g., "3:07:30" or "3 hours, 7 minutes, 30 seconds", and returns true on
// success. See DurationFormatWidth for details.
//
// Please don't use width = DURATION_WIDTH_NUMERIC when the time duration
// can possibly be larger than 24h, as the hour value will be cut below 24
// after formatting.
// TODO(crbug.com/675791): fix function output when width =
// DURATION_WIDTH_NUMERIC.
CORE_EXPORT bool
TimeDurationFormatWithSeconds(const TimeDelta time,
                              const DurationFormatWidth width,
                              std::u16string* out) WARN_UNUSED_RESULT;

// Formats a date interval into various formats, e.g. "2 December - 4 December"
// or "March 2016 - December 2016". See DateFormat for details.
CORE_EXPORT std::u16string
DateIntervalFormat(const Time& begin_time, const Time& end_time, DateFormat format);

// Gets the hour clock type of the current locale. e.g.
// k12HourClock (en-US).
// k24HourClock (en-GB).
CORE_EXPORT HourClockType
GetHourClockType();

}  // namespace base

#endif  // BASE_I18N_TIME_FORMATTING_H_
