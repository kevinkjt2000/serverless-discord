#include <cstring>
#include <string>

#include <ed25519/ed25519.h>

#include "ed25519.h"
#include "utils.h"

void hex_to_bytes(std::string hex, unsigned char* buff) {
    for(std::size_t i = 0; i < hex.size(); i += 2) {
        buff[i/2] = std::stoi(hex.substr(i, 2), 0, 16);
    }
}

bool Ed25519::is_valid_signature(std::string signature, std::string signed_message) const {
    auto pub = from_string<public_key_t>(getEnvVar("PUBLIC_KEY"));
    auto sig = from_string<signature_t>(signature);
    int ret = ed25519_verify(&sig, reinterpret_cast<unsigned const char*>(signed_message.c_str()), signed_message.size(), &pub);
    return ret == ED25519_SIGNATURE_VALID;
}
