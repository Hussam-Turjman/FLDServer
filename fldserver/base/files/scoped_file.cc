// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/files/scoped_file.h"

#include "fldserver/base/check.h"
#include "fldserver/fldserver_config.h"

#if IS_POSIX || defined(OS_FUCHSIA)
#include "fldserver/base/posix/eintr_wrapper.h"

#include <unistd.h>

#include <errno.h>
#endif

namespace base
{
namespace internal
{
#if IS_POSIX || defined(OS_FUCHSIA)

// static
void
ScopedFDCloseTraits::Free(int fd)
{
    // It's important to crash here.
    // There are security implications to not closing a file descriptor
    // properly. As file descriptors are "capabilities", keeping them open
    // would make the current process keep access to a resource. Much of
    // Chrome relies on being able to "drop" such access.
    // It's especially problematic on Linux with the setuid sandbox, where
    // a single open directory would bypass the entire security model.
    int ret = IGNORE_EINTR(close(fd));

#if IS_LINUX || defined(OS_CHROMEOS) || IS_APPLE || defined(OS_FUCHSIA) || defined(OS_ANDROID)
    // NB: Some file descriptors can return errors from close() e.g. network
    // filesystems such as NFS and Linux input devices. On Linux, macOS, and
    // Fuchsia's POSIX layer, errors from close other than EBADF do not indicate
    // failure to actually close the fd.
    if (ret != 0 && errno != EBADF)
        ret = 0;
#endif

    PCHECK(0 == ret);
}

#endif  // OS_POSIX || OS_FUCHSIA

}  // namespace internal
}  // namespace base
