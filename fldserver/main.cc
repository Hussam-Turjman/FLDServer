// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#include "fldserver/base/command_line.h"
#include "fldserver/base/logging.h"
#include "fldserver/base/strings/string_number_conversions.h"
#include "fldserver/service/runner.h"
#include "fldserver/service/service_observer.h"
#include "fldserver/landmark_detector/manager.h"
#include "fldserver/landmark_detector/features_extractor.h"
#include "fldserver/base/json/json_writer.h"

int
main(int argc, char** argv)
{
    std::string command;
    for (int i = 0; i < argc; ++i)
    {
        command += " " + std::string(argv[i]);
    }
    LOG(INFO) << "Running: " << command;
    if (!base::CommandLine::Init(argc, argv))
    {
        LOG(ERROR) << "Failed to initialize CommandLine singleton";
        return 1;
    }
    auto switches = base::CommandLine::ForCurrentProcess()->GetSwitches();

    std::string port = base::CommandLine::ForCurrentProcess()->GetSwitchValueNative("port");
    std::string host = base::CommandLine::ForCurrentProcess()->GetSwitchValueNative("host");
    std::string workers = base::CommandLine::ForCurrentProcess()->GetSwitchValueNative("workers");
    bool just_testing = base::CommandLine::ForCurrentProcess()->HasSwitch("just_testing");
    if (just_testing)
    {
        LOG(INFO) << "The FLDServer works !!";
        return 0;
    }

    if (host.empty())
    {
        host = "127.0.0.1";
    }
    if (port.empty())
    {
        port = "34568";
    }

    int port_int = -1;
    if (!base::StringToInt(port, &port_int))
    {
        LOG(ERROR) << "Invalid port : " << port;
        return 1;
    }
    int workers_num = 4;
    if (!workers.empty())
    {
        if (!base::StringToInt(workers, &workers_num))
        {
            LOG(ERROR) << "Invalid workers : " << workers;
            return 1;
        }
    }

    std::vector<std::string> manager_args;
    manager_args.emplace_back(argv[0]);
    LandmarkDetector::FLDManager manager(manager_args);
    if (!manager.Init())
    {
        LOG(ERROR) << "Failed to initialize OpenFace manager";
        return 1;
    }
    LandmarkDetector::FeaturesExtractor extractor(&manager);
    const bool ret = service::Runner::Get()->Run(&extractor, host, port_int, workers_num);
    if (!ret)
    {
        LOG(ERROR) << "Failed to start the FLDServer";
        return 1;
    }
    return 0;
}