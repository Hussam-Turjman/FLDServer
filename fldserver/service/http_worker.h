//
// Copyright (c) 2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP server, fast
//
//------------------------------------------------------------------------------

#include "fldserver/service/fields_alloc.h"
#include "fldserver/fldserver_config.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <string>

namespace service
{
class ServiceObserver;
namespace beast = boost::beast;    // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>
namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

class FLDSERVER_EXPORT HttpWorker
{
private:
    using alloc_t = fields_alloc<char>;
    // using request_body_t = http::basic_dynamic_body<beast::flat_static_buffer<1024 * 1024>>;
    using request_body_t = http::string_body;
    static const char kServerName[];

public:
    HttpWorker(HttpWorker const&) = delete;
    HttpWorker&
    operator=(HttpWorker const&) = delete;
    HttpWorker(tcp::acceptor& acceptor, std::string doc_root, ServiceObserver* observer);
    void
    Start();

private:
    void
    Accept();
    void
    ReadRequest();

    void
    ProcessRequest(http::request<request_body_t, http::basic_fields<alloc_t>> const& req);

    void
    HandlePost(const http::request<request_body_t, http::basic_fields<alloc_t>>& req);
    void
    SendBadResponse(http::status status, std::string const& error);

    void
    SendStringResponse(http::status status,
                       std::string const& content,
                       const std::string& content_type);
    void
    SendFile(beast::string_view target);

    void
    CheckDeadline();

private:
    // The acceptor used to listen for incoming connections.
    tcp::acceptor& acceptor_;

    // The path to the root of the document directory.
    std::string doc_root_;

    // The socket for the currently connected client.
    tcp::socket socket_{acceptor_.get_executor()};

    // The buffer for performing reads
    beast::flat_static_buffer<8192> buffer_;

    // The allocator used for the fields in the request and reply.
    alloc_t alloc_{8192};

    // The parser for reading the requests
    boost::optional<http::request_parser<request_body_t, alloc_t>> parser_;

    // The timer putting a time limit on requests.
    net::basic_waitable_timer<std::chrono::steady_clock> request_deadline_{
            acceptor_.get_executor(), (std::chrono::steady_clock::time_point::max)()};

    // The string-based response message.
    boost::optional<http::response<http::string_body, http::basic_fields<alloc_t>>>
            string_response_;

    // The string-based response serializer.
    boost::optional<http::response_serializer<http::string_body, http::basic_fields<alloc_t>>>
            string_serializer_;

    // The file-based response message.
    boost::optional<http::response<http::file_body, http::basic_fields<alloc_t>>> file_response_;

    // The file-based response serializer.
    boost::optional<http::response_serializer<http::file_body, http::basic_fields<alloc_t>>>
            file_serializer_;
    ServiceObserver* const observer_;
};
}  // namespace service
