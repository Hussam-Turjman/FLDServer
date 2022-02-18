// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_HASH_LEGACY_HASH_H_
#define BASE_HASH_LEGACY_HASH_H_

#include "fldserver/base/containers/span.h"
#include "fldserver/fldserver_config.h"

#include <stdint.h>

namespace base
{
namespace legacy
{
// Implementation is fixed at CityHash v1.0.3.
// Unchanging forever: yes
CORE_EXPORT uint64_t
CityHash64(base::span<const uint8_t> data);
CORE_EXPORT uint64_t
CityHash64WithSeed(base::span<const uint8_t> data, uint64_t seed);

}  // namespace legacy
}  // namespace base

#endif  // BASE_HASH_LEGACY_HASH_H_
