#define CATCH_CONFIG_MAIN // This tells the Catch2 header to generate a main

#include <string>
#include <string_view>
#include <vector>

#include <hamarr/format.hpp>
#include <hamarr/hex.hpp>
#include <hamarr/binary.hpp>
#include <hamarr/base64.hpp>
#include <hamarr/url.hpp>
#include <hamarr/prng.hpp>
#include <hamarr/bitwise.hpp>
#include <hamarr/analysis.hpp>
#include <hamarr/pkcs7.hpp>
#include <hamarr/serialisation.hpp>
#include <hamarr/uuid.hpp>
#include <hamarr/crypto.hpp>

#include "catch.hpp"

using namespace std::string_literals;

// hmr::format
TEST_CASE("hmr::format", "[formatting]")
{
  auto const input = "Hello,\nWorld!"s;
  REQUIRE(hmr::format::to_upper(input) == "HELLO,\nWORLD!"s);
  REQUIRE(hmr::format::to_lower(input) == "hello,\nworld!"s);

  REQUIRE(hmr::format::escape(input) == "Hello,\\nWorld!"s);
  REQUIRE(hmr::format::unescape("Hello,\\nWorld!"s) == input);

  REQUIRE(hmr::format::split(input, '\n') == std::vector<std::string>{"Hello,", "World!"});
}

// hmr::hex
TEST_CASE("hmr::hex", "[encoding][hex]")
{
  auto const input = "Hello, World!"s;

  // Padded - uppercase/lowercase/mixed case
  REQUIRE(hmr::hex::encode(input) == "48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21"s);
  REQUIRE(hmr::hex::decode("48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21"s) == input);
  REQUIRE(hmr::hex::decode("48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21"s) == input);
  REQUIRE(hmr::hex::decode("48 65 6C 6C 6F 2C 20 57 6f 72 6c 64 21"s) == input);

  // Un-padded - uppercase/lowercase/mixed case
  REQUIRE(hmr::hex::encode(input, false) == "48656C6C6F2C20576F726C6421"s);
  REQUIRE(hmr::hex::decode("48656C6C6F2C20576F726C6421"s) == input);
  REQUIRE(hmr::hex::decode("48656c6c6f2c20576f726c6421"s) == input);

  // Partially padded - uppercase/lowercase/mixed case
  REQUIRE(hmr::hex::decode("48656C6C6F2C 20 57 6F 726C6421"s) == input);
  REQUIRE(hmr::hex::decode("48656c6c6f2c 20 57 6f 726c6421"s) == input);
  REQUIRE(hmr::hex::decode("48656C6C6F2C 20 57 6f 726c6421"s) == input);

  // Failures
  REQUIRE_THROWS(hmr::hex::decode("Invalid hex input"s) == std::string{}); // Invalid hex chars
  REQUIRE_THROWS(hmr::hex::decode("11 22 3"s) == std::string{}); // Missing nibble
  REQUIRE_THROWS(hmr::hex::decode("11223"s) == std::string{}); // Missing nibble
  REQUIRE_THROWS(hmr::hex::decode("1 122 3"s) == std::string{}); // Missing nibble

  // uint8_t
  REQUIRE(hmr::hex::encode(uint8_t{18}) == "12"s);
  REQUIRE(hmr::hex::decode<uint8_t>("12"s) == uint8_t{18});

  // uint16_t - padded/un-padded
  REQUIRE(hmr::hex::encode(uint16_t{4660}) == "12 34"s);
  REQUIRE(hmr::hex::encode(uint16_t{4660}, false) == "1234"s);
  REQUIRE(hmr::hex::decode<uint16_t>("12 34"s) == uint16_t{4660});
  REQUIRE(hmr::hex::decode<uint16_t>("1234"s) == uint16_t{4660});

  // uint32_t - padded/un-padded
  REQUIRE(hmr::hex::encode(uint32_t{305419896}) == "12 34 56 78"s);
  REQUIRE(hmr::hex::encode(uint32_t{305419896}, false) == "12345678"s);
  REQUIRE(hmr::hex::decode<uint32_t>("12 34 56 78"s) == uint32_t{305419896});
  REQUIRE(hmr::hex::decode<uint32_t>("12345678"s) == uint32_t{305419896});

  // uint64_t - padded/un-padded
  REQUIRE(hmr::hex::encode(uint64_t{1311768467463790320}) == "12 34 56 78 9A BC DE F0"s);
  REQUIRE(hmr::hex::encode(uint64_t{1311768467463790320}, false) == "123456789ABCDEF0"s);
  REQUIRE(hmr::hex::decode<uint64_t>("12 34 56 78 9A BC DE F0"s) == uint64_t{1311768467463790320});
  REQUIRE(hmr::hex::decode<uint64_t>("123456789ABCDEF0"s) == uint64_t{1311768467463790320});

  // int - padded/un-padded
  REQUIRE(sizeof(int) == 4);
  REQUIRE(hmr::hex::encode(256) == "00 00 01 00"s);
  REQUIRE(hmr::hex::encode(256, false) == "00000100"s);
  REQUIRE(hmr::hex::decode<int>("00 00 01 00"s) == 256);
  REQUIRE(hmr::hex::decode<int>("00000100"s) == 256);

  // Failures
  REQUIRE_THROWS(hmr::hex::decode<uint8_t>("FF FF"s) == uint8_t{}); // Too many bytes for the requested return type
  REQUIRE_THROWS(hmr::hex::decode<uint16_t>("FF FF FF"s) == uint16_t{}); // Too many bytes for the requested return type
  REQUIRE_THROWS(hmr::hex::decode<uint32_t>("FF FF FF FF FF FF"s) == uint32_t{}); // Too many bytes for the requested return type
  REQUIRE_THROWS(hmr::hex::decode<uint64_t>("AA BB CC DD EE FF 00 11 22 33 44 55 66 77 88 99"s) == uint64_t{}); // Too many bytes for the requested return type
}

// hmr::binary
TEST_CASE("hmr::binary", "[encoding][binary]")
{
  // String-like
  REQUIRE(hmr::binary::encode("Hello, World!"s) == "01001000 01100101 01101100 01101100 01101111 00101100 00100000 01010111 01101111 01110010 01101100 01100100 00100001"s);
  REQUIRE(hmr::binary::encode("Hello, World!"s, false) == "01001000011001010110110001101100011011110010110000100000010101110110111101110010011011000110010000100001"s);
  REQUIRE(hmr::binary::decode("01001000 01100101 01101100 01101100 01101111 00101100 00100000 01010111 01101111 01110010 01101100 01100100 00100001"s) == "Hello, World!"s);
  REQUIRE(hmr::binary::decode("01001000011001010110110001101100011011110010110000100000010101110110111101110010011011000110010000100001"s) == "Hello, World!"s);

  // uint8_t
  REQUIRE(hmr::binary::encode(uint8_t{18}) == "00010010"s);
  REQUIRE(hmr::binary::decode<uint8_t>("11110000"s) == uint8_t{240});

  // uint16_t
  REQUIRE(hmr::binary::encode(uint16_t{4660}) == "00010010 00110100"s);
  REQUIRE(hmr::binary::encode(uint16_t{4660}, false) == "0001001000110100"s);
  REQUIRE(hmr::binary::decode<uint16_t>("00001111 11110000"s) == uint16_t{4080});
  REQUIRE(hmr::binary::decode<uint16_t>("0000111111110000"s) == uint16_t{4080});

  // int16_t
  REQUIRE(hmr::binary::encode(int16_t{-256}) == "11111111 00000000"s);
  REQUIRE(hmr::binary::decode<int16_t>("11111111 00000000"s) == int16_t{-256});

  // uint32_t
  REQUIRE(hmr::binary::encode(uint32_t{305419896}) == "00010010 00110100 01010110 01111000"s);
  REQUIRE(hmr::binary::encode(uint32_t{305419896}, false) == "00010010001101000101011001111000"s);
  REQUIRE(hmr::binary::decode<uint32_t>("11110000 00001111 11110000 00001111"s) == uint32_t{4027576335});
  REQUIRE(hmr::binary::decode<uint32_t>("11110000000011111111000000001111"s) == uint32_t{4027576335});

  // uint64_t
  REQUIRE(hmr::binary::encode(uint64_t{1311768467463790320}) == "00010010 00110100 01010110 01111000 10011010 10111100 11011110 11110000"s);
  REQUIRE(hmr::binary::encode(uint64_t{1311768467463790320}, false) == "0001001000110100010101100111100010011010101111001101111011110000"s);
  REQUIRE(hmr::binary::decode<uint64_t>("00001111 11110000 00001111 11110000 00001111 11110000 00001111 11110000"s) == uint64_t{1148435428713435120});
  REQUIRE(hmr::binary::decode<uint64_t>("0000111111110000000011111111000000001111111100000000111111110000"s) == uint64_t{1148435428713435120});

  // int
  REQUIRE(sizeof(int) == 4);
  REQUIRE(hmr::binary::encode(-12345) == "11111111 11111111 11001111 11000111"s);
  REQUIRE(hmr::binary::encode(-12345, false) == "11111111111111111100111111000111"s);
  REQUIRE(hmr::binary::decode<int>("11111111 11111111 00000000 00000000"s) == -65536);
  REQUIRE(hmr::binary::decode<int>("11111111111111110000000000000000"s) == -65536);

  // Failures
  REQUIRE_THROWS(hmr::binary::decode<uint8_t>("11110000 11110000"s) == uint8_t{}); // Too many bits for requested result type
  REQUIRE_THROWS(hmr::binary::decode<uint16_t>("11110000"s) == uint16_t{}); // Not enough bits for requested result type
  REQUIRE_THROWS(hmr::binary::decode<uint8_t>("1010"s) == uint8_t{}); // Number of characters not a multiple of 8
  REQUIRE_THROWS(hmr::binary::decode<int>("11110000 !INVALID!!CHARS! 00001111"s) == int{}); // Invalid input characters
}

// hmr::base64
TEST_CASE("hmr::base64", "[encoding][base64]")
{
  auto const input = "Hello, World!"s;

  // Standard base64 alphabet
  REQUIRE(hmr::base64::encode(input) == "SGVsbG8sIFdvcmxkIQ=="s);
  REQUIRE(hmr::base64::decode("SGVsbG8sIFdvcmxkIQ=="s) == input);

  REQUIRE(hmr::base64::encode(hmr::hex::decode("1f 8b 08 00 00 00 00 00 00 00 05 80 21 0d 00 00 10 02 4b 7e 05 f6 c3 60 a0 be b8 9d 3a e7 01 9a 63 4e 27 07 00 00 00"s)) == "H4sIAAAAAAAAAAWAIQ0AABACS34F9sNgoL64nTrnAZpjTicHAAAA"s);
  REQUIRE(hmr::base64::decode("H4sIAAAAAAAAAAWAIQ0AABACS34F9sNgoL64nTrnAZpjTicHAAAA"s) == hmr::hex::decode("1f 8b 08 00 00 00 00 00 00 00 05 80 21 0d 00 00 10 02 4b 7e 05 f6 c3 60 a0 be b8 9d 3a e7 01 9a 63 4e 27 07 00 00 00"s));

  REQUIRE(hmr::base64::encode(hmr::hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd"s)) == "MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9"s);
  REQUIRE(hmr::base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9"s) == hmr::hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd"s));

  REQUIRE(hmr::base64::encode(hmr::hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe"s)) == "MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/g=="s);
  REQUIRE(hmr::base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/g=="s) == hmr::hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe"s));

  REQUIRE(hmr::base64::encode(hmr::hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff"s)) == "MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/v8="s);
  REQUIRE(hmr::base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/v8="s) == hmr::hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff"s));

  // Custom base64 alphabet
  REQUIRE(hmr::base64::encode(input, "abcdefgh0123456789ijklmnopqrstuvwxyz=/ABCDEFGHIJKLMNOPQRSTUVWXYZ+"s) == "iglGrgWG0ftJsAL=08++"s);
  REQUIRE(hmr::base64::decode("iglGrgWG0ftJsAL=08++"s, "abcdefgh0123456789ijklmnopqrstuvwxyz=/ABCDEFGHIJKLMNOPQRSTUVWXYZ+"s) == input);

  // Failures
  REQUIRE_THROWS(hmr::base64::encode("This won't work"s, "Thisalphabetistoosmall"s) == std::string{}); // Custom alphabet is too short
  REQUIRE_THROWS(hmr::base64::decode("This won't work"s, "Thisalphabetistoosmall"s) == std::string{}); // Custom alphabet is too short
  REQUIRE_THROWS(hmr::base64::encode("This won't work"s, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123455555+/="s) == std::string{}); // Custom alphabet contains repeat chars ('5')
  REQUIRE_THROWS(hmr::base64::decode("This won't work"s, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123455555+/="s) == std::string{}); // Custom alphabet contains repeat chars ('5')
  REQUIRE_THROWS(hmr::base64::decode("This won't work"s) == std::string{}); // Invalid base64 chars in input
}

// hmr::url
TEST_CASE("hmr::url", "[encoding][url]")
{
  // RFC 3986 compliant
  REQUIRE(hmr::url::encode(hmr::hex::decode("10 33 55 77 99 AA BB DD FF"s)) == "%103Uw%C2%99%C2%AA%C2%BB%C3%9D%C3%BF"s);
  REQUIRE(hmr::url::decode("%103Uw%C2%99%C2%AA%C2%BB%C3%9D%C3%BF"s) == hmr::hex::decode("10 33 55 77 99 AA BB DD FF"s));

  REQUIRE(hmr::url::encode(hmr::hex::decode("1f 8b 08 00 00 00 00 00 00 00 05 80 21 0d 00 00 10 02 4b 7e 05 f6 c3 60 a0 be b8 9d 3a e7 01 9a 63 4e 27 07 00 00 00"s)) == "%1F%C2%8B%08%00%00%00%00%00%00%00%05%C2%80%21%0D%00%00%10%02K~%05%C3%B6%C3%83%60%C2%A0%C2%BE%C2%B8%C2%9D%3A%C3%A7%01%C2%9AcN%27%07%00%00%00"s);
  REQUIRE(hmr::url::decode("%1F%C2%8B%08%00%00%00%00%00%00%00%05%C2%80%21%0D%00%00%10%02K~%05%C3%B6%C3%83%60%C2%A0%C2%BE%C2%B8%C2%9D%3A%C3%A7%01%C2%9AcN%27%07%00%00%00"s) == hmr::hex::decode("1f 8b 08 00 00 00 00 00 00 00 05 80 21 0d 00 00 10 02 4b 7e 05 f6 c3 60 a0 be b8 9d 3a e7 01 9a 63 4e 27 07 00 00 00"s));

  REQUIRE(hmr::url::encode(hmr::hex::decode("00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f 30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f 40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f 50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f 60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f 80 81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f 90 91 92 93 94 95 96 97 98 99 9a 9b 9c 9d 9e 9f a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 aa ab ac ad ae af b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 ba bb bc bd be bf c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 ca cb cc cd ce cf d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 da db dc dd de df e0 e1 e2 e3 e4 e5 e6 e7 e8 e9 ea eb ec ed ee ef f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff"s)) == "%00%01%02%03%04%05%06%07%08%09%0A%0B%0C%0D%0E%0F%10%11%12%13%14%15%16%17%18%19%1A%1B%1C%1D%1E%1F%20%21%22%23%24%25%26%27%28%29%2A%2B%2C-.%2F0123456789%3A%3B%3C%3D%3E%3F%40ABCDEFGHIJKLMNOPQRSTUVWXYZ%5B%5C%5D%5E_%60abcdefghijklmnopqrstuvwxyz%7B%7C%7D~%7F%C2%80%C2%81%C2%82%C2%83%C2%84%C2%85%C2%86%C2%87%C2%88%C2%89%C2%8A%C2%8B%C2%8C%C2%8D%C2%8E%C2%8F%C2%90%C2%91%C2%92%C2%93%C2%94%C2%95%C2%96%C2%97%C2%98%C2%99%C2%9A%C2%9B%C2%9C%C2%9D%C2%9E%C2%9F%C2%A0%C2%A1%C2%A2%C2%A3%C2%A4%C2%A5%C2%A6%C2%A7%C2%A8%C2%A9%C2%AA%C2%AB%C2%AC%C2%AD%C2%AE%C2%AF%C2%B0%C2%B1%C2%B2%C2%B3%C2%B4%C2%B5%C2%B6%C2%B7%C2%B8%C2%B9%C2%BA%C2%BB%C2%BC%C2%BD%C2%BE%C2%BF%C3%80%C3%81%C3%82%C3%83%C3%84%C3%85%C3%86%C3%87%C3%88%C3%89%C3%8A%C3%8B%C3%8C%C3%8D%C3%8E%C3%8F%C3%90%C3%91%C3%92%C3%93%C3%94%C3%95%C3%96%C3%97%C3%98%C3%99%C3%9A%C3%9B%C3%9C%C3%9D%C3%9E%C3%9F%C3%A0%C3%A1%C3%A2%C3%A3%C3%A4%C3%A5%C3%A6%C3%A7%C3%A8%C3%A9%C3%AA%C3%AB%C3%AC%C3%AD%C3%AE%C3%AF%C3%B0%C3%B1%C3%B2%C3%B3%C3%B4%C3%B5%C3%B6%C3%B7%C3%B8%C3%B9%C3%BA%C3%BB%C3%BC%C3%BD%C3%BE%C3%BF"s);
  REQUIRE(hmr::url::decode("%00%01%02%03%04%05%06%07%08%09%0A%0B%0C%0D%0E%0F%10%11%12%13%14%15%16%17%18%19%1A%1B%1C%1D%1E%1F%20%21%22%23%24%25%26%27%28%29%2A%2B%2C-.%2F0123456789%3A%3B%3C%3D%3E%3F%40ABCDEFGHIJKLMNOPQRSTUVWXYZ%5B%5C%5D%5E_%60abcdefghijklmnopqrstuvwxyz%7B%7C%7D~%7F%C2%80%C2%81%C2%82%C2%83%C2%84%C2%85%C2%86%C2%87%C2%88%C2%89%C2%8A%C2%8B%C2%8C%C2%8D%C2%8E%C2%8F%C2%90%C2%91%C2%92%C2%93%C2%94%C2%95%C2%96%C2%97%C2%98%C2%99%C2%9A%C2%9B%C2%9C%C2%9D%C2%9E%C2%9F%C2%A0%C2%A1%C2%A2%C2%A3%C2%A4%C2%A5%C2%A6%C2%A7%C2%A8%C2%A9%C2%AA%C2%AB%C2%AC%C2%AD%C2%AE%C2%AF%C2%B0%C2%B1%C2%B2%C2%B3%C2%B4%C2%B5%C2%B6%C2%B7%C2%B8%C2%B9%C2%BA%C2%BB%C2%BC%C2%BD%C2%BE%C2%BF%C3%80%C3%81%C3%82%C3%83%C3%84%C3%85%C3%86%C3%87%C3%88%C3%89%C3%8A%C3%8B%C3%8C%C3%8D%C3%8E%C3%8F%C3%90%C3%91%C3%92%C3%93%C3%94%C3%95%C3%96%C3%97%C3%98%C3%99%C3%9A%C3%9B%C3%9C%C3%9D%C3%9E%C3%9F%C3%A0%C3%A1%C3%A2%C3%A3%C3%A4%C3%A5%C3%A6%C3%A7%C3%A8%C3%A9%C3%AA%C3%AB%C3%AC%C3%AD%C3%AE%C3%AF%C3%B0%C3%B1%C3%B2%C3%B3%C3%B4%C3%B5%C3%B6%C3%B7%C3%B8%C3%B9%C3%BA%C3%BB%C3%BC%C3%BD%C3%BE%C3%BF"s) == hmr::hex::decode("00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f 30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f 40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f 50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f 60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f 80 81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f 90 91 92 93 94 95 96 97 98 99 9a 9b 9c 9d 9e 9f a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 aa ab ac ad ae af b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 ba bb bc bd be bf c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 ca cb cc cd ce cf d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 da db dc dd de df e0 e1 e2 e3 e4 e5 e6 e7 e8 e9 ea eb ec ed ee ef f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff"s));

  // Lazy
  REQUIRE(hmr::url::encode(hmr::hex::decode("10 33 55 77 99 AA BB DD FF"s), true) == "%103Uw%99%AA%BB%DD%FF"s);
  REQUIRE(hmr::url::decode("%103Uw%99%AA%BB%DD%FF"s, true) == hmr::hex::decode("10 33 55 77 99 AA BB DD FF"s));

  // Failures
  REQUIRE_THROWS(hmr::url::decode("Invalid percent-encoded hex sequences %ZZ %JJ"s) == std::string{});
  REQUIRE_THROWS(hmr::url::decode("Missing data %C2%"s) == std::string{});
  REQUIRE_THROWS(hmr::url::decode("Invalid UTF-8 -> ASCII conversion %C4%11"s) == std::string{});
  REQUIRE_THROWS(hmr::url::decode("Invalid second half of UTF-8 -> ASCII conversion %C2%ZZ"s) == std::string{});
  REQUIRE_THROWS(hmr::url::decode("Unprintable chars mixed in %1\x98"s) == std::string{});
}

// hmr::prng
TEST_CASE("hmr::prng", "[prng]")
{
  auto const rnd_int = hmr::prng::number_between<unsigned int>(0, 10);
  REQUIRE(rnd_int >= 0);
  REQUIRE(rnd_int <= 10);

  auto const rnd_dbl = hmr::prng::number_between<double>(-1, 1);
  REQUIRE(rnd_dbl >= -1);
  REQUIRE(rnd_dbl <= 1);

  REQUIRE(hmr::prng::bytes(16).size() == 16);
}

// hmr::bitwise
TEST_CASE("hmr::bitwise", "[obfuscation][xor][bitshift][rotate]")
{
  auto const input = "Hello, World!"s;

  // String as key
  REQUIRE(hmr::bitwise::xor_with_key(input, "great key"s) == hmr::hex::decode("2F 17 09 0D 1B 0C 4B 32 16 15 1E 01 40"s));
  REQUIRE(hmr::bitwise::xor_with_key("12345678"s, "ABC\xFF"s) == hmr::hex::decode("70 70 70 cb 74 74 74 c7"s));

  // Char as key
  REQUIRE(hmr::bitwise::xor_with_key(input, 'c') == hmr::hex::decode("2b 06 0f 0f 0c 4f 43 34 0c 11 0f 07 42"s));

  // Integral as key
  REQUIRE(hmr::bitwise::xor_with_key(input, 1) == hmr::hex::decode("49 64 6d 6d 6e 2d 21 56 6e 73 6d 65 20"s));
  REQUIRE(hmr::bitwise::xor_with_key(input, uint8_t{0x33}) == hmr::hex::decode("7b 56 5f 5f 5c 1f 13 64 5c 41 5f 57 12"s));

  // Rolling XOR
  REQUIRE(hmr::bitwise::xor_rolling("12345678"s) == hmr::hex::decode("31 03 01 07 01 03 01 0F"s));
  REQUIRE(hmr::bitwise::xor_rolling("12345678"s, hmr::bitwise::xor_differential::output) == hmr::hex::decode("31 03 30 04 31 07 30 08"s));

  // Counter XOR
  REQUIRE(hmr::bitwise::xor_counter(input) == hmr::hex::decode("48 64 6E 6F 6B 29 26 50 67 7B 66 6F 2D"s));
  REQUIRE(hmr::bitwise::xor_counter(input, '\xAA') == hmr::hex::decode("E2 CE C0 C1 C1 83 90 E6 DD C1 D8 D1 97"s));
  REQUIRE(hmr::bitwise::xor_counter(input, '\xEF', 6) == hmr::hex::decode("A7 90 97 6D 68 21 33 4E 70 57 47 55 16"s));
  REQUIRE(hmr::bitwise::xor_counter(input, '\x04', -2) == hmr::hex::decode("4C 67 6C 92 93 D6 D8 A1 9B 80 9C 8A CD"s));

  // Bit shift
  REQUIRE(hmr::binary::encode(hmr::bitwise::shift_left(hmr::binary::decode("00000001"s))) == "00000010"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::shift_left(hmr::binary::decode("00000001"s), 7)) == "10000000"s);

  REQUIRE(hmr::binary::encode(hmr::bitwise::shift_right(hmr::binary::decode("00000001"s))) == "00000000"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::shift_right(hmr::binary::decode("01001001"s), 6)) == "00000001"s);

  REQUIRE(hmr::binary::encode(hmr::bitwise::shift_right(input, 6)) == "00000001 00000001 00000001 00000001 00000001 00000000 00000000 00000001 00000001 00000001 00000001 00000001 00000000"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::shift_right("Another"s, 2)) == "00010000 00011011 00011011 00011101 00011010 00011001 00011100"s);

  REQUIRE(hmr::binary::encode(hmr::bitwise::shift_left<uint16_t>(255, 8)) == "11111111 00000000"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::shift_left<uint64_t>(7890123499999, 8)) == "00000000 00000111 00101101 00010000 00000010 00101001 11011111 00000000"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::shift_left<int8_t>(-27, 3)) == "00101000"s);

  REQUIRE(hmr::binary::encode(hmr::bitwise::shift_right<uint8_t>(16, 2)) == "00000100"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::shift_right<int16_t>(-1000, 3)) == "11111111 10000011"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::shift_right(int16_t(-1000), 3)) == "11111111 10000011"s);

  // Bit rotate
  REQUIRE(hmr::binary::encode(hmr::bitwise::rotate_left(input, 5)) == "00001001 10101100 10001101 10001101 11101101 10000101 00000100 11101010 11101101 01001110 10001101 10001100 00100100"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::rotate_left(hmr::binary::decode("01100111 11100010 11110000 00001111"s), 4)) == "01110110 00101110 00001111 11110000"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::rotate_right(input, 12)) == "10000100 01010110 11000110 11000110 11110110 11000010 00000010 01110101 11110110 00100111 11000110 01000110 00010010"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::rotate_right(hmr::binary::decode("01100111 11100010 11110000 00001111"s), 4)) == "01110110 00101110 00001111 11110000"s);

  // Bit rotate with carry through
  REQUIRE(hmr::binary::encode(hmr::bitwise::rotate_left(input, 5, hmr::bitwise::carry_through::enabled)) == "00001100 10101101 10001101 10001101 11100101 10000100 00001010 11101101 11101110 01001101 10001100 10000100 00101001"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::rotate_left(hmr::binary::decode("10000000 00000000 11111111 00000001"s), 20, hmr::bitwise::carry_through::enabled)) == "11110000 00011000 00000000 00001111"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::rotate_right(input, 2, hmr::bitwise::carry_through::enabled)) == "01010010 00011001 01011011 00011011 00011011 11001011 00001000 00010101 11011011 11011100 10011011 00011001 00001000"s);
  REQUIRE(hmr::binary::encode(hmr::bitwise::rotate_right(hmr::binary::decode("10001000 01110111 00110010"s), 7, hmr::bitwise::carry_through::enabled)) == "01100101 00010000 11101110"s);
}

// hmr::analysis
TEST_CASE("hmr::analysis", "[analysis]")
{
  auto const input = "Hello, World!"s;

  // Hamming distance
  REQUIRE(hmr::analysis::hamming_distance("this is a test"s, "wokka wokka!!!"s) == 37);
  REQUIRE(hmr::analysis::hamming_distance("this is a testEXTRASTUFF"s, "wokka wokka!!!"s) == 37);

  // Character frequency analysis
  auto const freqs_1 = hmr::analysis::character_frequency(input);
  REQUIRE(freqs_1['l'] == 3);
  REQUIRE(freqs_1['o'] == 2);

  auto const freqs_2 = hmr::analysis::character_frequency("Mix OF upPer AND LOWER"s, hmr::analysis::case_sensitivity::disabled);
  REQUIRE(freqs_2['p'] == 2);
  REQUIRE(freqs_2['e'] == 2);

  auto const freqs_3 = hmr::analysis::character_frequency("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Curabitur pretium tincidunt lacus. Nulla gravida orci a odio. Nullam varius, turpis et commodo pharetra, est eros bibendum elit, nec luctus magna felis sollicitudin mauris. Integer in mauris eu nibh euismod gravida. Duis ac tellus et risus vulputate vehicula. Donec lobortis risus a elit. Etiam tempor. Ut ullamcorper, ligula eu tempor congue, eros est euismod turpis, id tincidunt sapien risus a quam. Maecenas fermentum consequat mi. Donec fermentum. Pellentesque malesuada nulla a mi. Duis sapien sem, aliquet nec, commodo eget, consequat quis, neque. Aliquam faucibus, elit ut dictum aliquet, felis nisl adipiscing sapien, sed malesuada diam lacus eget erat. Cras mollis scelerisque nunc. Nullam arcu. Aliquam consequat. Curabitur augue lorem, dapibus quis, laoreet et, pretium ac, nisi. Aenean magna nisl, mollis quis, molestie eu, feugiat in, orci. In hac habitasse platea dictumst."s);
  REQUIRE(freqs_3['e'] == 114);
  REQUIRE(freqs_3['u'] == 97);

  auto const freqs_4 = hmr::analysis::character_frequency("Some \x99 unprintable \t chars \x8c\x8c\x8c \x10 \x02 11111"s);
  REQUIRE(freqs_4['1'] == 5);
  REQUIRE(freqs_4[0x8c] == 3);

  // English-like text detection
  REQUIRE(hmr::analysis::looks_like_english("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Curabitur pretium tincidunt lacus. Nulla gravida orci a odio. Nullam varius, turpis et commodo pharetra, est eros bibendum elit, nec luctus magna felis sollicitudin mauris. Integer in mauris eu nibh euismod gravida. Duis ac tellus et risus vulputate vehicula. Donec lobortis risus a elit. Etiam tempor. Ut ullamcorper, ligula eu tempor congue, eros est euismod turpis, id tincidunt sapien risus a quam. Maecenas fermentum consequat mi. Donec fermentum. Pellentesque malesuada nulla a mi. Duis sapien sem, aliquet nec, commodo eget, consequat quis, neque. Aliquam faucibus, elit ut dictum aliquet, felis nisl adipiscing sapien, sed malesuada diam lacus eget erat. Cras mollis scelerisque nunc. Nullam arcu. Aliquam consequat. Curabitur augue lorem, dapibus quis, laoreet et, pretium ac, nisi. Aenean magna nisl, mollis quis, molestie eu, feugiat in, orci. In hac habitasse platea dictumst."s));
  REQUIRE(hmr::analysis::looks_like_english("ASD(*&'(*$T^&%!G$^(&*'%$^'G$%F"s) == false);

  // Shannon entropy score
  REQUIRE(hmr::analysis::entropy("1223334444"s) < 2);
  REQUIRE(hmr::analysis::entropy(input) > 3);

  // Single byte XOR key solution
  auto const xord_1 = hmr::hex::decode("01 3d 3c 26 75 3c 26 75 34 75 27 30 34 39 39 2c 75 32 27 30 34 21 75 30 2d 34 38 25 39 30 75 3a 33 75 34 75 38 30 31 3c 20 38 75 26 3c 2f 30 31 75 10 3b 32 39 3c 26 3d 75 26 21 27 3c 3b 32 79 75 22 3c 21 3d 75 34 75 32 3a 3a 31 75 38 3c 2d 75 3a 33 75 00 05 05 10 07 16 14 06 10 75 34 3b 31 75 39 3a 22 30 27 36 34 26 30 75 39 30 21 21 30 27 26 79 75 25 20 3b 36 21 20 34 21 3c 3a 3b 79 75 30 21 36 7b 74"s);
  auto possible_keys = hmr::analysis::solve_single_byte_xor(xord_1);
  REQUIRE(possible_keys.size() == 1);

  // Single byte XOR failure - try solving for 64 random bytes
  auto possible_keys_2 = hmr::analysis::solve_single_byte_xor(hmr::prng::bytes(64));
  REQUIRE(possible_keys_2.empty() == true);

  // Multi-byte XOR key size solution
  auto const random_xor_key = hmr::prng::bytes(20);
  auto const plaintext = "This should be a long enough bit of example text to demonstrate multi-byte XOR key length detection. Fingers crossed this works!"s;
  auto const xord_2 = hmr::bitwise::xor_with_key(plaintext, random_xor_key);
  auto const most_likely_xor_keysize = hmr::analysis::find_candidate_keysize(xord_2).first;
  REQUIRE(most_likely_xor_keysize == 20);

  // Multi-byte XOR key size failure - try solving for 64 random bytes
  auto const random_input = hmr::prng::bytes(64);
  auto const probable_xor_keysize = hmr::analysis::find_candidate_keysize(random_input).first;
  REQUIRE(probable_xor_keysize != 64);

  // Repeated block detection
  REQUIRE(hmr::analysis::repeated_blocks("12345678ABCDEFGH12345678ZYXWVUTS"s, 8) == true);
  REQUIRE(hmr::analysis::repeated_blocks("11111111111111112222222222222222"s) == false);
  REQUIRE(hmr::analysis::repeated_blocks("111111111111111122222222222222221111111111111111"s) == true);
}

// hmr::pkcs7
TEST_CASE("hmr::pkcs7", "[pkcs7]")
{
  auto const input = "Hello, World!"s;

  REQUIRE(hmr::pkcs7::unpad(hmr::pkcs7::pad(input)) == input);
  REQUIRE(hmr::pkcs7::padded(input) == false);
  REQUIRE(hmr::pkcs7::padded(hmr::pkcs7::pad(input)) == true);

  REQUIRE(hmr::pkcs7::padded(hmr::hex::decode("AA AA AA AA AA AA AA AA AA AA AA AA AA AA 02 02"s)) == true);
  REQUIRE(hmr::pkcs7::padded(hmr::hex::decode("AA AA AA AA AA AA AA AA AA AA AA AA AA AA 00 00"s)) == false);
  REQUIRE(hmr::pkcs7::padded(hmr::hex::decode("AA AA AA AA AA 03 03 03"s), 8) == true);
  REQUIRE(hmr::pkcs7::padded(hmr::hex::decode("AA AA AA AA AA 0B 0B 0B 0B 0B 0B 0B 0B 0B 0B 0B"s)) == true);
  REQUIRE(hmr::pkcs7::pad("Test"s) == hmr::hex::decode("54 65 73 74 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C"s));
  REQUIRE(hmr::pkcs7::pad("Test"s, 8) == hmr::hex::decode("54 65 73 74 04 04 04 04"s));
}

// hmr::kvp
TEST_CASE("hmr::kvp", "[serialisation][kvp]")
{
  auto const kvps = std::map<std::string, std::string>{{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}};

  REQUIRE(hmr::kvp::serialise(kvps) == "key1=value1&key2=value2&key3=value3"s);
  REQUIRE(hmr::kvp::deserialise("key1=value1&key2=value2&key3=value3"s) == kvps);

  // Failures
  REQUIRE_THROWS(hmr::kvp::deserialise("key1=value1=value2") == std::map<std::string, std::string>{});
  REQUIRE_THROWS(hmr::kvp::deserialise("key1=value1&something&key2=value2") == std::map<std::string, std::string>{});
  REQUIRE_THROWS(hmr::kvp::deserialise("key1=value1& &key2=value2") == std::map<std::string, std::string>{});
}

// hmr::uuid
TEST_CASE("hmr::uuid", "[uuid]")
{
  auto const uuid = hmr::uuid::generate();

  REQUIRE(uuid.size() == 36);
  REQUIRE(uuid.find_first_not_of("0123456789ABCDEF-") == std::string::npos);
}

// hmr::crypto
TEST_CASE("hmr::crypto", "[crypto][aes]")
{
  auto const input = "Hello, World!"s;

  REQUIRE(hmr::base64::encode(hmr::crypto::aes_ecb_encrypt(input, "YELLOW SUBMARINE"s)) == "401gyzJgpJNkouYaQRZZRg=="s);
  REQUIRE(hmr::crypto::aes_ecb_decrypt(hmr::base64::decode("401gyzJgpJNkouYaQRZZRg=="s), "YELLOW SUBMARINE"s) == input);

  REQUIRE(hmr::crypto::aes_ecb_decrypt(hmr::base64::decode("401gyzJgpJNkouYaQRZZRg=="s), "YELLOW SUBMARINE"s, false) == "Hello, World!\x03\x03\x03"s);
  REQUIRE(hmr::pkcs7::padded(hmr::crypto::aes_ecb_decrypt(hmr::base64::decode("401gyzJgpJNkouYaQRZZRg=="s), "YELLOW SUBMARINE"s, false)));

  auto const longer_plaintext = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal"s;
  REQUIRE(hmr::base64::encode(hmr::crypto::aes_cbc_encrypt(longer_plaintext, "ORANGE SUBMARINE"s, "ORANGE SUBMARINE"s)) == "rnPbRj30TGD+MRXM2O14b/xSA9oAv8Al/um7hObPUi5wP82Idy3FvXxNYghiPMeB+YLHDpzQDPm4FsNeSARVda55uN8ePdMZhoPkaiNbQcA="s);
  REQUIRE(hmr::crypto::aes_cbc_decrypt(hmr::base64::decode("rnPbRj30TGD+MRXM2O14b/xSA9oAv8Al/um7hObPUi5wP82Idy3FvXxNYghiPMeB+YLHDpzQDPm4FsNeSARVda55uN8ePdMZhoPkaiNbQcA="s), "ORANGE SUBMARINE"s, "ORANGE SUBMARINE"s) == longer_plaintext);

  // Hashing
  REQUIRE(hmr::crypto::md5(input) == "65a8e27d8879283831b664bd8b7f0ad4"s);
  REQUIRE(hmr::crypto::md5_raw(input) == "\x65\xa8\xe2\x7d\x88\x79\x28\x38\x31\xb6\x64\xbd\x8b\x7f\x0a\xd4"s);
  REQUIRE(hmr::crypto::sha1(input) == "0a0a9f2a6772942557ab5355d76af442f8f65e01"s);
  REQUIRE(hmr::crypto::sha1_raw(input) == "\x0a\x0a\x9f\x2a\x67\x72\x94\x25\x57\xab\x53\x55\xd7\x6a\xf4\x42\xf8\xf6\x5e\x01"s);
  REQUIRE(hmr::crypto::sha256(input) == "dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f"s);
  REQUIRE(hmr::crypto::sha256_raw(input) == "\xdf\xfd\x60\x21\xbb\x2b\xd5\xb0\xaf\x67\x62\x90\x80\x9e\xc3\xa5\x31\x91\xdd\x81\xc7\xf7\x0a\x4b\x28\x68\x8a\x36\x21\x82\x98\x6f"s);
}
