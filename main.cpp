#include <assert.h>

#include "utils.hpp"

using namespace hmr;

int main()
{
  auto test = std::string("Hello, World!");
  LOG_INFO(test);
  LOG_ERROR("This is an error!");

  LOG_INFO("\n\n---[ Formatting ]---\n");
  LOG_INFO(format::to_upper(test));
  LOG_INFO(format::to_lower(test));


  LOG_INFO("\n\n---[ Hex Strings ]---\n");
  LOG_INFO(hex::encode(test));
  LOG_INFO(hex::decode(hex::encode(test)));
  assert(hex::decode(hex::encode(test)) == test);
  assert(hex::decode("Invalid hex input") == std::string{}); // Chars that fall outside the set of valid hex chars
  assert(hex::decode("11 22 3") == std::string{}); // Uneven number of hex chars

  LOG_INFO("\n\n---[ Hex Strings (Non-Padded) ]---\n");
  LOG_INFO(hex::encode(test, false));
  LOG_INFO(hex::decode(hex::encode(test, false)));
  assert(hex::decode(hex::encode(test, false)) == test);
  assert(hex::decode("11223") == std::string{}); // Uneven number of hex chars


  LOG_INFO("\n\n---[ Hex Numbers ]---\n");
  LOG_INFO(hex::encode(uint8_t{18}));
  assert(hex::encode(uint8_t{18}) == "12");
  assert(hex::decode<uint8_t>("12") == uint8_t{18});

  LOG_INFO(hex::encode(uint16_t{4660}));
  assert(hex::encode(uint16_t{4660}) == "12 34");
  assert(hex::decode<uint16_t>("12 34") == uint16_t{4660});

  LOG_INFO(hex::encode(uint32_t{305419896}));
  assert(hex::encode(uint32_t{305419896}) == "12 34 56 78");
  assert(hex::decode<uint32_t>("12 34 56 78") == uint32_t{305419896});

  LOG_INFO(hex::encode(uint64_t{1311768467463790320}));
  assert(hex::encode(uint64_t{1311768467463790320}) == "12 34 56 78 9A BC DE F0");
  assert(hex::decode<uint64_t>("12 34 56 78 9A BC DE F0") == uint64_t{1311768467463790320});

  LOG_INFO(hex::decode<uint16_t>("FF"));
  assert(hex::decode<uint16_t>("FF") == uint16_t{255});

  LOG_INFO(hex::decode<uint32_t>("FF FF"));
  assert(hex::decode<uint32_t>("FF FF") == uint32_t{65535});

  LOG_INFO(hex::decode<uint64_t>("0F"));
  assert(hex::decode<uint64_t>("0F") == uint64_t{15});

  assert(hex::decode<uint8_t>("FF FF") == uint8_t{}); // Too many bytes for the requested return type
  assert(hex::decode<uint16_t>("FF FF FF") == uint16_t{}); // Too many bytes for the requested return type
  assert(hex::decode<uint32_t>("FF FF FF FF FF FF") == uint32_t{}); // Too many bytes for the requested return type
  assert(hex::decode<uint64_t>("AA BB CC DD EE FF 00 11 22 33 44 55 66 77 88 99") == uint64_t{}); // Too many bytes for the requested return type

  LOG_INFO("\n\n---[ Hex Numbers (Non-Padded) ]---\n");
  LOG_INFO(hex::encode(uint16_t{4660}, false));
  assert(hex::encode(uint16_t{4660}, false) == "1234");
  assert(hex::decode<uint16_t>("1234") == uint16_t{4660});

  LOG_INFO(hex::encode(uint32_t{305419896}, false));
  assert(hex::encode(uint32_t{305419896}, false) == "12345678");
  assert(hex::decode<uint32_t>("12345678") == uint32_t{305419896});

  LOG_INFO(hex::encode(uint64_t{1311768467463790320}, false));
  assert(hex::encode(uint64_t{1311768467463790320}, false) == "123456789ABCDEF0");
  assert(hex::decode<uint64_t>("123456789ABCDEF0") == uint64_t{1311768467463790320});


  LOG_INFO("\n\n---[ Binary Strings ]---\n");
  LOG_INFO(binary::encode(test));
  LOG_INFO(binary::decode(binary::encode(test)));
  assert(binary::decode(binary::encode(test)) == test);

  LOG_INFO("\n\n---[ Binary Strings (Non-Padded) ]---\n");
  LOG_INFO(binary::encode(test, false));
  LOG_INFO(binary::decode(binary::encode(test, false)));
  assert(binary::decode(binary::encode(test, false)) == test);


  LOG_INFO("\n\n---[ Binary Numbers ]---\n");
  LOG_INFO(binary::encode(uint8_t{18}));
  assert(binary::encode(uint8_t{18}) == "00010010");

  LOG_INFO(binary::encode(uint16_t{4660}));
  assert(binary::encode(uint16_t{4660}) == "00010010 00110100");

  LOG_INFO(binary::encode(uint32_t{305419896}));
  assert(binary::encode(uint32_t{305419896}) == "00010010 00110100 01010110 01111000");

  LOG_INFO(binary::encode(uint64_t{1311768467463790320}));
  assert(binary::encode(uint64_t{1311768467463790320}) == "00010010 00110100 01010110 01111000 10011010 10111100 11011110 11110000");

  LOG_INFO(int(binary::decode<uint8_t>("11110000")));
  assert(binary::decode<uint8_t>("11110000") == uint8_t{240});

  LOG_INFO(binary::decode<uint16_t>("00001111 11110000"));
  assert(binary::decode<uint16_t>("00001111 11110000") == uint16_t{4080});

  LOG_INFO(binary::decode<uint32_t>("11110000 00001111 11110000 00001111"));
  assert(binary::decode<uint32_t>("11110000 00001111 11110000 00001111") == uint32_t{4027576335});

  LOG_INFO(binary::decode<uint64_t>("00001111 11110000 00001111 11110000 00001111 11110000 00001111 11110000"));
  assert(binary::decode<uint64_t>("00001111 11110000 00001111 11110000 00001111 11110000 00001111 11110000") == uint64_t{1148435428713435120});

  LOG_INFO(binary::decode<int>("11111111 11111111 00000000 00000000"));
  assert(binary::decode<int>("11111111 11111111 00000000 00000000") == -65536);

  LOG_INFO(binary::decode<int16_t>("11111111 00000000"));
  assert(binary::decode<int16_t>("11111111 00000000") == int16_t{-256});

  LOG_INFO("\n\n---[ Binary Numbers (Non-Padded) ]---\n");
  LOG_INFO(binary::encode(uint16_t{4660}, false));
  assert(binary::encode(uint16_t{4660}, false) == "0001001000110100");

  LOG_INFO(binary::encode(uint32_t{305419896}, false));
  assert(binary::encode(uint32_t{305419896}, false) == "00010010001101000101011001111000");

  LOG_INFO(binary::encode(uint64_t{1311768467463790320}, false));
  assert(binary::encode(uint64_t{1311768467463790320}, false) == "0001001000110100010101100111100010011010101111001101111011110000");

  LOG_INFO(binary::encode(-12345, false));
  assert(binary::encode(-12345, false) == "11111111111111111100111111000111");


  LOG_INFO("\n\n---[ Base64 ]---\n");
  LOG_INFO(base64::encode(test));
  LOG_INFO(base64::decode(base64::encode(test)));
  assert(base64::decode(base64::encode(test)) == test);

  LOG_INFO(base64::encode(hex::decode("1f 8b 08 00 00 00 00 00 00 00 05 80 21 0d 00 00 10 02 4b 7e 05 f6 c3 60 a0 be b8 9d 3a e7 01 9a 63 4e 27 07 00 00 00")));
  assert(base64::encode(hex::decode("1f 8b 08 00 00 00 00 00 00 00 05 80 21 0d 00 00 10 02 4b 7e 05 f6 c3 60 a0 be b8 9d 3a e7 01 9a 63 4e 27 07 00 00 00")) == "H4sIAAAAAAAAAAWAIQ0AABACS34F9sNgoL64nTrnAZpjTicHAAAA");
  LOG_INFO(hex::encode(base64::decode("H4sIAAAAAAAAAAWAIQ0AABACS34F9sNgoL64nTrnAZpjTicHAAAA"), true));
  assert(base64::decode("H4sIAAAAAAAAAAWAIQ0AABACS34F9sNgoL64nTrnAZpjTicHAAAA") == hex::decode("1f 8b 08 00 00 00 00 00 00 00 05 80 21 0d 00 00 10 02 4b 7e 05 f6 c3 60 a0 be b8 9d 3a e7 01 9a 63 4e 27 07 00 00 00"));

  LOG_INFO(base64::encode(hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd")));
  assert(base64::encode(hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd")) == "MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9");
  LOG_INFO(hex::encode(base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9"), true));
  assert(base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9") == hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd"));
  
  LOG_INFO(base64::encode(hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe")));
  assert(base64::encode(hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe")) == "MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/g==");
  LOG_INFO(hex::encode(base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/g=="), true));
  assert(base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/g==") == hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe"));
  
  LOG_INFO(base64::encode(hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff")));
  assert(base64::encode(hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff")) == "MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/v8=");
  LOG_INFO(hex::encode(base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/v8="), true));
  assert(base64::decode("MDEyMzQ1Njc4OTo7PD0+P/Dx8vP09fb3+Pn6+/z9/v8=") == hex::decode("30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff"));

  LOG_INFO("\n\n---[ Custom Base64 ]---\n");
  LOG_INFO(base64::encode(test, "abcdefgh0123456789ijklmnopqrstuvwxyz=/ABCDEFGHIJKLMNOPQRSTUVWXYZ+"));
  assert(base64::encode(test, "abcdefgh0123456789ijklmnopqrstuvwxyz=/ABCDEFGHIJKLMNOPQRSTUVWXYZ+") == "iglGrgWG0ftJsAL=08++");
  LOG_INFO(base64::decode("iglGrgWG0ftJsAL=08++", "abcdefgh0123456789ijklmnopqrstuvwxyz=/ABCDEFGHIJKLMNOPQRSTUVWXYZ+"));
  assert(base64::decode("iglGrgWG0ftJsAL=08++", "abcdefgh0123456789ijklmnopqrstuvwxyz=/ABCDEFGHIJKLMNOPQRSTUVWXYZ+") == test);

  assert(base64::encode("This won't work", "Thisalphabetistoosmall") == std::string{});
  assert(base64::decode("This won't work", "Thisalphabetistoosmall") == std::string{});


  LOG_INFO("\n\n---[ URL encoding ]---\n");
  LOG_INFO(url::encode(test));
  LOG_INFO(url::decode(url::encode(test)));
  assert(url::decode(url::encode(test)) == test);

  LOG_INFO(url::encode(hex::decode("10 33 55 77 99 AA BB DD FF")));
  assert(url::encode(hex::decode("10 33 55 77 99 AA BB DD FF")) == "%103Uw%C2%99%C2%AA%C2%BB%C3%FD%C3%FF");

  LOG_INFO("\n\n---[ URL encoding (lazy) ]---\n");
  LOG_INFO(url::encode(hex::decode("10 33 55 77 99 AA BB DD FF"), true));
  assert(url::encode(hex::decode("10 33 55 77 99 AA BB DD FF"), true) == "%103Uw%99%AA%BB%DD%FF");


  LOG_INFO("\n\n---[ XOR ]---\n");
  LOG_INFO(hex::encode(bitwise::xor_with_key(test, "great key")));
  assert(bitwise::xor_with_key(test, "great key") == hex::decode("2F 17 09 0D 1B 0C 4B 32 16 15 1E 01 40"));

  LOG_INFO(hex::encode(bitwise::xor_with_key("12345678", "ABC\xFF")));
  assert(bitwise::xor_with_key("12345678", "ABC\xFF") == hex::decode("70 70 70 cb 74 74 74 c7"));

  LOG_INFO(hex::encode(bitwise::xor_with_key(test, 1))); // 49 64 6d 6d 6e 2d 21 56 6e 73 6d 65 20
  assert(bitwise::xor_with_key(test, 1) == hex::decode("49 64 6d 6d 6e 2d 21 56 6e 73 6d 65 20"));

  LOG_INFO(hex::encode(bitwise::xor_with_key(test, 'c'))); // 2b 06 0f 0f 0c 4f 43 34 0c 11 0f 07 42
  assert(bitwise::xor_with_key(test, 'c') == hex::decode("2b 06 0f 0f 0c 4f 43 34 0c 11 0f 07 42"));

  LOG_INFO(hex::encode(bitwise::xor_with_key(test, uint8_t{0x33}))); // 7b 56 5f 5f 5c 1f 13 64 5c 41 5f 57 12
  assert(bitwise::xor_with_key(test, uint8_t{0x33}) == hex::decode("7b 56 5f 5f 5c 1f 13 64 5c 41 5f 57 12"));

  LOG_INFO("\n\n---[ XOR (rolling) ]---\n");
  LOG_INFO(hex::encode(bitwise::xor_rolling("12345678")));
  assert(bitwise::xor_rolling("12345678") == hex::decode("31 03 01 07 01 03 01 0F"));

  LOG_INFO(hex::encode(bitwise::xor_rolling("12345678", bitwise::xor_differential::output)));
  assert(bitwise::xor_rolling("12345678", bitwise::xor_differential::output) == hex::decode("31 03 30 04 31 07 30 08"));

  LOG_INFO("\n\n---[ XOR (counter) ]---\n");
  LOG_INFO(hex::encode(bitwise::xor_counter(test)));
  assert(bitwise::xor_counter(test) == hex::decode("48 64 6E 6F 6B 29 26 50 67 7B 66 6F 2D"));

  LOG_INFO(hex::encode(bitwise::xor_counter(test, '\xAA')));
  assert(bitwise::xor_counter(test, '\xAA') == hex::decode("E2 CE C0 C1 C1 83 90 E6 DD C1 D8 D1 97"));

  LOG_INFO(hex::encode(bitwise::xor_counter(test, '\xEF', 6)));
  assert(bitwise::xor_counter(test, '\xEF', 6) == hex::decode("A7 90 97 6D 68 21 33 4E 70 57 47 55 16"));

  LOG_INFO(hex::encode(bitwise::xor_counter(test, '\x04', -2)));
  assert(bitwise::xor_counter(test, '\x04', -2) == hex::decode("4C 67 6C 92 93 D6 D8 A1 9B 80 9C 8A CD"));


  LOG_INFO("\n\n---[ Bit shift ]---\n");
  LOG_INFO(binary::encode(bitwise::shift_left(binary::decode("00000001"))));
  assert(binary::encode(bitwise::shift_left(binary::decode("00000001"))) == "00000010");

  LOG_INFO(binary::encode(bitwise::shift_left(binary::decode("00000001"), 7)));
  assert(binary::encode(bitwise::shift_left(binary::decode("00000001"), 7)) == "10000000");

  LOG_INFO(binary::encode(bitwise::shift_right(binary::decode("00000001"))));
  assert(binary::encode(bitwise::shift_right(binary::decode("00000001"))) == "00000000");

  LOG_INFO(binary::encode(bitwise::shift_right(binary::decode("01001001"), 6)));
  assert(binary::encode(bitwise::shift_right(binary::decode("01001001"), 6)) == "00000001");

  LOG_INFO(binary::encode(bitwise::shift_right(test, 6)));
  assert(binary::encode(bitwise::shift_right(test, 6)) == "00000001 00000001 00000001 00000001 00000001 00000000 00000000 00000001 00000001 00000001 00000001 00000001 00000000");

  LOG_INFO(binary::encode(bitwise::shift_right("Another", 2)));
  assert(binary::encode(bitwise::shift_right("Another", 2)) == "00010000 00011011 00011011 00011101 00011010 00011001 00011100");

  LOG_INFO(binary::encode(bitwise::shift_left<uint16_t>(255, 8)));
  assert(binary::encode(bitwise::shift_left<uint16_t>(255, 8)) == "11111111 00000000");

  LOG_INFO(binary::encode(bitwise::shift_left<uint64_t>(7890123499999, 8)));
  assert(binary::encode(bitwise::shift_left<uint64_t>(7890123499999, 8)) == "00000000 00000111 00101101 00010000 00000010 00101001 11011111 00000000");

  LOG_INFO(binary::encode(bitwise::shift_left<int8_t>(-27, 3)));
  assert(binary::encode(bitwise::shift_left<int8_t>(-27, 3)) == "00101000");

  LOG_INFO(binary::encode(bitwise::shift_right<uint8_t>(16, 2)));
  assert(binary::encode(bitwise::shift_right<uint8_t>(16, 2)) == "00000100");

  LOG_INFO(binary::encode(bitwise::shift_right<int16_t>(-1000, 3)));
  assert(binary::encode(bitwise::shift_right<int16_t>(-1000, 3)) == "11111111 10000011");

  LOG_INFO(binary::encode(bitwise::shift_right(int16_t(-1000), 3)));
  assert(binary::encode(bitwise::shift_right(int16_t(-1000), 3)) == "11111111 10000011");


  LOG_INFO("\n\n---[ Bit rotate ]---\n");
  LOG_INFO(binary::encode(bitwise::rotate_left(test, 5)));
  assert(binary::encode(bitwise::rotate_left(test, 5)) == "00001001 10101100 10001101 10001101 11101101 10000101 00000100 11101010 11101101 01001110 10001101 10001100 00100100");

  LOG_INFO(binary::encode(bitwise::rotate_left(binary::decode("01100111 11100010 11110000 00001111"), 4)));
  assert(binary::encode(bitwise::rotate_left(binary::decode("01100111 11100010 11110000 00001111"), 4)) == "01110110 00101110 00001111 11110000");

  LOG_INFO(binary::encode(bitwise::rotate_left(test, 5, bitwise::carry_through::enabled)));
  assert(binary::encode(bitwise::rotate_left(test, 5, bitwise::carry_through::enabled)) == "00001100 10101101 10001101 10001101 11100101 10000100 00001010 11101101 11101110 01001101 10001100 10000100 00101001");

  LOG_INFO(binary::encode(bitwise::rotate_left(binary::decode("10000000 00000000 11111111 00000001"), 20, bitwise::carry_through::enabled)));
  assert(binary::encode(bitwise::rotate_left(binary::decode("10000000 00000000 11111111 00000001"), 20, bitwise::carry_through::enabled)) == "11110000 00011000 00000000 00001111");

  LOG_INFO(binary::encode(bitwise::rotate_right(test, 12)));
  assert(binary::encode(bitwise::rotate_right(test, 12)) == "10000100 01010110 11000110 11000110 11110110 11000010 00000010 01110101 11110110 00100111 11000110 01000110 00010010");

  LOG_INFO(binary::encode(bitwise::rotate_right(binary::decode("01100111 11100010 11110000 00001111"), 4)));
  assert(binary::encode(bitwise::rotate_right(binary::decode("01100111 11100010 11110000 00001111"), 4)) == "01110110 00101110 00001111 11110000");

  LOG_INFO(binary::encode(bitwise::rotate_right(test, 2, bitwise::carry_through::enabled)));
  assert(binary::encode(bitwise::rotate_right(test, 2, bitwise::carry_through::enabled)) == "01010010 00011001 01011011 00011011 00011011 11001011 00001000 00010101 11011011 11011100 10011011 00011001 00001000");

  LOG_INFO(binary::encode(bitwise::rotate_right(binary::decode("10001000 01110111 00110010"), 7, bitwise::carry_through::enabled)));
  assert(binary::encode(bitwise::rotate_right(binary::decode("10001000 01110111 00110010"), 7, bitwise::carry_through::enabled)) == "01100101 00010000 11101110");


  LOG_INFO("\n\n---[ Pseudo-Random Number Generator ]---\n");
  LOG_INFO(prng::number<unsigned int>());
  LOG_INFO(static_cast<int>(prng::number<uint8_t>()));
  LOG_INFO(prng::number<double>());
  LOG_INFO(prng::number<unsigned long long int>());
  LOG_INFO(prng::number_between<unsigned int>(0, 10));
  LOG_INFO(prng::number_between<uint16_t>(0, 1));
  LOG_INFO(prng::number_between<double>(-1, 1));
  LOG_INFO(hex::encode(prng::bytes(16)));


  LOG_INFO("\n\n---[ Hamming Distance ]---\n");
  LOG_INFO("Hamming distance between 'this is a test' and 'wokka wokka!!!' is: " << analysis::hamming_distance("this is a test", "wokka wokka!!!"));
  assert(analysis::hamming_distance("this is a test", "wokka wokka!!!") == 37);
  LOG_INFO(analysis::hamming_distance("this is a testEXTRASTUFF", "wokka wokka!!!"));
  assert(analysis::hamming_distance("this is a testEXTRASTUFF", "wokka wokka!!!") == 37);


  LOG_INFO("\n\n---[ Character Frequency Analysis ]---\n");
  auto freqs_1 = analysis::character_frequency(test);
  assert(freqs_1['l'] == 3 && freqs_1['o'] == 2);

  auto freqs_2 = analysis::character_frequency("Mix OF upPer AND LOWER", analysis::case_sensitivity::disabled);
  assert(freqs_2['p'] == 2 && freqs_2['e'] == 2);

  auto freqs_3 = analysis::character_frequency("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Curabitur pretium tincidunt lacus. Nulla gravida orci a odio. Nullam varius, turpis et commodo pharetra, est eros bibendum elit, nec luctus magna felis sollicitudin mauris. Integer in mauris eu nibh euismod gravida. Duis ac tellus et risus vulputate vehicula. Donec lobortis risus a elit. Etiam tempor. Ut ullamcorper, ligula eu tempor congue, eros est euismod turpis, id tincidunt sapien risus a quam. Maecenas fermentum consequat mi. Donec fermentum. Pellentesque malesuada nulla a mi. Duis sapien sem, aliquet nec, commodo eget, consequat quis, neque. Aliquam faucibus, elit ut dictum aliquet, felis nisl adipiscing sapien, sed malesuada diam lacus eget erat. Cras mollis scelerisque nunc. Nullam arcu. Aliquam consequat. Curabitur augue lorem, dapibus quis, laoreet et, pretium ac, nisi. Aenean magna nisl, mollis quis, molestie eu, feugiat in, orci. In hac habitasse platea dictumst.");
  assert(freqs_3['e'] == 114 && freqs_3['u'] == 97);

  auto freqs_4 = analysis::character_frequency(prng::bytes(16));
  analysis::print_character_frequency(freqs_4);


  LOG_INFO("\n\n---[ English Text Detection ]---\n");
  LOG_INFO(std::boolalpha << analysis::looks_like_english("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Curabitur pretium tincidunt lacus. Nulla gravida orci a odio. Nullam varius, turpis et commodo pharetra, est eros bibendum elit, nec luctus magna felis sollicitudin mauris. Integer in mauris eu nibh euismod gravida. Duis ac tellus et risus vulputate vehicula. Donec lobortis risus a elit. Etiam tempor. Ut ullamcorper, ligula eu tempor congue, eros est euismod turpis, id tincidunt sapien risus a quam. Maecenas fermentum consequat mi. Donec fermentum. Pellentesque malesuada nulla a mi. Duis sapien sem, aliquet nec, commodo eget, consequat quis, neque. Aliquam faucibus, elit ut dictum aliquet, felis nisl adipiscing sapien, sed malesuada diam lacus eget erat. Cras mollis scelerisque nunc. Nullam arcu. Aliquam consequat. Curabitur augue lorem, dapibus quis, laoreet et, pretium ac, nisi. Aenean magna nisl, mollis quis, molestie eu, feugiat in, orci. In hac habitasse platea dictumst."));
  assert(analysis::looks_like_english("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Curabitur pretium tincidunt lacus. Nulla gravida orci a odio. Nullam varius, turpis et commodo pharetra, est eros bibendum elit, nec luctus magna felis sollicitudin mauris. Integer in mauris eu nibh euismod gravida. Duis ac tellus et risus vulputate vehicula. Donec lobortis risus a elit. Etiam tempor. Ut ullamcorper, ligula eu tempor congue, eros est euismod turpis, id tincidunt sapien risus a quam. Maecenas fermentum consequat mi. Donec fermentum. Pellentesque malesuada nulla a mi. Duis sapien sem, aliquet nec, commodo eget, consequat quis, neque. Aliquam faucibus, elit ut dictum aliquet, felis nisl adipiscing sapien, sed malesuada diam lacus eget erat. Cras mollis scelerisque nunc. Nullam arcu. Aliquam consequat. Curabitur augue lorem, dapibus quis, laoreet et, pretium ac, nisi. Aenean magna nisl, mollis quis, molestie eu, feugiat in, orci. In hac habitasse platea dictumst.") == true);

  LOG_INFO(std::boolalpha << analysis::looks_like_english("ASD(*&'(*$T^&%!G$^(&*'%$^'G$%F"));
  assert(analysis::looks_like_english("ASD(*&'(*$T^&%!G$^(&*'%$^'G$%F") == false);


  LOG_INFO("\n\n---[ Code Profiling ]---\n");
  profile::benchmark([&test]()
  {
    for (int i = 0; i < 1000; ++i)
    {
      hex::encode(test);
    }
  });

  profile::benchmark([]()
  {
    for (int i = 0; i < 1000; ++i)
    {
      hex::decode("48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21");
    }
  });

  profile::benchmark([]()
  {
    for (int i = 0; i < 1000; ++i)
    {
      hex::encode(prng::number<uint32_t>());
    }
  });


  LOG_INFO("\n\n---[ PKCS7 Padding ]---\n");
  LOG_INFO(hex::encode(pkcs7::pad(test)));
  LOG_INFO(hex::encode(pkcs7::unpad(pkcs7::pad(test))));
  assert(pkcs7::unpad(pkcs7::pad(test)) == test);

  assert(pkcs7::padded(test) == false);
  assert(pkcs7::padded(pkcs7::pad(test)) == true);
  assert(pkcs7::padded(hex::decode("AA AA AA AA AA AA AA AA AA AA AA AA AA AA 02 02")) == true);
  assert(pkcs7::padded(hex::decode("AA AA AA AA AA AA AA AA AA AA AA AA AA AA 00 00")) == false);
  assert(pkcs7::padded(hex::decode("AA AA AA AA AA 03 03 03"), 8) == true);
  assert(pkcs7::padded(hex::decode("AA AA AA AA AA 0B 0B 0B 0B 0B 0B 0B 0B 0B 0B 0B")) == true);

  LOG_INFO(hex::encode(pkcs7::pad("Test")));
  assert(pkcs7::pad("Test") == hex::decode("54 65 73 74 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C"));
  LOG_INFO(hex::encode(pkcs7::pad("Test", 8)));
  assert(pkcs7::pad("Test", 8) == hex::decode("54 65 73 74 04 04 04 04"));

  
  LOG_INFO("\n\n---[ Shannon Entropy ]---\n");
  LOG_INFO("1223334444 has entropy: " << analysis::entropy("1223334444"));
  LOG_INFO(test << " has entropy: " << analysis::entropy(test));


  LOG_INFO(std::boolalpha << analysis::looks_like_english("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."));
  
  return 0;
}
