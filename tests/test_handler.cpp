#include <catch2/catch.hpp>
#include <fakeit/catch/fakeit.hpp>
#include <nlohmann/json.hpp>

#include "../src/discord.h"
#include "../src/ed25519.h"
#include "../src/handler.h"

using namespace aws::lambda_runtime;
using namespace fakeit;
using json = nlohmann::json;

TEST_CASE("Ping pong", "[handler]") {
    invocation_request req{.payload = (R"(
      {
        "body": "{\"type\": 1}",
        "headers": {
          "x-signature-ed25519": "",
          "x-signature-timestamp": ""
        }
      }
    )"_json).dump()};
    Mock<SignatureInterface> mock;
    Method(mock, is_valid_signature) = true;
    auto resp = my_handler(req, mock.get());
    REQUIRE(resp.is_success());
    auto resp_payload = json::parse(resp.get_payload());
    REQUIRE(resp_payload["type"] == Discord::InteractionResponseType::Pong);
}

TEST_CASE("Bad signature returns 401 unauthorized", "[handler]") {
    invocation_request req{.payload = (R"(
      {
        "body": "",
        "headers": {
          "x-signature-ed25519": "",
          "x-signature-timestamp": ""
        }
      }
    )"_json).dump()};
    Mock<SignatureInterface> mock;
    Method(mock, is_valid_signature) = false;
    auto resp = my_handler(req, mock.get());
    REQUIRE(!resp.is_success());
    auto resp_payload = json::parse(resp.get_payload());
    REQUIRE(resp_payload["errorMessage"] == "unauthorized");
    REQUIRE(resp_payload["errorType"] == "401");
}
