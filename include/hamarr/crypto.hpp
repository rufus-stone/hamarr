#pragma once

#include <string>
#include <string_view>
#include <vector>


namespace hmr::crypto
{

using namespace std::string_literals;

auto aes_ecb_encrypt_block(std::string_view input, std::string_view key) -> std::string;
auto aes_ecb_decrypt_block(std::string_view input, std::string_view key) -> std::string;

auto aes_ecb_encrypt(std::string_view input, std::string_view key) -> std::string;
auto aes_ecb_decrypt(std::string_view input, std::string_view key, bool remove_padding = true) -> std::string;

auto aes_cbc_encrypt(std::string_view input, std::string_view key, std::string const &iv = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"s) -> std::string;
auto aes_cbc_decrypt(std::string_view input, std::string_view key, std::string const &iv = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"s, bool remove_padding = true) -> std::string;

// Hashing
auto md5_raw(std::string_view input) -> std::string;
auto md5(std::string_view input) -> std::string;
auto sha1_raw(std::string_view input) -> std::string;
auto sha1(std::string_view input) -> std::string;
auto sha256_raw(std::string_view input) -> std::string;
auto sha256(std::string_view input) -> std::string;

} // namespace hmr::crypto
