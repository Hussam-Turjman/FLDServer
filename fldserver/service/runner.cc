// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#include "fldserver/service/runner.h"
#include "fldserver/base/logging.h"
#include "fldserver/service/http_worker.h"
#include "fldserver/base/time/time.h"
#include "fldserver/base/time/time_to_iso8601.h"
#include <memory>
#include <boost/asio.hpp>
#include <chrono>

#include <iostream>
#include <list>
#include <string>

namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

namespace service
{
Runner*
Runner::Get()
{
    static std::unique_ptr<Runner> instance(new Runner());
    return instance.get();
}
Runner::Runner()
{
}
bool
Runner::Run(ServiceObserver* observer, const std::string& host, uint16_t port, int num_workers)
{
    try
    {
        auto const address = net::ip::make_address(host);

        std::string doc_root = ".";

        net::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {address, port}};

        std::list<HttpWorker> workers;
        for (int i = 0; i < num_workers; ++i)
        {
            workers.emplace_back(acceptor, doc_root, observer);
            workers.back().Start();
        }
        auto current_time = base::TimeToISO8601(base::Time::Now());
        LOG(INFO) << "FLDServer started on : " << current_time;
        LOG(INFO) << "Listening on : http://" << host << ":" << port;
        ioc.run();
        return true;
    }
    catch (const std::exception& e)
    {
        LOG(ERROR) << "Exception occurred : " << e.what();

        return false;
    }
}
}  // namespace service