#pragma once

#include <string>
#include <string_view>
#include <vector>


namespace hmr::crypto
{

using namespace std::string_literals;

std::string aes_ecb_encrypt_block(std::string_view input, std::string_view key);
std::string aes_ecb_decrypt_block(std::string_view input, std::string_view key);

std::string aes_ecb_encrypt(std::string_view input, std::string_view key);
std::string aes_ecb_decrypt(std::string_view input, std::string_view key, bool remove_padding = true);

std::string aes_cbc_encrypt(std::string_view input, std::string_view key, std::string const &iv = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"s);
std::string aes_cbc_decrypt(std::string_view input, std::string_view key, std::string const &iv = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"s, bool remove_padding = true);

} // namespace hmr::crypto
