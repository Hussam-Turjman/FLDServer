// Copyright 2021 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILES_FILE_PATH_WATCHER_LINUX_H_
#define BASE_FILES_FILE_PATH_WATCHER_LINUX_H_

#include "fldserver/fldserver_config.h"

#include <stddef.h>

namespace base
{
// Overrides max inotify watcher counter for test.
class CORE_EXPORT ScopedMaxNumberOfInotifyWatchesOverrideForTest
{
public:
    explicit ScopedMaxNumberOfInotifyWatchesOverrideForTest(size_t override_max);
    ~ScopedMaxNumberOfInotifyWatchesOverrideForTest();
};

}  // namespace base

#endif  // BASE_FILES_FILE_PATH_WATCHER_LINUX_H_
