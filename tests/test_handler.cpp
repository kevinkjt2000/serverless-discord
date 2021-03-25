#include <catch2/catch.hpp>
#include <nlohmann/json.hpp>

#include "../src/handler.h"

using namespace aws::lambda_runtime;
using json = nlohmann::json;

TEST_CASE("Bad signatures return 401 unauthorized", "[handler]") {
    invocation_request req{.payload = (R"(
      {
        "body": "",
        "headers": {
          "x-signature-ed25519": "",
          "x-signature-timestamp": ""
        }
      }
    )"_json).dump()};
    auto resp = my_handler(req);
    REQUIRE(!resp.is_success());
    auto resp_payload = json::parse(resp.get_payload());
    REQUIRE(resp_payload["errorMessage"] == "unauthorized");
    REQUIRE(resp_payload["errorType"] == "401");
}
