// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Cross platform methods for FilePathWatcher. See the various platform
// specific implementation files, too.

#include "fldserver/base/files/file_path_watcher.h"

#include "fldserver/base/check.h"
#include "fldserver/fldserver_config.h"

namespace base
{
FilePathWatcher::~FilePathWatcher()
{
    DCHECK(sequence_checker_.CalledOnValidSequence());
    impl_->Cancel();
}

// static
bool
FilePathWatcher::RecursiveWatchAvailable()
{
#if defined(OS_MAC) || IS_WIN || IS_LINUX || defined(OS_CHROMEOS) || defined(OS_ANDROID)           \
        || defined(OS_AIX)
    return true;
#else
    // FSEvents isn't available on iOS.
    return false;
#endif
}

FilePathWatcher::PlatformDelegate::PlatformDelegate() = default;

FilePathWatcher::PlatformDelegate::~PlatformDelegate()
{
    DCHECK(is_cancelled());
}

bool
FilePathWatcher::Watch(const FilePath& path, Type type, const Callback& callback)
{
    DCHECK(sequence_checker_.CalledOnValidSequence());
    DCHECK(path.IsAbsolute());
    return impl_->Watch(path, type, callback);
}

}  // namespace base
