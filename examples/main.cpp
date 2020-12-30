#include <string>
#include <chrono>

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
#include <hamarr/profiling.hpp>

#include <spdlog/spdlog.h>

using namespace hmr;

using namespace std::string_literals;

int main()
{
  auto test = std::string("Hello, World!");

  spdlog::info("\n\n---[ Formatting ]---\n");
  spdlog::info(format::to_upper(test));
  spdlog::info(format::to_lower(test));
  spdlog::info(format::escape("This\nhas\nnew\nlines and backslash \\ and unprintable \x03 hex \x00 chars \x7F"s));
  spdlog::info(format::unescape("This\\nhas\\nnew\\nlines and backslash \\\\ and hex \x31 \x32 \x33\\g"s));


  spdlog::info("\n\n---[ Hex Strings ]---\n");
  spdlog::info(hex::encode(test));
  spdlog::info(hex::decode(hex::encode(test)));

  spdlog::info("\n\n---[ Hex Strings (Non-Padded) ]---\n");
  spdlog::info(hex::encode(test, false));
  spdlog::info(hex::decode(hex::encode(test, false)));


  spdlog::info("\n\n---[ Hex Numbers ]---\n");
  spdlog::info(hex::encode(uint8_t{18}));
  spdlog::info(hex::encode(uint16_t{4660}));
  spdlog::info(hex::encode(uint32_t{305419896}));
  spdlog::info(hex::encode(uint64_t{1311768467463790320}));
  spdlog::info(hex::decode<uint16_t>("FF"));
  spdlog::info(hex::decode<uint32_t>("FF FF"));
  spdlog::info(hex::decode<uint64_t>("0F"));

  spdlog::info("\n\n---[ Hex Numbers (Non-Padded) ]---\n");
  spdlog::info(hex::encode(uint16_t{4660}, false));
  spdlog::info(hex::encode(uint32_t{305419896}, false));
  spdlog::info(hex::encode(uint64_t{1311768467463790320}, false));


  spdlog::info("\n\n---[ Binary Strings ]---\n");
  spdlog::info(binary::encode(test));
  spdlog::info(binary::decode(binary::encode(test)));

  spdlog::info("\n\n---[ Binary Strings (Non-Padded) ]---\n");
  spdlog::info(binary::encode(test, false));
  spdlog::info(binary::decode(binary::encode(test, false)));


  spdlog::info("\n\n---[ Binary Numbers ]---\n");
  spdlog::info(binary::encode(uint8_t{18}));
  spdlog::info(binary::encode(uint16_t{4660}));
  spdlog::info(binary::encode(uint32_t{305419896}));
  spdlog::info(binary::encode(uint64_t{1311768467463790320}));
  spdlog::info(static_cast<int>(binary::decode<uint8_t>("11110000")));
  spdlog::info(binary::decode<uint16_t>("00001111 11110000"));
  spdlog::info(binary::decode<uint32_t>("11110000 00001111 11110000 00001111"));
  spdlog::info(binary::decode<uint64_t>("00001111 11110000 00001111 11110000 00001111 11110000 00001111 11110000"));
  spdlog::info(binary::decode<int>("11111111 11111111 00000000 00000000"));
  spdlog::info(binary::decode<int16_t>("11111111 00000000"));

  spdlog::info("\n\n---[ Binary Numbers (Non-Padded) ]---\n");
  spdlog::info(binary::encode(uint16_t{4660}, false));
  spdlog::info(binary::encode(uint32_t{305419896}, false));
  spdlog::info(binary::encode(uint64_t{1311768467463790320}, false));
  spdlog::info(binary::encode(-12345, false));


  spdlog::info("\n\n---[ Base64 ]---\n");
  spdlog::info(base64::encode(test));
  spdlog::info(base64::decode(base64::encode(test)));

  spdlog::info(base64::encode(hex::decode("1f 8b 08 00 00 00 00 00 00 00 05 80 21 0d 00 00 10 02 4b 7e 05 f6 c3 60 a0 be b8 9d 3a e7 01 9a 63 4e 27 07 00 00 00")));
  spdlog::info(hex::encode(base64::decode("H4sIAAAAAAAAAAWAIQ0AABACS34F9sNgoL64nTrnAZpjTicHAAAA"), true));

  spdlog::info(base64::encode(hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd")));
  spdlog::info(hex::encode(base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9"), true));

  spdlog::info(base64::encode(hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe")));
  spdlog::info(hex::encode(base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/g=="), true));

  spdlog::info(base64::encode(hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff")));
  spdlog::info(hex::encode(base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/v8="), true));

  spdlog::info("\n\n---[ Custom Base64 ]---\n");
  spdlog::info(base64::encode(test, "abcdefgh0123456789ijklmnopqrstuvwxyz=/ABCDEFGHIJKLMNOPQRSTUVWXYZ+"));
  spdlog::info(base64::decode("iglGrgWG0ftJsAL=08++", "abcdefgh0123456789ijklmnopqrstuvwxyz=/ABCDEFGHIJKLMNOPQRSTUVWXYZ+"));


  spdlog::info("\n\n---[ URL encoding ]---\n");
  spdlog::info(url::encode(test));
  spdlog::info(url::decode(url::encode(test)));
  spdlog::info(url::encode(hex::decode("10 33 55 77 99 AA BB DD FF")));

  spdlog::info("\n\n---[ URL encoding (lazy) ]---\n");
  spdlog::info(url::encode(hex::decode("10 33 55 77 99 AA BB DD FF"), true));


  spdlog::info("\n\n---[ XOR ]---\n");
  spdlog::info(hex::encode(bitwise::xor_with_key(test, "great key")));
  spdlog::info(hex::encode(bitwise::xor_with_key("12345678", "ABC\xFF")));
  spdlog::info(hex::encode(bitwise::xor_with_key(test, 1))); // 49 64 6d 6d 6e 2d 21 56 6e 73 6d 65 20
  spdlog::info(hex::encode(bitwise::xor_with_key(test, 'c'))); // 2b 06 0f 0f 0c 4f 43 34 0c 11 0f 07 42
  spdlog::info(hex::encode(bitwise::xor_with_key(test, uint8_t{0x33}))); // 7b 56 5f 5f 5c 1f 13 64 5c 41 5f 57 12

  spdlog::info("\n\n---[ XOR (rolling) ]---\n");
  spdlog::info(hex::encode(bitwise::xor_rolling("12345678")));
  spdlog::info(hex::encode(bitwise::xor_rolling("12345678", bitwise::xor_differential::output)));

  spdlog::info("\n\n---[ XOR (counter) ]---\n");
  spdlog::info(hex::encode(bitwise::xor_counter(test)));
  spdlog::info(hex::encode(bitwise::xor_counter(test, '\xAA')));
  spdlog::info(hex::encode(bitwise::xor_counter(test, '\xEF', 6)));
  spdlog::info(hex::encode(bitwise::xor_counter(test, '\x04', -2)));


  spdlog::info("\n\n---[ Bit shift ]---\n");
  spdlog::info(binary::encode(bitwise::shift_left(binary::decode("00000001"))));
  spdlog::info(binary::encode(bitwise::shift_left(binary::decode("00000001"), 7)));
  spdlog::info(binary::encode(bitwise::shift_right(binary::decode("00000001"))));
  spdlog::info(binary::encode(bitwise::shift_right(binary::decode("01001001"), 6)));
  spdlog::info(binary::encode(bitwise::shift_right(test, 6)));
  spdlog::info(binary::encode(bitwise::shift_right("Another", 2)));

  spdlog::info(binary::encode(bitwise::shift_left<uint16_t>(255, 8)));
  spdlog::info(binary::encode(bitwise::shift_left<uint64_t>(7890123499999, 8)));
  spdlog::info(binary::encode(bitwise::shift_left<int8_t>(-27, 3)));
  spdlog::info(binary::encode(bitwise::shift_right<uint8_t>(16, 2)));
  spdlog::info(binary::encode(bitwise::shift_right<int16_t>(-1000, 3)));
  spdlog::info(binary::encode(bitwise::shift_right(int16_t(-1000), 3)));


  spdlog::info("\n\n---[ Bit rotate ]---\n");
  spdlog::info(binary::encode(bitwise::rotate_left(test, 5)));
  spdlog::info(binary::encode(bitwise::rotate_left(binary::decode("01100111 11100010 11110000 00001111"), 4)));
  spdlog::info(binary::encode(bitwise::rotate_left(test, 5, bitwise::carry_through::enabled)));
  spdlog::info(binary::encode(bitwise::rotate_left(binary::decode("10000000 00000000 11111111 00000001"), 20, bitwise::carry_through::enabled)));
  spdlog::info(binary::encode(bitwise::rotate_right(test, 12)));
  spdlog::info(binary::encode(bitwise::rotate_right(binary::decode("01100111 11100010 11110000 00001111"), 4)));
  spdlog::info(binary::encode(bitwise::rotate_right(test, 2, bitwise::carry_through::enabled)));
  spdlog::info(binary::encode(bitwise::rotate_right(binary::decode("10001000 01110111 00110010"), 7, bitwise::carry_through::enabled)));


  spdlog::info("\n\n---[ Pseudo-Random Number Generator ]---\n");
  spdlog::info(prng::number<unsigned int>());
  spdlog::info(static_cast<int>(prng::number<uint8_t>()));
  spdlog::info(prng::number<double>());
  spdlog::info(prng::number<unsigned long long int>());
  spdlog::info(prng::number_between<unsigned int>(0, 10));
  spdlog::info(prng::number_between<uint16_t>(0, 1));
  spdlog::info(prng::number_between<double>(-1, 1));
  spdlog::info(hex::encode(prng::bytes(16)));


  spdlog::info("\n\n---[ Hamming Distance ]---\n");
  spdlog::info("Hamming distance between 'this is a test' and 'wokka wokka!!!' is: {}", analysis::hamming_distance("this is a test", "wokka wokka!!!"));
  spdlog::info(analysis::hamming_distance("this is a testEXTRASTUFF", "wokka wokka!!!"));


  spdlog::info("\n\n---[ Character Frequency Analysis ]---\n");
  auto freqs_1 = analysis::character_frequency(test);
  analysis::print_character_frequency(freqs_1);

  auto freqs_2 = analysis::character_frequency("Mix OF upPer AND LOWER", analysis::case_sensitivity::disabled);
  analysis::print_character_frequency(freqs_2);

  auto freqs_3 = analysis::character_frequency("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Curabitur pretium tincidunt lacus. Nulla gravida orci a odio. Nullam varius, turpis et commodo pharetra, est eros bibendum elit, nec luctus magna felis sollicitudin mauris. Integer in mauris eu nibh euismod gravida. Duis ac tellus et risus vulputate vehicula. Donec lobortis risus a elit. Etiam tempor. Ut ullamcorper, ligula eu tempor congue, eros est euismod turpis, id tincidunt sapien risus a quam. Maecenas fermentum consequat mi. Donec fermentum. Pellentesque malesuada nulla a mi. Duis sapien sem, aliquet nec, commodo eget, consequat quis, neque. Aliquam faucibus, elit ut dictum aliquet, felis nisl adipiscing sapien, sed malesuada diam lacus eget erat. Cras mollis scelerisque nunc. Nullam arcu. Aliquam consequat. Curabitur augue lorem, dapibus quis, laoreet et, pretium ac, nisi. Aenean magna nisl, mollis quis, molestie eu, feugiat in, orci. In hac habitasse platea dictumst.");
  analysis::print_character_frequency(freqs_3);

  auto freqs_4 = analysis::character_frequency(prng::bytes(16));
  analysis::print_character_frequency(freqs_4);


  spdlog::info("\n\n---[ English Text Detection ]---\n");
  spdlog::info(analysis::looks_like_english("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Curabitur pretium tincidunt lacus. Nulla gravida orci a odio. Nullam varius, turpis et commodo pharetra, est eros bibendum elit, nec luctus magna felis sollicitudin mauris. Integer in mauris eu nibh euismod gravida. Duis ac tellus et risus vulputate vehicula. Donec lobortis risus a elit. Etiam tempor. Ut ullamcorper, ligula eu tempor congue, eros est euismod turpis, id tincidunt sapien risus a quam. Maecenas fermentum consequat mi. Donec fermentum. Pellentesque malesuada nulla a mi. Duis sapien sem, aliquet nec, commodo eget, consequat quis, neque. Aliquam faucibus, elit ut dictum aliquet, felis nisl adipiscing sapien, sed malesuada diam lacus eget erat. Cras mollis scelerisque nunc. Nullam arcu. Aliquam consequat. Curabitur augue lorem, dapibus quis, laoreet et, pretium ac, nisi. Aenean magna nisl, mollis quis, molestie eu, feugiat in, orci. In hac habitasse platea dictumst."));
  spdlog::info(analysis::looks_like_english("ASD(*&'(*$T^&%!G$^(&*'%$^'G$%F"));


  spdlog::info("\n\n---[ Code Profiling ]---\n");
  auto nanoseconds_taken = profile::benchmark([&test]()
    {
      for (int i = 0; i < 1000; ++i)
      {
        hex::encode(test);
      }
    });

  auto milliseconds_taken = std::chrono::duration<double, std::milli>(nanoseconds_taken).count();
  auto seconds_taken = std::chrono::duration<double>(nanoseconds_taken).count();
  spdlog::info("Execution took {} ns ({} ms / {} s)", nanoseconds_taken.count(), milliseconds_taken, seconds_taken);


  nanoseconds_taken = profile::benchmark([]()
    {
      for (int i = 0; i < 1000; ++i)
      {
        hex::decode("48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21");
      }
    });

  milliseconds_taken = std::chrono::duration<double, std::milli>(nanoseconds_taken).count();
  seconds_taken = std::chrono::duration<double>(nanoseconds_taken).count();
  spdlog::info("Execution took {} ns ({} ms / {} s)", nanoseconds_taken.count(), milliseconds_taken, seconds_taken);


  nanoseconds_taken = profile::benchmark([]()
    {
      for (int i = 0; i < 1000; ++i)
      {
        hex::encode(prng::number<uint32_t>());
      }
    });

  milliseconds_taken = std::chrono::duration<double, std::milli>(nanoseconds_taken).count();
  seconds_taken = std::chrono::duration<double>(nanoseconds_taken).count();
  spdlog::info("Execution took {} ns ({} ms / {} s)", nanoseconds_taken.count(), milliseconds_taken, seconds_taken);


  spdlog::info("\n\n---[ PKCS7 Padding ]---\n");
  spdlog::info(hex::encode(pkcs7::pad(test)));
  spdlog::info(hex::encode(pkcs7::unpad(pkcs7::pad(test))));
  spdlog::info(hex::encode(pkcs7::pad("Test")));
  spdlog::info(hex::encode(pkcs7::pad("Test", 8)));


  spdlog::info("\n\n---[ Shannon Entropy ]---\n");
  spdlog::info("1223334444 has entropy: {}", analysis::entropy("1223334444"));
  spdlog::info("{} has entropy: {}", test, analysis::entropy(test));


  spdlog::info("\n\n---[ Solve Single Byte XOR ]---\n");
  auto xord = hex::decode("01 3d 3c 26 75 3c 26 75 34 75 27 30 34 39 39 2c 75 32 27 30 34 21 75 30 2d 34 38 25 39 30 75 3a 33 75 34 75 38 30 31 3c 20 38 75 26 3c 2f 30 31 75 10 3b 32 39 3c 26 3d 75 26 21 27 3c 3b 32 79 75 22 3c 21 3d 75 34 75 32 3a 3a 31 75 38 3c 2d 75 3a 33 75 00 05 05 10 07 16 14 06 10 75 34 3b 31 75 39 3a 22 30 27 36 34 26 30 75 39 30 21 21 30 27 26 79 75 25 20 3b 36 21 20 34 21 3c 3a 3b 79 75 30 21 36 7b 74");
  auto possible_keys = analysis::solve_single_byte_xor(xord);
  spdlog::info("XOR key == {}", hex::encode(possible_keys[0]));
  spdlog::info("Output  == {}", bitwise::xor_with_key(xord, possible_keys[0]));


  spdlog::info("\n\n---[ Repeated Block Detection ]---\n");
  spdlog::info(analysis::repeated_blocks("12345678ABCDEFGH12345678ZYXWVUTS", 8));
  spdlog::info(analysis::repeated_blocks("11111111111111112222222222222222"));


  spdlog::info("\n\n---[ Data Serialisation ]---\n");
  auto kvps = std::map<std::string, std::string>{{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}};
  spdlog::info(hmr::kvp::serialise(kvps));


  spdlog::info("\n\n---[ UUID Generation ]---\n");
  spdlog::info(hmr::uuid::generate());


  spdlog::info("\n\n---[ Crypto - AES ]---\n");
  spdlog::info(hmr::base64::encode(hmr::crypto::aes_ecb_encrypt(test, "YELLOW SUBMARINE")));
  spdlog::info(hmr::format::escape(hmr::crypto::aes_ecb_decrypt(hmr::base64::decode("401gyzJgpJNkouYaQRZZRg=="), "YELLOW SUBMARINE", false)));

  auto longer_plaintext = std::string{"Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal"};
  spdlog::info(hmr::base64::encode(hmr::crypto::aes_cbc_encrypt(longer_plaintext, "ORANGE SUBMARINE", "ORANGE SUBMARINE")));


  spdlog::info("\n\n---[ Crypto - Hashes ]---\n");
  spdlog::info(hmr::crypto::md5(test));
  spdlog::info(hmr::crypto::sha1(test));
  spdlog::info(hmr::crypto::sha256(test));

  return 0;
}
