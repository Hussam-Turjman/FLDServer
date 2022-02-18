// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#ifndef FLDSERVER_SERVICE_RUNNER_H
#define FLDSERVER_SERVICE_RUNNER_H

#include "fldserver/fldserver_config.h"
#include <string>
#include <cinttypes>

namespace service
{
class ServiceObserver;
class FLDSERVER_EXPORT Runner
{
public:
    static Runner*
    Get();
    Runner(const Runner&) = delete;
    Runner&
    operator=(const Runner&) = delete;
    bool
    Run(ServiceObserver* observer, const std::string& host, uint16_t port, int workers);

private:
    Runner();
};
}  // namespace service

#endif  // FLDSERVER_SERVICE_RUNNER_H
