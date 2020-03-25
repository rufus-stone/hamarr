#include <assert.h>

#include "utils.hpp"

int main()
{
    auto test = std::string("Hello, World!");
    LOG_INFO(test);

    LOG_INFO("\n\n---[ Formatting ]---\n");
    LOG_INFO(format::to_upper(test));
    LOG_INFO(format::to_lower(test));


    LOG_INFO("\n\n---[ Hex Strings ]---\n");
    LOG_INFO(hex::encode(test));
    LOG_INFO(hex::decode(hex::encode(test)));
    assert(hex::decode(hex::encode(test)) == test);

    LOG_INFO("\n\n---[ Hex Strings (Non-Padded) ]---\n");
    LOG_INFO(hex::encode(test, false));
    LOG_INFO(hex::decode(hex::encode(test, false)));
    assert(hex::decode(hex::encode(test, false)) == test);


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

    assert(hex::decode<uint8_t>("FF FF") == uint8_t{});
    assert(hex::decode<uint16_t>("FF FF FF") == uint16_t{});
    assert(hex::decode<uint32_t>("FF FF FF FF FF FF") == uint32_t{});
    assert(hex::decode<uint64_t>("AA BB CC DD EE FF 00 11 22 33 44 55 66 77 88 99") == uint64_t{});

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


    LOG_INFO("\n\n---[ Pseudo-Random Number Generator ]---\n");
    LOG_INFO(prng::number<unsigned int>());
    LOG_INFO(static_cast<int>(prng::number<uint8_t>()));
    LOG_INFO(prng::number<double>());
    LOG_INFO(prng::number_between<unsigned int>(0, 10));
    LOG_INFO(prng::number_between<uint16_t>(0, 1));
    LOG_INFO(prng::number_between<double>(-1, 1));


    LOG_INFO("\n\n---[ Hamming Distance ]---\n");
    LOG_INFO("Hamming distance between 'this is a test' and 'wokka wokka!!!' is: " << analysis::hamming_distance("this is a test", "wokka wokka!!!"));
    assert(analysis::hamming_distance("this is a test", "wokka wokka!!!") == 37);
    LOG_INFO(analysis::hamming_distance("this is a testEXTRASTUFF", "wokka wokka!!!"));
    assert(analysis::hamming_distance("this is a testEXTRASTUFF", "wokka wokka!!!") == 37);


    LOG_INFO("\n\n---[ Character Frequency Analysis ]---\n");
    auto freqs = analysis::character_frequency(test);
    assert(freqs['l'] == 3 && freqs['o'] == 2);
    analysis::print_character_frequency(freqs);

    return 0;
}