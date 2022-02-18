// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_THREADING_FEATURES_H_
#define BASE_THREADING_THREADING_FEATURES_H_

#include "fldserver/fldserver_config.h"

#if IS_APPLE
#include "fldserver/base/metrics/field_trial_params.h"
#endif

namespace base
{
struct Feature;

#if IS_APPLE
extern const CORE_EXPORT Feature kOptimizedRealtimeThreadingMac;
extern const CORE_EXPORT FeatureParam<bool> kOptimizedRealtimeThreadingMacPreemptible;
extern const CORE_EXPORT FeatureParam<double> kOptimizedRealtimeThreadingMacBusy;
extern const CORE_EXPORT FeatureParam<double> kOptimizedRealtimeThreadingMacBusyLimit;
#endif

}  // namespace base

#endif  // BASE_THREADING_THREADING_FEATURES_H_
