// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file exists for Unix systems which don't have the inotify headers, and
// thus cannot build file_watcher_inotify.cc

#include "fldserver/base/files/file_path_watcher.h"
#include "fldserver/base/memory/ptr_util.h"

namespace base
{
namespace
{
class FilePathWatcherImpl : public FilePathWatcher::PlatformDelegate
{
public:
    FilePathWatcherImpl() = default;
    FilePathWatcherImpl(const FilePathWatcherImpl&) = delete;
    FilePathWatcherImpl&
    operator=(const FilePathWatcherImpl&) = delete;
    ~FilePathWatcherImpl() override = default;

    bool
    Watch(const FilePath& path, Type type, const FilePathWatcher::Callback& callback) override
    {
        return false;
    }

    void
    Cancel() override
    {
    }
};

}  // namespace

FilePathWatcher::FilePathWatcher()
{
    sequence_checker_.DetachFromSequence();
    impl_ = std::make_unique<FilePathWatcherImpl>();
}

}  // namespace base
