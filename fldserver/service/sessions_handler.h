// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#ifndef FLDSERVER_SESSIONS_HANDLER_H
#define FLDSERVER_SESSIONS_HANDLER_H

#include "fldserver/fldserver_config.h"
#include "fldserver/service/session.h"
#include <vector>

namespace service
{
class FLDSERVER_EXPORT SessionsHandler
{
public:
    static SessionsHandler*
    Get();
    ~SessionsHandler();
    void
    SetArguments(const std::vector<std::string>& arguments);
    Session*global_session()const{return global_session_.get();};
    bool InitializeGlobalSession(int fps,
                                 int frame_width,
                                 int frame_height,
                                 float fx = -1,
                                 float fy = -1,
                                 float cx = -1,
                                 float cy = -1);
    bool
    CreateNewSession(const std::string& session_id,
                     int fps,
                     int frame_width,
                     int frame_height,
                     float fx = -1,
                     float fy = -1,
                     float cx = -1,
                     float cy = -1);
    Session*
    GetSession(const std::string& id, bool* should_be_initialized);

    /*Return the number of deleted sessions*/
    int
    CleanDeadSessions(double older_than_seconds /*in seconds*/);

    int
    ClearAllSessions();
    bool
    ClearSession(const std::string& id);

private:
    SessionsHandler();

private:
    std::vector<std::string> arguments_;
    std::vector<std::tuple<double, std::unique_ptr<Session>>> sessions_;
    std::mutex mutex_;
    std::unique_ptr<Session>global_session_ ;
};

}  // namespace service

#endif  // FLDSERVER_SESSIONS_HANDLER_H
