// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_DISCARDABLE_MEMORY_INTERNAL_H_
#define BASE_MEMORY_DISCARDABLE_MEMORY_INTERNAL_H_

#include "fldserver/base/feature_list.h"
#include "fldserver/base/metrics/field_trial_params.h"
#include "fldserver/fldserver_config.h"

#if defined(OS_ANDROID) || IS_LINUX || defined(OS_CHROMEOS)

namespace base
{
// Enumeration of the possible experiment groups in the discardable memory
// backing trial. Note that |kAshmem| and |kEmulatedSharedMemory| both map to
// discardable shared memory, except the former allows for the use of ashmem for
// unpinning memory. Ensure that the order of the enum values matches those in
// |kDiscardableMemoryBackingParamOptions|.
enum DiscardableMemoryTrialGroup : int
{
    kEmulatedSharedMemory = 0,
    kMadvFree,
    // Only Android devices will be assigned to the ashmem group.
    kAshmem,
};

namespace features
{
// Feature flag enabling the discardable memory backing trial.
CORE_EXPORT extern const base::Feature kDiscardableMemoryBackingTrial;

CORE_EXPORT extern const base::FeatureParam<DiscardableMemoryTrialGroup>::Option
        kDiscardableMemoryBackingParamOptions[];

CORE_EXPORT extern const base::FeatureParam<DiscardableMemoryTrialGroup>
        kDiscardableMemoryBackingParam;
}  // namespace features

// Whether we should do the discardable memory backing trial for this session.
CORE_EXPORT bool
DiscardableMemoryBackingFieldTrialIsEnabled();

// If we should do the discardable memory backing trial, then get the trial
// group this session belongs in.
CORE_EXPORT DiscardableMemoryTrialGroup
GetDiscardableMemoryBackingFieldTrialGroup();

}  // namespace base

#endif  // IS_LINUX || defined(OS_CHROMEOS) || defined(OS_ANDROID)

#endif  //  BASE_MEMORY_DISCARDABLE_MEMORY_INTERNAL_H_
