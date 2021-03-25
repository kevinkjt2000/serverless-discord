#include <string>

#include <fmt/core.h>
#include <nlohmann/json.hpp>

#include "discord.h"
#include "handler.h"
#include "ed25519.h"

using namespace aws::lambda_runtime;
using json = nlohmann::json;

invocation_response my_handler(invocation_request const& req, SignatureInterface const& verifier) {
    try {
        auto parsed_payload = json::parse(req.payload);
        auto headers = parsed_payload["headers"];
        std::string body_str = parsed_payload["body"];
        std::string signature = headers["x-signature-ed25519"];
        std::string timestamp = headers["x-signature-timestamp"];
        if(!verifier.is_valid_signature(signature, timestamp + body_str)) {
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
            fmt::print("{}\n{}\n{}\n", invoked_command_id, invoked_command_name, token);
            json resp = Discord::InteractionResponse{
                .type = Discord::InteractionResponseType::ChannelMessageWithSource,
                .data = {.tts = false, .content = "bleep bloop"}
            };
            return invocation_response::success(resp.dump(), "application/json");
        }
    } catch(nlohmann::detail::type_error& err) {
        fmt::print("{}\n", err.what());
        return invocation_response::failure("bad request", "402");
    }

    return invocation_response::failure("not implemented", "501");
}

invocation_response my_handler_shim(invocation_request const& req)
{
    Ed25519 verifier;
    return my_handler(req, verifier);
}
