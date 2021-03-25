#pragma once
#include <string>

struct SignatureInterface {
    virtual bool is_valid_signature(std::string signature, std::string signed_message) const = 0;
};

struct Ed25519 : public SignatureInterface {
    virtual bool is_valid_signature(std::string signature, std::string signed_message) const override;
};

void hex_to_bytes(std::string hex, unsigned char* buff);

template<class T>
T from_string(std::string str) {
    T t;
    hex_to_bytes(str, t.data);
    return t;
}
