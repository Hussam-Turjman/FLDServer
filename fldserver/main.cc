// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#include "fldserver/base/command_line.h"
#include "fldserver/base/logging.h"
#include "fldserver/base/strings/string_number_conversions.h"
#include "fldserver/service/runner.h"
#include "fldserver/service/service_observer.h"

#include "fldserver/landmark_detection/features_extractor.h"
#include "fldserver/base/json/json_writer.h"
#include "fldserver/base/timer/timer.h"
#include "base/sequence_checker.h"
#include "base/task/sequence_manager/sequence_manager.h"
#include "base/message_loop/message_pump.h"
#include "base/task/thread_pool/thread_pool_instance.h"
#include "base/task/simple_task_executor.h"
#include "fldserver/service/sessions_handler.h"
#include <thread>
#include "base/run_loop.h"

static double delete_session_older_than = 10.0;  // seconds

static void
Cleaner()
{
    //service::SessionsHandler::Get()->global_session()->manager()->OpenSequenceReader(30,640,480);

    /*const int deleted_sessions =
            service::SessionsHandler::Get()->CleanDeadSessions(delete_session_older_than);

    if (deleted_sessions > 0)
    {
        LOG(INFO) << "\033[31mDeleted sessions : \033[m" << deleted_sessions;
    }*/
}

namespace base
{
std::unique_ptr<sequence_manager::SequenceManager>
CreateSequenceManager(MessagePumpType type = MessagePumpType::IO)
{
    return sequence_manager::CreateSequenceManagerOnCurrentThreadWithPump(
            MessagePump::Create(type),
            base::sequence_manager::SequenceManager::Settings::Builder()
                    .SetMessagePumpType(type)
                    .Build());
}
}  // namespace base

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

    int max_clients = 1;

    std::string port = base::CommandLine::ForCurrentProcess()->GetSwitchValueNative("port");
    std::string host = base::CommandLine::ForCurrentProcess()->GetSwitchValueNative("host");
    std::string workers = base::CommandLine::ForCurrentProcess()->GetSwitchValueNative("workers");
    std::string max_clients_flag =
            base::CommandLine::ForCurrentProcess()->GetSwitchValueNative("max_clients");

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
    if (!max_clients_flag.empty())
    {
        if (!base::StringToInt(max_clients_flag, &max_clients))
        {
            LOG(ERROR) << "Invalid max clients : " << max_clients_flag;
            return 1;
        }
        else
        {
            if (max_clients < 1)
            {
                LOG(ERROR) << "Invalid max clients : " << max_clients;
                return 1;
            }
        }
    }
    std::vector<std::string> manager_args;
    manager_args.emplace_back(argv[0]);

    LandmarkDetector::FeaturesExtractor extractor;

    service::SessionsHandler::Get()->SetArguments(manager_args);
    const bool create_status = service::SessionsHandler::Get()->InitializeGlobalSession(30,640,480);
    if (!create_status){
        LOG(ERROR) << "Failed to create global session ";
        return 1;
    }

    /*for (int i = 0; i < max_clients; ++i)
    {
        const bool create_status = service::SessionsHandler::Get()->CreateNewSession(std::string(),

                                                                                     30,
                                                                                     640,
                                                                                     480);
        if (!create_status)
        {
            LOG(ERROR) << "Failed to create a session !!";
            return 1;
        }
    }*/

    std::thread timer_thread([] {
        std::unique_ptr<base::sequence_manager::SequenceManager> sequence_manager =
                base::CreateSequenceManager();
        scoped_refptr<base::sequence_manager::TaskQueue> task_queue =
                sequence_manager->CreateTaskQueue(
                        base::sequence_manager::TaskQueue::Spec("task_environment_default"));
        scoped_refptr<base::SingleThreadTaskRunner> task_runner = task_queue->task_runner();
        sequence_manager->SetDefaultTaskRunner(task_runner);
        auto simple_task_executor = std::make_unique<base::SimpleTaskExecutor>(task_runner);
        base::ThreadPoolInstance::Create("Main");
        base::ThreadPoolInstance::InitParams initParams(10);
        initParams.suggested_reclaim_time = base::TimeDelta::Max();
        base::ThreadPoolInstance::Get()->Start(initParams);
        base::RunLoop runLoop;
        base::RepeatingTimer repeatingTimer;
        repeatingTimer.Start(
                FROM_HERE, base::TimeDelta::FromSeconds(10),
                base::BindRepeating(&Cleaner));
        runLoop.Run(FROM_HERE);
    });
    timer_thread.detach();
    const bool ret = service::Runner::Get()->Run(&extractor, host, port_int, workers_num);
    if (!ret)
    {
        LOG(ERROR) << "Failed to start the FLDServer";
        return 1;
    }
    return 0;
}