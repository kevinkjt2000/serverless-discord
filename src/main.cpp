#include <string>

#include <aws/lambda-runtime/runtime.h>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

#include "ed25519.h"

using namespace aws::lambda_runtime;
using json = nlohmann::json;


enum class InteractionType {
    Ping = 1,
    ApplicationCommand = 2,
};


invocation_response my_handler(invocation_request const& req)
{
    try {
        auto parsed_payload = json::parse(req.payload);
        auto headers = parsed_payload["headers"];
        std::string body_str = parsed_payload["body"];
        std::string signature = headers["x-signature-ed25519"];
        std::string timestamp = headers["x-signature-timestamp"];
        fmt::print("X{}X\n", body_str);
        fmt::print("X{}X\n", signature);
        if(!is_valid_signature(signature, timestamp + body_str)) {
            fmt::print("invalid signature!\n");
            return invocation_response::failure("invalid signature", "unauthorized");
        }
        auto body = json::parse(body_str);
        if (body["type"] == InteractionType::Ping) {
            auto response = R"(
              {
                "type": 1
              }
            )"_json;
            fmt::print("success!\n");
            return invocation_response::success(response.dump(), "application/json");
        }
    } catch(nlohmann::detail::type_error& err) {
        fmt::print("{}\n", err.what());
        return invocation_response::failure("internal error", "internal");
    }

    fmt::print("generic failure\n");
    return invocation_response::failure("unexpected non-ping payload received",
                                        "error type here");
}

int main()
{
    run_handler(my_handler);
    return 0;
}
