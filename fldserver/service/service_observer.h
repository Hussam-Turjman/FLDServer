// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#ifndef FLDSERVER_SERVICE_SERVICE_OBSERVER_H
#define FLDSERVER_SERVICE_SERVICE_OBSERVER_H

#include "fldserver/fldserver_config.h"

#include <optional>
#include <string>
#include <atomic>

namespace service
{
class FLDSERVER_EXPORT ServiceObserver
{
public:
    virtual ~ServiceObserver()
    {
    }
    virtual std::optional<std::string>
    OnFrame(const std::string& session_id,
            double fps,
            int width,
            int height,
            const std::string& image,
            size_t size,
            bool with_vis_image) = 0;

    virtual int
    successful_tries() const final
    {
        return successful_tries_;
    }
    virtual int
    failed_tries() const final
    {
        return failed_tries_;
    }

protected:
    virtual void
    IncreaseSuccessfulTries() final
    {
        successful_tries_++;
    };
    virtual void
    DecreaseSuccessfulTries() final
    {
        successful_tries_--;
    };

    virtual void
    IncreaseFailedTries() final
    {
        failed_tries_++;
    };
    virtual void
    DecreaseFailedTries() final
    {
        failed_tries_--;
    };

private:
    std::atomic_int successful_tries_ = 0;
    std::atomic_int failed_tries_ = 0;
};
}  // namespace service

#endif  // FLDSERVER_SERVICE_SERVICE_OBSERVER_H
