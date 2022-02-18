// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILE_VERSION_INFO_H_
#define BASE_FILE_VERSION_INFO_H_

#include "fldserver/fldserver_config.h"

#include <memory>
#include <string>

#if IS_WIN
#include <windows.h>
#endif

namespace base
{
class FilePath;
}

// Provides an interface for accessing the version information for a file. This
// is the information you access when you select a file in the Windows Explorer,
// right-click select Properties, then click the Version tab, and on the Mac
// when you select a file in the Finder and do a Get Info.
//
// This list of properties is straight out of Win32's VerQueryValue
// <http://msdn.microsoft.com/en-us/library/ms647464.aspx> and the Mac
// version returns values from the Info.plist as appropriate. TODO(avi): make
// this a less-obvious Windows-ism.

class CORE_EXPORT FileVersionInfo
{
public:
    virtual ~FileVersionInfo()
    {
    }
#if IS_WIN || IS_APPLE
    // Creates a FileVersionInfo for the specified path. Returns nullptr if
    // something goes wrong (typically the file does not exit or cannot be
    // opened).
    static std::unique_ptr<FileVersionInfo>
    CreateFileVersionInfo(const base::FilePath& file_path);
#endif  // OS_WIN || OS_APPLE

#if IS_WIN
    // Creates a FileVersionInfo for the specified module. Returns nullptr in
    // case of error.
    static std::unique_ptr<FileVersionInfo>
    CreateFileVersionInfoForModule(HMODULE module);
#else
    // Creates a FileVersionInfo for the current module. Returns nullptr in case
    // of error.
    static std::unique_ptr<FileVersionInfo>
    CreateFileVersionInfoForCurrentModule();
#endif  // OS_WIN

    // Accessors to the different version properties.
    // Returns an empty string if the property is not found.
    virtual std::u16string
    company_name() = 0;
    virtual std::u16string
    company_short_name() = 0;
    virtual std::u16string
    product_name() = 0;
    virtual std::u16string
    product_short_name() = 0;
    virtual std::u16string
    internal_name() = 0;
    virtual std::u16string
    product_version() = 0;
    virtual std::u16string
    special_build() = 0;
    virtual std::u16string
    original_filename() = 0;
    virtual std::u16string
    file_description() = 0;
    virtual std::u16string
    file_version() = 0;
};

#endif  // BASE_FILE_VERSION_INFO_H_