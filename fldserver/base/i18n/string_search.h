// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_STRING_SEARCH_H_
#define BASE_I18N_STRING_SEARCH_H_

#include "fldserver/base/i18n/base_i18n_export.h"

#include <stddef.h>

#include <string>

struct UStringSearch;

namespace base
{
namespace i18n
{
// Returns true if |in_this| contains |find_this|. If |match_index| or
// |match_length| are non-NULL, they are assigned the start position and total
// length of the match.
//
// Only differences between base letters are taken into consideration. Case and
// accent differences are ignored. Please refer to 'primary level' in
// http://userguide.icu-project.org/collation/concepts for additional details.
CORE_EXPORT
bool
StringSearchIgnoringCaseAndAccents(const std::u16string& find_this,
                                   const std::u16string& in_this,
                                   size_t* match_index,
                                   size_t* match_length);

// Returns true if |in_this| contains |find_this|. If |match_index| or
// |match_length| are non-NULL, they are assigned the start position and total
// length of the match.
//
// When |case_sensitive| is false, only differences between base letters are
// taken into consideration. Case and accent differences are ignored.
// Please refer to 'primary level' in
// http://userguide.icu-project.org/collation/concepts for additional details.
// When |forward_search| is true, finds the first instance of |find_this|,
// otherwise finds the last instance
CORE_EXPORT
bool
StringSearch(const std::u16string& find_this,
             const std::u16string& in_this,
             size_t* match_index,
             size_t* match_length,
             bool case_sensitive,
             bool forward_search);

// This class is for speeding up multiple StringSearch()
// with the same |find_this| argument. |find_this| is passed as the constructor
// argument, and precomputation for searching is done only at that time.
class CORE_EXPORT FixedPatternStringSearch
{
public:
    explicit FixedPatternStringSearch(const std::u16string& find_this, bool case_sensitive);
    ~FixedPatternStringSearch();

    // Returns true if |in_this| contains |find_this|. If |match_index| or
    // |match_length| are non-NULL, they are assigned the start position and total
    // length of the match.
    bool
    Search(const std::u16string& in_this,
           size_t* match_index,
           size_t* match_length,
           bool forward_search);

private:
    std::u16string find_this_;
    UStringSearch* search_;
};

// This class is for speeding up multiple StringSearchIgnoringCaseAndAccents()
// with the same |find_this| argument. |find_this| is passed as the constructor
// argument, and precomputation for searching is done only at that time.
class CORE_EXPORT FixedPatternStringSearchIgnoringCaseAndAccents
{
public:
    explicit FixedPatternStringSearchIgnoringCaseAndAccents(const std::u16string& find_this);

    // Returns true if |in_this| contains |find_this|. If |match_index| or
    // |match_length| are non-NULL, they are assigned the start position and total
    // length of the match.
    bool
    Search(const std::u16string& in_this, size_t* match_index, size_t* match_length);

private:
    FixedPatternStringSearch base_search_;
};

}  // namespace i18n
}  // namespace base

#endif  // BASE_I18N_STRING_SEARCH_H_
