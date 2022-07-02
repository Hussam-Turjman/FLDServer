// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#include "fldserver/service/sessions_handler.h"
#include "fldserver/base/logging.h"

#include <memory>

namespace service
{
namespace
{
double
CurrentTime()
{
    return std::chrono::duration_cast<std::chrono::duration<double>>(
                   std::chrono::system_clock::now().time_since_epoch())
            .count();
}
}  // namespace

SessionsHandler*
SessionsHandler::Get()
{
    static std::unique_ptr<SessionsHandler> global_instance;
    if (!global_instance)
    {
        global_instance.reset(new SessionsHandler());
    }
    return global_instance.get();
}

SessionsHandler::~SessionsHandler()
{
}

SessionsHandler::SessionsHandler() : sessions_(), mutex_()
{
}

bool
SessionsHandler::CreateNewSession(const std::string& session_id,
                                  int fps,
                                  int frame_width,
                                  int frame_height,
                                  float fx,
                                  float fy,
                                  float cx,
                                  float cy)
{
    std::lock_guard<std::mutex> guard(mutex_);
    if (arguments_.empty())
    {
        LOG(ERROR) << "Arguments are not set !!";
        return false;
    }
    auto session = Session::Create(arguments_, fps, frame_width, frame_height, fx, fy, cx, cy);
    if (session)
    {
        session->SetId(session_id);
        sessions_.emplace_back(CurrentTime(), std::move(session));
        return true;
    }
    return false;
}

Session*
SessionsHandler::GetSession(const std::string& id, bool* should_be_initialized)
{
    std::lock_guard<std::mutex> guard(mutex_);
    CHECK(should_be_initialized) << "Invalid pointer";
    *should_be_initialized = false;
    if (id.empty())
    {
        LOG(ERROR) << "Session id is empty !!";
        return nullptr;
    }
    auto iter = sessions_.begin();
    while (iter != sessions_.end())
    {
        if (std::get<1>((*iter))->id() == id)
        {
            std::get<0>(*iter) = CurrentTime();
            return std::get<1>(*iter).get();
        }
        iter++;
    }
    iter = sessions_.begin();
    while (iter != sessions_.end())
    {
        if (std::get<1>((*iter))->id().empty())
        {
            std::get<1>((*iter))->SetId(id);
            const bool init_status = std::get<1>((*iter))->manager()->Init(arguments_);
            CHECK(init_status) << "Failed to initialize detection manager !! session id : " << id;

            *should_be_initialized = true;
            return std::get<1>((*iter)).get();
        }
        iter++;
    }
    return nullptr;
}

int
SessionsHandler::CleanDeadSessions(double older_than)
{
    std::lock_guard<std::mutex> guard(mutex_);

    auto iter = sessions_.begin();
    const double current_time = CurrentTime();
    int counter = 0;

    while (iter != sessions_.end())
    {
        if (!std::get<1>(*iter)->id().empty())
        {
            if (current_time - std::get<0>(*iter) >= older_than)
            {
                counter++;
                std::get<1>(*iter)->SetId(std::string());
            }
        }

        iter++;
    }
    return counter;
}

int
SessionsHandler::ClearAllSessions()
{
    std::lock_guard<std::mutex> guard(mutex_);

    auto iter = sessions_.begin();

    int counter = 0;

    while (iter != sessions_.end())
    {
        counter++;
        std::get<1>(*iter)->SetId(std::string());
        iter++;
    }
    return counter;
}

bool
SessionsHandler::ClearSession(const std::string& id)
{
    std::lock_guard<std::mutex> guard(mutex_);

    auto iter = sessions_.begin();

    while (iter != sessions_.end())
    {
        if (std::get<1>(*iter)->id() == id)
        {
            std::get<1>(*iter)->SetId(std::string());
            return true;
        }
        iter++;
    }

    return false;
}
}  // namespace service