// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <cstring>

#include "fldserver/base/sql/statement_id.h"

namespace sql
{
bool
StatementID::operator<(const StatementID& rhs) const noexcept
{
    if (rhs.source_line_ != source_line_)
        return source_line_ < rhs.source_line_;
    return std::strcmp(source_file_, rhs.source_file_) < 0;
}

}  // namespace sql