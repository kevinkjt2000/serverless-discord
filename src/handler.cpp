#include <string>

#include <fmt/core.h>
#include <nlohmann/json.hpp>

#include "discord.h"
#include "handler.h"
#include "ed25519.h"

using namespace aws::lambda_runtime;
using json = nlohmann::json;

invocation_response my_handler(invocation_request const& req)
{
    try {
        auto parsed_payload = json::parse(req.payload);
        auto headers = parsed_payload["headers"];
        std::string body_str = parsed_payload["body"];
        std::string signature = headers["x-signature-ed25519"];
        std::string timestamp = headers["x-signature-timestamp"];
        if(!is_valid_signature(signature, timestamp + body_str)) {
            fmt::print("invalid signature!\n");
            return invocation_response::failure("unauthorized", "401");
        }
        auto body = json::parse(body_str);
        if (body["type"] == Discord::InteractionType::Ping) {
            auto pong = R"(
              {
                "type": 1
              }
            )"_json;
            fmt::print("pong success!\n");
            return invocation_response::success(pong.dump(), "application/json");
        } else if (body["type"] == Discord::InteractionType::ApplicationCommand) {
            auto data = body["data"];
            auto invoked_command_id = data["id"];
            auto invoked_command_name = data["name"];
            auto options = data["options"];
            auto token = body["token"];
        }
    } catch(nlohmann::detail::type_error& err) {
        fmt::print("{}\n", err.what());
        return invocation_response::failure("bad request", "402");
    }

    fmt::print("generic failure\n");
    return invocation_response::failure("unexpected non-ping payload received",
                                        "error type here");
}
