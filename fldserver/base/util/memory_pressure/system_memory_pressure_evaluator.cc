// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/util/memory_pressure/system_memory_pressure_evaluator.h"

#include "fldserver/base/feature_list.h"
#include "fldserver/fldserver_config.h"
#include "fldserver/fldserver_config.h"

#if defined(OS_FUCHSIA)
#include "fldserver/base/util/memory_pressure/system_memory_pressure_evaluator_fuchsia.h"
#elif defined(OS_MAC)
#include "fldserver/base/util/memory_pressure/system_memory_pressure_evaluator_mac.h"
#elif IS_WIN
#include "fldserver/base/util/memory_pressure/system_memory_pressure_evaluator_win.h"
#include "fldserver/base/win/windows_version.h"
#elif 0
#include "fldserver/base/util/memory_pressure/system_memory_pressure_evaluator_linux.h"
#endif

namespace util
{
#if IS_WIN
constexpr base::Feature kUseWinOSMemoryPressureSignals{"UseWinOSMemoryPressureSignals",
                                                       base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// static
std::unique_ptr<SystemMemoryPressureEvaluator>
SystemMemoryPressureEvaluator::CreateDefaultSystemEvaluator(
        MultiSourceMemoryPressureMonitor* monitor)
{
#if defined(OS_FUCHSIA)
    return std::make_unique<util::SystemMemoryPressureEvaluatorFuchsia>(monitor->CreateVoter());
#elif defined(OS_MAC)
    return std::make_unique<util::mac::SystemMemoryPressureEvaluator>(monitor->CreateVoter());
#elif IS_WIN
    auto evaluator =
            std::make_unique<util::win::SystemMemoryPressureEvaluator>(monitor->CreateVoter());
    // Also subscribe to the OS signals if they're available and the feature is
    // enabled.
    if (base::FeatureList::IsEnabled(kUseWinOSMemoryPressureSignals)
        && base::win::GetVersion() >= base::win::Version::WIN8_1)
    {
        evaluator->CreateOSSignalPressureEvaluator(monitor->CreateVoter());
    }
    return evaluator;
#elif 0
    return std::make_unique<util::os_linux::SystemMemoryPressureEvaluator>(monitor->CreateVoter());
#endif
    return nullptr;
}

SystemMemoryPressureEvaluator::SystemMemoryPressureEvaluator(
        std::unique_ptr<MemoryPressureVoter> voter) :
    current_vote_(base::MemoryPressureListener::MEMORY_PRESSURE_LEVEL_NONE),
    voter_(std::move(voter))
{
}

SystemMemoryPressureEvaluator::~SystemMemoryPressureEvaluator() = default;

void
SystemMemoryPressureEvaluator::SetCurrentVote(
        base::MemoryPressureListener::MemoryPressureLevel level)
{
    DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
    current_vote_ = level;
}

void
SystemMemoryPressureEvaluator::SendCurrentVote(bool notify) const
{
    DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
    voter_->SetVote(current_vote_, notify);
}

}  // namespace util
