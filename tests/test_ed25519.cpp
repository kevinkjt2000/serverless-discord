#include <string>

#include <catch2/catch.hpp>
#include <ed25519/ed25519.h>
#include <fmt/core.h>

#include "../src/ed25519.h"
#include "../src/utils.h"

template<class T>
std::string to_string(T t) {
    std::string ret;
    for(int i = 0; i < sizeof(t.data); i++) {
        ret += fmt::format("{0:02x}", t.data[i]);
    }
    return ret;
}

TEST_CASE("String conversions are inverses of each other", "[ed25519]") {
    std::string key = "0b178bc8f835c3fe8b2e91085d3f0000c400f5d2181a2fc0663b6d9062cdb7b8";
    REQUIRE(to_string<public_key_t>(from_string<public_key_t>(key)) == key);
}

TEST_CASE("Ed25519 signatures are verified", "[ed25519]") {
    public_key_t pub;
    private_key_t priv;
    int ret = ed25519_create_keypair(&priv, &pub);
    REQUIRE(setenv("PUBLIC_KEY", to_string<public_key_t>(pub).c_str(), 1) == 0);
    REQUIRE(ret == ED25519_SUCCESS);

    signature_t sig;
    std::string test_message = "Hammer Time!";
    ed25519_sign(&sig, reinterpret_cast<const unsigned char*>(test_message.c_str()), test_message.size(), &pub, &priv);
    std::string signature = to_string<signature_t>(sig);

    REQUIRE(is_valid_signature(signature, test_message));
}
