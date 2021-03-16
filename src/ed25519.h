#pragma once
#include <string>

void hex_to_bytes(std::string hex, unsigned char* buff);
bool is_valid_signature(std::string signature, std::string signed_message);

template<class T>
T from_string(std::string str) {
    T t;
    hex_to_bytes(str, t.data);
    return t;
}
