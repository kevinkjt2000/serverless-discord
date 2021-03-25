#include <catch2/catch.hpp>
#include <fakeit/catch/fakeit.hpp>
#include <nlohmann/json.hpp>

#include "../src/discord.h"
#include "../src/ed25519.h"
#include "../src/handler.h"

using namespace aws::lambda_runtime;
using namespace fakeit;
using json = nlohmann::json;

TEST_CASE("Robot beeps when blep'ed at", "[handler]") {
    invocation_request req{.payload = (R"(
      {
        "body": "{\"application_id\":\"888888888888888888\",\"channel_id\":\"888888888888888888\",\"data\":{\"id\":\"888888888888888888\",\"name\":\"blep\",\"options\":[{\"name\":\"animal\",\"type\":3,\"value\":\"animal_dog\"}]},\"guild_id\":\"888888888888888888\",\"id\":\"888888888888888888\",\"member\":{\"deaf\":false,\"is_pending\":false,\"joined_at\":\"2021-03-08T02:04:39.013000+00:00\",\"mute\":false,\"nick\":null,\"pending\":false,\"permissions\":\"8888888888\",\"premium_since\":null,\"roles\":[],\"user\":{\"avatar\":\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\",\"discriminator\":\"2000\",\"id\":\"88888888888888888\",\"public_flags\":512,\"username\":\"someUser\"}},\"token\":\"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\",\"type\":2,\"version\":1}",
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
    REQUIRE(resp_payload["type"] == Discord::InteractionResponseType::ChannelMessageWithSource);
    REQUIRE(resp_payload["data"]["content"] == "bleep bloop");
}

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
