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

#include "fldserver/service/http_worker.h"
#include "fldserver/service/service_observer.h"
#include "fldserver/base/json/json_reader.h"
#include "fldserver/base/logging.h"
#include "fldserver/base/check.h"
#include "fldserver/base/json/json_writer.h"
#include "fldserver/service/sessions_handler.h"

#include <utility>

namespace
{
static const char index_page[] =
        "<!doctype html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <!-- Required meta tags -->\n"
        "    <meta charset=\"utf-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, "
        "shrink-to-fit=no\">\n"
        "\n"
        "    <!-- Bootstrap CSS -->\n"
        "    <link rel=\"stylesheet\" "
        "href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css\"\n"
        "          "
        "integrity=\"sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh\" "
        "crossorigin=\"anonymous\">\n"
        "\n"
        "    <title>FLDServer</title>\n"
        "</head>\n"
        "<body>\n"
        "\n"
        "<!-- Begin page content -->\n"
        "<main role=\"main\" class=\"container\">\n"
        "    <h1 class=\"mt-5\">FLDServer</h1>\n"
        "    <p class=\"lead\">Congratulation, it works !!</p>\n"
        "</main>\n"
        "\n"
        "\n"
        "<footer class=\"footer\">\n"
        "    <div class=\"container\">\n"
        "        <span class=\"text-muted\">This project is still under development and may crash "
        "if used outside the defined REST-API. <a\n"
        "                href=\"https://github.com/Hussam-Turjman/FLDServer\">Click here</a> "
        "for more information.</span>\n"
        "    </div>\n"
        "</footer>\n"
        "<!-- Optional JavaScript -->\n"
        "<!-- jQuery first, then Popper.js, then Bootstrap JS -->\n"
        "<script src=\"https://code.jquery.com/jquery-3.4.1.slim.min.js\"\n"
        "        "
        "integrity=\"sha384-J6qa4849blE2+poT4WnyKhv5vZF5SrPo0iEjwBvKU7imGFAV0wwj1yYfoRSJoZ+n\"\n"
        "        crossorigin=\"anonymous\"></script>\n"
        "<script src=\"https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js\"\n"
        "        "
        "integrity=\"sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo\"\n"
        "        crossorigin=\"anonymous\"></script>\n"
        "<script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js\"\n"
        "        "
        "integrity=\"sha384-wfSDF2E50Y2D1uUdj0O3uMBJnjuUD4Ih7YwaYd1iqfktj0Uod8GCExl3Og8ifwB6\"\n"
        "        crossorigin=\"anonymous\"></script>\n"
        "</body>\n"
        "</html>";
}
namespace service
{
// Return a reasonable mime type based on the extension of a file.
static beast::string_view
mime_type(beast::string_view path)
{
    using beast::iequals;
    auto const ext = [&path] {
        auto const pos = path.rfind(".");
        if (pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
    }();
    if (iequals(ext, ".htm"))
        return "text/html";
    if (iequals(ext, ".html"))
        return "text/html";
    if (iequals(ext, ".php"))
        return "text/html";
    if (iequals(ext, ".css"))
        return "text/css";
    if (iequals(ext, ".txt"))
        return "text/plain";
    if (iequals(ext, ".js"))
        return "application/javascript";
    if (iequals(ext, ".json"))
        return "application/json";
    if (iequals(ext, ".xml"))
        return "application/xml";
    if (iequals(ext, ".swf"))
        return "application/x-shockwave-flash";
    if (iequals(ext, ".flv"))
        return "video/x-flv";
    if (iequals(ext, ".png"))
        return "image/png";
    if (iequals(ext, ".jpe"))
        return "image/jpeg";
    if (iequals(ext, ".jpeg"))
        return "image/jpeg";
    if (iequals(ext, ".jpg"))
        return "image/jpeg";
    if (iequals(ext, ".gif"))
        return "image/gif";
    if (iequals(ext, ".bmp"))
        return "image/bmp";
    if (iequals(ext, ".ico"))
        return "image/vnd.microsoft.icon";
    if (iequals(ext, ".tiff"))
        return "image/tiff";
    if (iequals(ext, ".tif"))
        return "image/tiff";
    if (iequals(ext, ".svg"))
        return "image/svg+xml";
    if (iequals(ext, ".svgz"))
        return "image/svg+xml";
    return "application/text";
}

const char HttpWorker::kServerName[] = "FLDServer";

HttpWorker::HttpWorker(tcp::acceptor& acceptor, std::string doc_root, ServiceObserver* observer) :
    acceptor_(acceptor), doc_root_(std::move(doc_root)), observer_(observer)
{
}

void
HttpWorker::Start()
{
    Accept();
    CheckDeadline();
}

void
HttpWorker::Accept()
{
    // Clean up any previous connection.
    beast::error_code ec;
    socket_.close(ec);
    buffer_.consume(buffer_.size());

    acceptor_.async_accept(socket_, [this](beast::error_code ec) {
        if (ec)
        {
            Accept();
        }
        else
        {
            // Request must be fully processed within 60 seconds.
            request_deadline_.expires_after(std::chrono::seconds(60));

            ReadRequest();
        }
    });
}

void
HttpWorker::ReadRequest()
{
    // On each read the parser needs to be destroyed and
    // recreated. We store it in a boost::optional to
    // achieve that.
    //
    // Arguments passed to the parser constructor are
    // forwarded to the message object. A single argument
    // is forwarded to the body constructor.
    //
    // We construct the dynamic body with a 1MB limit
    // to prevent vulnerability to buffer attacks.
    //
    parser_.emplace(std::piecewise_construct, std::make_tuple(), std::make_tuple(alloc_));

    http::async_read(socket_, buffer_, *parser_, [this](beast::error_code ec, std::size_t) {
        if (ec)
            Accept();
        else
            ProcessRequest(parser_->get());
    });
}

void
HttpWorker::ProcessRequest(const http::request<request_body_t, http::basic_fields<alloc_t>>& req)
{
    switch (req.method())
    {
        case http::verb::get: HandleGet(req); break;
        case http::verb::post: HandlePost(req); break;
        default:
            // We return responses indicating an error if
            // we do not recognize the request method.
            SendBadResponse(http::status::bad_request,
                            "Invalid request-method '" + std::string(req.method_string())
                                    + "'\r\n");
            break;
    }
}

void
HttpWorker::SendStringResponse(http::status status,
                               std::string const& content,
                               const std::string& content_type)
{
    string_response_.emplace(std::piecewise_construct, std::make_tuple(), std::make_tuple(alloc_));

    string_response_->result(status);
    string_response_->keep_alive(false);
    string_response_->set(http::field::server, kServerName);
    string_response_->set(http::field::content_type, content_type);
    string_response_->body() = content;
    string_response_->prepare_payload();

    string_serializer_.emplace(*string_response_);

    http::async_write(socket_, *string_serializer_, [this](beast::error_code ec, std::size_t) {
        socket_.shutdown(tcp::socket::shutdown_send, ec);
        string_serializer_.reset();
        string_response_.reset();
        Accept();
    });
}

void
HttpWorker::SendBadResponse(http::status status, const std::string& error)
{
    SendStringResponse(status, error, "text/plain");
}

void
HttpWorker::HandleGet(const http::request<request_body_t, http::basic_fields<alloc_t>>& req)
{
    beast::string_view target = req.target();

    if (target == "/")
    {
        SendStringResponse(http::status::ok, index_page, "text/html");
        return;
    }
    else if (target == "/status")
    {
        base::Value value(base::Value::Type::DICTIONARY);
        value.SetKey("SuccessfulTries", base::Value(observer_->successful_tries()));
        value.SetKey("FailedTries", base::Value(observer_->failed_tries()));

        std::string serialized;
        if (base::JSONWriter::Write(value, &serialized))
        {
            SendStringResponse(http::status::ok, serialized, "application/json");
            return;
        }
    }
    else if (target == "/clear_all_sessions")
    {
        base::Value value(base::Value::Type::DICTIONARY);
        const int cleared_sessions = SessionsHandler::Get()->ClearAllSessions();
        value.SetKey("cleared_sessions", base::Value(cleared_sessions));

        std::string serialized;
        if (base::JSONWriter::Write(value, &serialized))
        {
            SendStringResponse(http::status::ok, serialized, "application/json");
            return;
        }
    }

    SendBadResponse(http::status::bad_request, "Bad Request\r\n");
    return;
    // From here is for serving files.

    // Request path must be absolute and not contain "..".
    if (target.empty() || target[0] != '/' || target.find("..") != std::string::npos)
    {
        SendBadResponse(http::status::not_found, "File not found\r\n");
        return;
    }

    std::string full_path = doc_root_;
    full_path.append(target.data(), target.size());

    http::file_body::value_type file;
    beast::error_code ec;
    file.open(full_path.c_str(), beast::file_mode::read, ec);
    if (ec)
    {
        SendBadResponse(http::status::not_found, "File not found\r\n");
        return;
    }

    file_response_.emplace(std::piecewise_construct, std::make_tuple(), std::make_tuple(alloc_));

    file_response_->result(http::status::ok);
    file_response_->keep_alive(false);
    file_response_->set(http::field::server, "Beast");
    file_response_->set(http::field::content_type, mime_type(std::string(target)));
    file_response_->body() = std::move(file);
    file_response_->prepare_payload();

    file_serializer_.emplace(*file_response_);

    http::async_write(socket_, *file_serializer_, [this](beast::error_code ec, std::size_t) {
        socket_.shutdown(tcp::socket::shutdown_send, ec);
        file_serializer_.reset();
        file_response_.reset();
        Accept();
    });
}

void
HttpWorker::CheckDeadline()
{
    // The deadline may have moved, so check it has really passed.
    if (request_deadline_.expiry() <= std::chrono::steady_clock::now())
    {
        // Close socket to cancel any outstanding operation.
        beast::error_code ec;
        socket_.close();

        // Sleep indefinitely until we're given a new deadline.
        request_deadline_.expires_at(std::chrono::steady_clock::time_point::max());
    }

    request_deadline_.async_wait([this](beast::error_code) { CheckDeadline(); });
}

void
HttpWorker::HandlePost(const http::request<request_body_t, http::basic_fields<alloc_t>>& req)
{
    auto json_value = base::JSONReader::Read(req.body());
    beast::string_view target = req.target();
    if (target == "/")
    {
        if (json_value.has_value())
        {
            auto image = json_value->FindKey("image");
            auto size = json_value->FindKey("size");
            auto width = json_value->FindKey("width");
            auto height = json_value->FindKey("height");
            auto fps = json_value->FindKey("fps");
            auto with_vis_image = json_value->FindKey("with_vis_image");

            auto session_id = json_value->FindKey("session_id");
            if (!(image && size && width && height && fps && session_id && with_vis_image))
            {
                SendBadResponse(http::status::bad_request, "Invalid arguments\r\n");
                return;
            }
            if (!image->is_string())
            {
                SendBadResponse(http::status::bad_request, "Image must be string\r\n");
                return;
            }
            if (!width->is_int())
            {
                SendBadResponse(http::status::bad_request, "Width must be int\r\n");
                return;
            }
            if (!height->is_int())
            {
                SendBadResponse(http::status::bad_request, "Height must be int\r\n");
                return;
            }
            if (!size->is_int())
            {
                SendBadResponse(http::status::bad_request, "Size must be int\r\n");
                return;
            }
            if (!fps->is_double())
            {
                SendBadResponse(http::status::bad_request, "FPS must be double\r\n");
                return;
            }
            if (!session_id->is_string())
            {
                SendBadResponse(http::status::bad_request, "Session_id must be string\r\n");
                return;
            }
            if (!with_vis_image->is_bool())
            {
                SendBadResponse(http::status::bad_request, "With_vis_image must be bool\r\n");
                return;
            }

            auto features = observer_->OnFrame(session_id->GetString(),
                                               fps->GetDouble(),
                                               width->GetInt(),
                                               height->GetInt(),
                                               image->GetString(),
                                               size->GetInt(),
                                               with_vis_image->GetBool());
            if (features.has_value())
            {
                SendStringResponse(http::status::ok, features.value(), "application/json");
                return;
            }
        }
        else
        {
            SendBadResponse(http::status::bad_request, "Body must be json\r\n");
            return;
        }
    }
    else if (target == "/clear_session")
    {
        if (json_value.has_value())
        {
            auto session_id = json_value->FindKey("image");
            if (!session_id)
            {
                SendBadResponse(http::status::bad_request, "Invalid arguments\r\n");
                return;
            }
            if (!session_id->is_string())
            {
                SendBadResponse(http::status::bad_request, "Session_id must be string\r\n");
                return;
            }
            const bool clear_status = SessionsHandler::Get()->ClearSession(session_id->GetString());
            base::Value value(base::Value::Type::DICTIONARY);

            value.SetKey("cleared", base::Value(clear_status));

            std::string serialized;
            if (base::JSONWriter::Write(value, &serialized))
            {
                SendStringResponse(http::status::ok, serialized, "application/json");
                return;
            }
        }
        else
        {
            SendBadResponse(http::status::bad_request, "Body must be json\r\n");
            return;
        }
    }

    SendBadResponse(http::status::bad_request, "Bad Request\r\n");
}

}  // namespace service