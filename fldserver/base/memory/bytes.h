// Copyright 2021 The Blitz Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Original Author : Mohammed Hussam Al Turjman

#ifndef CORE_BASE_MEMORY_BYTES_H
#define CORE_BASE_MEMORY_BYTES_H

#include <inttypes.h>
#include "fldserver/fldserver_config.h"

namespace base
{
class CORE_EXPORT Bytes
{
public:
    constexpr explicit Bytes(uint64_t val)
    {
        _val = val;
    }
    uint64_t constexpr toKiloBytes() const
    {
        return _val / 1000LU;
    };

    uint64_t constexpr toMegaBytes() const
    {
        return toKiloBytes() / 1000LU;
    };

    uint64_t constexpr toGigaBytes() const
    {
        return toMegaBytes() / 1000LU;
    };

    uint64_t constexpr toTeraBytes() const
    {
        return toGigaBytes() / 1000LU;
    };

    uint64_t constexpr
    operator()() const
    {
        return _val;
    }

private:
    uint64_t _val = 0;
};

class CORE_EXPORT KiloBytes : public Bytes
{
public:
    constexpr explicit KiloBytes(uint64_t val) : Bytes(1000LU * val)
    {
    }
};

class CORE_EXPORT MegaBytes : public KiloBytes
{
public:
    constexpr explicit MegaBytes(uint64_t val) : KiloBytes(1000LU * val)
    {
    }
};

class CORE_EXPORT GigaBytes : public MegaBytes
{
public:
    constexpr explicit GigaBytes(uint64_t val) : MegaBytes(1000LU * val)
    {
    }
};

class CORE_EXPORT TeraBytes : public GigaBytes
{
public:
    constexpr explicit TeraBytes(uint64_t val) : GigaBytes(1000LU * val)
    {
    }
};

CORE_EXPORT inline uint64_t
toBytes(Bytes bytes)
{
    return bytes();
}
CORE_EXPORT inline uint64_t
toKiloBytes(Bytes bytes)
{
    return bytes.toKiloBytes();
}
CORE_EXPORT inline uint64_t
toMegaBytes(Bytes bytes)
{
    return bytes.toMegaBytes();
}
CORE_EXPORT inline uint64_t
toGigaBytes(Bytes bytes)
{
    return bytes.toGigaBytes();
}

CORE_EXPORT inline uint64_t
toTeraBytes(Bytes bytes)
{
    return bytes.toTeraBytes();
}

//

CORE_EXPORT inline constexpr uint64_t
fromBytes(uint64_t bytes)
{
    return Bytes(bytes).operator()();
}
CORE_EXPORT inline constexpr uint64_t
fromKiloBytes(uint64_t kilobytes)
{
    return KiloBytes(kilobytes).operator()();
}
CORE_EXPORT inline constexpr uint64_t
fromMegaBytes(uint64_t megaBytes)
{
    return MegaBytes(megaBytes).operator()();
}
CORE_EXPORT inline constexpr uint64_t
fromGigaBytes(uint64_t gigaBytes)
{
    return GigaBytes(gigaBytes).operator()();
}

CORE_EXPORT inline constexpr uint64_t
fromTeraBytes(uint64_t terabytes)
{
    return TeraBytes(terabytes).operator()();
}
}  // namespace base
#endif  // CORE_BASE_MEMORY_BYTES_H
