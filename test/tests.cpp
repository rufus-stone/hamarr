#define CATCH_CONFIG_MAIN // This tells the Catch2 header to generate a main

#include <string>

#include <hamarr/hamarr.hpp>

#include "catch.hpp"

using namespace std::string_literals;

// Uppercase/lowercase string conversion
TEST_CASE("Uppercase/lowercase std::string conversion", "[formatting][case][string]")
{
  REQUIRE(hmr::format::to_upper("Hello, World!"s) == "HELLO, WORLD!"s);
  REQUIRE(hmr::format::to_lower("Hello, World!"s) == "hello, world!"s);
}

// Escaping/un-escaping strings
TEST_CASE("Escape/un-escape std::string", "[formatting][escape][string]")
{
  REQUIRE(hmr::format::escape("This\nhas\nnew\nlines and backslash \\ and unprintable \x03 hex \x00 chars \x7F"s) == "This\\nhas\\nnew\\nlines and backslash \\\\ and unprintable \\x03 hex \\x00 chars \\x7F"s);
  REQUIRE(hmr::format::unescape("This\\nhas\\nnew\\nlines and backslash \\\\ and unprintable \\x03 hex \\x00 chars \\x7F"s) == "This\nhas\nnew\nlines and backslash \\ and unprintable \x03 hex \x00 chars \x7F"s);

  // Failures
  REQUIRE(hmr::format::unescape("\\"s) == std::string{}); // Escape sequence is unfinished
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
  REQUIRE(hmr::hex::decode<uint8_t>("FF FF"s) == uint8_t{}); // Too many bytes for the requested return type
  REQUIRE(hmr::hex::decode<uint16_t>("FF FF FF"s) == uint16_t{}); // Too many bytes for the requested return type
  REQUIRE(hmr::hex::decode<uint32_t>("FF FF FF FF FF FF"s) == uint32_t{}); // Too many bytes for the requested return type
  REQUIRE(hmr::hex::decode<uint64_t>("AA BB CC DD EE FF 00 11 22 33 44 55 66 77 88 99"s) == uint64_t{}); // Too many bytes for the requested return type
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
  REQUIRE(hmr::binary::decode<uint8_t>("11110000 11110000"s) == uint8_t{}); // Too many bits for requested result type
  REQUIRE(hmr::binary::decode<uint16_t>("11110000"s) == uint16_t{}); // Not enough bits for requested result type
  REQUIRE(hmr::binary::decode<uint8_t>("1010"s) == uint8_t{}); // Number of characters not a multiple of 8
  REQUIRE(hmr::binary::decode<int>("11110000 !INVALID!!CHARS! 00001111"s) == int{}); // Invalid input characters
}

// Base64 encoding/decoding strings
TEST_CASE("Base64 encode/decode std::string", "[encoding][base64][string]")
{
  // Standard base64 alphabet
  REQUIRE(hmr::base64::encode("Hello, World!"s) == "SGVsbG8sIFdvcmxkIQ=="s);
  REQUIRE(hmr::base64::decode("SGVsbG8sIFdvcmxkIQ=="s) == "Hello, World!"s);

  REQUIRE(hmr::base64::encode(hmr::hex::decode("1f 8b 08 00 00 00 00 00 00 00 05 80 21 0d 00 00 10 02 4b 7e 05 f6 c3 60 a0 be b8 9d 3a e7 01 9a 63 4e 27 07 00 00 00"s)) == "H4sIAAAAAAAAAAWAIQ0AABACS34F9sNgoL64nTrnAZpjTicHAAAA"s);
  REQUIRE(hmr::base64::decode("H4sIAAAAAAAAAAWAIQ0AABACS34F9sNgoL64nTrnAZpjTicHAAAA"s) == hmr::hex::decode("1f 8b 08 00 00 00 00 00 00 00 05 80 21 0d 00 00 10 02 4b 7e 05 f6 c3 60 a0 be b8 9d 3a e7 01 9a 63 4e 27 07 00 00 00"s));

  REQUIRE(hmr::base64::encode(hmr::hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd"s)) == "MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9"s);
  REQUIRE(hmr::base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9"s) == hmr::hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd"s));

  REQUIRE(hmr::base64::encode(hmr::hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe"s)) == "MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/g=="s);
  REQUIRE(hmr::base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/g=="s) == hmr::hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe"s));

  REQUIRE(hmr::base64::encode(hmr::hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff"s)) == "MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/v8="s);
  REQUIRE(hmr::base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/v8="s) == hmr::hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff"s));

  // Custom base64 alphabet
  REQUIRE(hmr::base64::encode("Hello, World!"s, "abcdefgh0123456789ijklmnopqrstuvwxyz=/ABCDEFGHIJKLMNOPQRSTUVWXYZ+"s) == "iglGrgWG0ftJsAL=08++"s);
  REQUIRE(hmr::base64::decode("iglGrgWG0ftJsAL=08++"s, "abcdefgh0123456789ijklmnopqrstuvwxyz=/ABCDEFGHIJKLMNOPQRSTUVWXYZ+"s) == "Hello, World!"s);

  // Failures
  REQUIRE(hmr::base64::encode("This won't work", "Thisalphabetistoosmall") == std::string{});
  REQUIRE(hmr::base64::decode("This won't work", "Thisalphabetistoosmall") == std::string{});
}

// URL encoding/decoding strings
TEST_CASE("URL encode/decode std::string", "[encoding][url][string]")
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
  REQUIRE(hmr::url::decode("Invalid percent-encoded hex sequences %ZZ %JJ") == std::string{});
  REQUIRE(hmr::url::decode("Missing data %C2%") == std::string{});
}

// XOR
TEST_CASE("XOR with key", "[obfuscation][xor][string]")
{
  const auto input = "Hello, World!"s;

  // String as key
  REQUIRE(hmr::bitwise::xor_with_key(input, "great key") == hmr::hex::decode("2F 17 09 0D 1B 0C 4B 32 16 15 1E 01 40"));
  REQUIRE(hmr::bitwise::xor_with_key("12345678", "ABC\xFF") == hmr::hex::decode("70 70 70 cb 74 74 74 c7"));

  // Char as key
  REQUIRE(hmr::bitwise::xor_with_key(input, 'c') == hmr::hex::decode("2b 06 0f 0f 0c 4f 43 34 0c 11 0f 07 42"));

  // Integral as key
  REQUIRE(hmr::bitwise::xor_with_key(input, 1) == hmr::hex::decode("49 64 6d 6d 6e 2d 21 56 6e 73 6d 65 20"));
  REQUIRE(hmr::bitwise::xor_with_key(input, uint8_t{0x33}) == hmr::hex::decode("7b 56 5f 5f 5c 1f 13 64 5c 41 5f 57 12"));
}
