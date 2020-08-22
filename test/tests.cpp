#define CATCH_CONFIG_MAIN // This tells the Catch2 header to generate a main

#include <string>

#include <hamarr/hamarr.hpp>

#include "catch.hpp"

using namespace std::string_literals;

// Formatting strings
TEST_CASE("Uppercase and lowercase std::string conversion", "[formatting][case][string]")
{
  REQUIRE(hmr::format::to_upper("Hello, World!"s) == "HELLO, WORLD!"s);
  REQUIRE(hmr::format::to_lower("Hello, World!"s) == "hello, world!"s);
}

// Hex encoding/decoding strings
TEST_CASE("Hex encode/decode std::string", "[encoding][hex][string]")
{
  // Padded - uppercase/lowercase/mixed case
  REQUIRE(hmr::hex::encode("Hello, World!"s) == "48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21"s);
  REQUIRE(hmr::hex::decode("48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21"s) == "Hello, World!"s);
  REQUIRE(hmr::hex::decode("48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21"s) == "Hello, World!"s);
  REQUIRE(hmr::hex::decode("48 65 6C 6C 6F 2C 20 57 6f 72 6c 64 21"s) == "Hello, World!"s);

  // Un-padded - uppercase/lowercase/mixed case
  REQUIRE(hmr::hex::encode("Hello, World!"s, false) == "48656C6C6F2C20576F726C6421"s);
  REQUIRE(hmr::hex::decode("48656C6C6F2C20576F726C6421"s) == "Hello, World!"s);
  REQUIRE(hmr::hex::decode("48656c6c6f2c20576f726c6421"s) == "Hello, World!"s);

  // Partially padded - uppercase/lowercase/mixed case
  REQUIRE(hmr::hex::decode("48656C6C6F2C 20 57 6F 726C6421"s) == "Hello, World!"s);
  REQUIRE(hmr::hex::decode("48656c6c6f2c 20 57 6f 726c6421"s) == "Hello, World!"s);
  REQUIRE(hmr::hex::decode("48656C6C6F2C 20 57 6f 726c6421"s) == "Hello, World!"s);

  // Failures
  REQUIRE(hmr::hex::decode("Invalid hex input"s) == std::string{}); // Invalid hex chars
  REQUIRE(hmr::hex::decode("11 22 3"s) == std::string{}); // Missing nibble
  REQUIRE(hmr::hex::decode("11223"s) == std::string{}); // Missing nibble
  REQUIRE(hmr::hex::decode("1 122 3"s) == std::string{}); // Missing nibble
}

// Hex encoding/decoding integrals
TEST_CASE("Hex encode/decode integral", "[encoding][hex][integral]")
{
  // uint8_t
  REQUIRE(hmr::hex::encode(uint8_t{18}) == "12"s);
  REQUIRE(hmr::hex::decode<uint8_t>("12"s) == uint8_t{18});

  // uint16_t - padded/un-padded
  REQUIRE(hmr::hex::encode(uint16_t{4660}) == "12 34"s);
  REQUIRE(hmr::hex::decode<uint16_t>("12 34"s) == uint16_t{4660});
  REQUIRE(hmr::hex::encode(uint16_t{4660}, false) == "1234"s);
  REQUIRE(hmr::hex::decode<uint16_t>("1234"s) == uint16_t{4660});

  // uint32_t - padded/un-padded
  REQUIRE(hmr::hex::encode(uint32_t{305419896}) == "12 34 56 78"s);
  REQUIRE(hmr::hex::decode<uint32_t>("12 34 56 78"s) == uint32_t{305419896});
  REQUIRE(hmr::hex::encode(uint32_t{305419896}, false) == "12345678"s);
  REQUIRE(hmr::hex::decode<uint32_t>("12345678"s) == uint32_t{305419896});

  // uint64_t - padded/un-padded
  REQUIRE(hmr::hex::encode(uint64_t{1311768467463790320}) == "12 34 56 78 9A BC DE F0"s);
  REQUIRE(hmr::hex::decode<uint64_t>("12 34 56 78 9A BC DE F0"s) == uint64_t{1311768467463790320});
  REQUIRE(hmr::hex::encode(uint64_t{1311768467463790320}, false) == "123456789ABCDEF0"s);
  REQUIRE(hmr::hex::decode<uint64_t>("123456789ABCDEF0"s) == uint64_t{1311768467463790320});

  // int - padded/un-padded
  REQUIRE(sizeof(int) == 4);
  REQUIRE(hmr::hex::encode(256) == "00 00 01 00"s);
  REQUIRE(hmr::hex::decode<int>("00 00 01 00"s) == 256);
  REQUIRE(hmr::hex::encode(256, false) == "00000100"s);
  REQUIRE(hmr::hex::decode<int>("00000100"s) == 256);

  // Failures
  REQUIRE(hmr::hex::decode<uint8_t>("FF FF") == uint8_t{}); // Too many bytes for the requested return type
  REQUIRE(hmr::hex::decode<uint16_t>("FF FF FF") == uint16_t{}); // Too many bytes for the requested return type
  REQUIRE(hmr::hex::decode<uint32_t>("FF FF FF FF FF FF") == uint32_t{}); // Too many bytes for the requested return type
  REQUIRE(hmr::hex::decode<uint64_t>("AA BB CC DD EE FF 00 11 22 33 44 55 66 77 88 99") == uint64_t{}); // Too many bytes for the requested return type
}

// Binary encoding/decoding strings
TEST_CASE("Binary encode/decode std::string", "[encoding][binary][string]")
{
  // Padded
  REQUIRE(hmr::binary::encode("Hello, World!"s) == "01001000 01100101 01101100 01101100 01101111 00101100 00100000 01010111 01101111 01110010 01101100 01100100 00100001"s);
  REQUIRE(hmr::binary::decode("01001000 01100101 01101100 01101100 01101111 00101100 00100000 01010111 01101111 01110010 01101100 01100100 00100001"s) == "Hello, World!"s);

  // Un-padded
  REQUIRE(hmr::binary::encode("Hello, World!"s, false) == "01001000011001010110110001101100011011110010110000100000010101110110111101110010011011000110010000100001"s);
  REQUIRE(hmr::binary::decode("01001000011001010110110001101100011011110010110000100000010101110110111101110010011011000110010000100001"s) == "Hello, World!"s);
}

// Binary encoding/decoding integrals
TEST_CASE("Binary encode/decode integral", "[encoding][binary][integral]")
{
  // uint8_t
  REQUIRE(hmr::hex::encode(uint8_t{18}) == "12"s);
  REQUIRE(hmr::hex::decode<uint8_t>("12"s) == uint8_t{18});

  // uint16_t
  REQUIRE(hmr::hex::encode(uint16_t{4660}) == "12 34"s);
  REQUIRE(hmr::hex::decode<uint16_t>("12 34"s) == uint16_t{4660});

  // uint32_t
  REQUIRE(hmr::hex::encode(uint32_t{305419896}) == "12 34 56 78"s);
  REQUIRE(hmr::hex::decode<uint32_t>("12 34 56 78"s) == uint32_t{305419896});

  // uint64_t
  REQUIRE(hmr::hex::encode(uint64_t{1311768467463790320}) == "12 34 56 78 9A BC DE F0"s);
  REQUIRE(hmr::hex::decode<uint64_t>("12 34 56 78 9A BC DE F0"s) == uint64_t{1311768467463790320});

  // int
  REQUIRE(sizeof(int) == 4);
  REQUIRE(hmr::hex::encode(256) == "00 00 01 00"s);
  REQUIRE(hmr::hex::decode<int>("00 00 01 00"s) == 256);
}
