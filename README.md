# Utils

A collection of header-only C++ utilities

This began life as things I found useful when playing around with the first few [cryptopals challenges](https://cryptopals.com), but has grown to include other kinds of data manipulation operations, functions for generating psuedo-random numbers, etc. Basically it's a bunch of stuff that I wish was already available as easy-to-use functions.

## Installation

Pop the contents of the include folder into your project and include the utils.hpp file

`#include "utils.hpp"`

### Requirements

C++17 or newer


## Usage

### Logger

The `LOG_INFO()` and `LOG_ERROR()` macros are just simple wrappers around `std::cout` and `std::cerr` for printing messages to the console. They end with a newline. For example:

```cpp
LOG_INFO("Blah blah blah " << 123); // Prints: "Blah blah blah 123"

LOG_ERROR("Some message " << 54321); // Prints: "Some message 54321"
```

### Hex

To convert to/from hexadecimal string representations of data, there are various overloads of two functions:

`hex::encode()`

`hex::decode()`

The `hex::encode()` function can take a `std::string`, a `const char*`, or any kind of integral value as its input, and will return a `std::string` containing the hex representation of the input. For example:

```cpp
auto encoded = hex::encode("Hello, World!"); // encoded contains the string "48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21"

encoded = hex::encode(27); // encoded contains the string "00 00 00 1B" (as an int is normally 4 bytes)

encoded = hex::encode(uint8_t{27}); // encoded contains the string "1B"
```

You can disable the insertion of spaces after each hex pair by adding the argument `false` to the function parameters. This is set to `true` by default. For example:

```cpp
auto encoded = hex::encode("Hello, World!", false); // encoded contains the string "48656C6C6F2C20576F726C6421"

encoded = hex::encode(uint16_t(4660), false); // encoded contains the string "1234"
```

The `hex::decode()` function has two variants. The un-templated variant takes a `std::string` as its input and returns a `std::string` containing the decoded bytes. It ignores any whitespace, so it doesn't matter if you feed it a hex string with spaces between the hex pairs or not, and is case insensitive. For example:

```cpp
auto decoded = hex::decode("48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21"); // decoded contains the string "Hello, World!"

decoded = hex::decode("48656C6C6F2C20576F726C6421"); // decoded still contains the string "Hello, World!"

decoded = hex::decode("48 65 6c 6C 6f 2C 20 57 6f 72 6C 64 21"); // hex::decode() internally normalises to all uppercase before decoding, so inputs are case insensitive
```

The templated variant takes a `std::string` as its input but returns an integral value of the type specified. The number of bytes worth of hex chars in the input must be equal to or less than the size of the return type. For example:

```cpp
auto decoded_1 = hex::decode<uint8_t>("FF"); // decoded is a uint8_t with the value 255 (0xFF)

auto decoded_2 = hex::decode<uint16_t>("00 10"); // decoded is a uint16_t with the value 16 (0x0010)

auto decoded_3 = hex::decode<uint32_t>("FF 00 FF 00"); // decoded is a uint32_t with the value 4278255360 (0xFF00FF00)

auto decoded_4 = hex::decode<int16_t>("FF 00"); // decoded_4 is a int16_t with the value -256 (0xFF00)

auto decoded_5 = hex::decode<uint16_t>("FF"); // decoded_5 is a uint16_t with the value 255 (0x00FF) - although the input contained only a single byte worth of hex chars, a valid uint16_t can be built

auto decoded_6 = hex::decode<uint8_t>("FF FF FF FF"); // The input contains more bytes worth of hex chars than can fit in a uint8_t, so a default/uninitialized uint8_t is returned - Todo: would a std::optional be better??
```

If the input to `hex::decode()` contains invalid hex characters, or is uneven in length (not counting any whitespace), an empty string is returned (or in the case of the templated variant, a default value is returned).

- Todo: Should the return type of the templated variant (or indeed, of all these functions?) be a `std::optional`? Would that make handling incorrect inputs easier?


### Binary

To convert to/from binary string representations of data, there are two functions:

`binary::encode()`

`binary::decode()`

The `binary::encode()` function can take a `std::string` or any kind of integral value as its input, and will return a `std::string` containing the binary representation of the Input. Similar to `hex::encode()`, you can optionally disable the insertion of spaces after each byte's worth of bits. This functionality is enabled by default, so to disable it add the argument `false` to the function. For example:

```cpp
auto encoded = binary::encode("Hello, World!"); // encoded contains the string "01001000 01100101 01101100 01101100 01101111 00101100 00100000 01010111 01101111 01110010 01101100 01100100 00100001"

encoded = binary::encode("Hello, World!", false); // encoded contains the string "01001000011001010110110001101100011011110010110000100000010101110110111101110010011011000110010000100001"

encoded = binary::encode(uint16_t(4660)); // encoded contains the string "00010010 00110100"
```

Decoding from binary is similarly straightforward (it also ignores any whitespace), although there are two variants. The un-templated variant takes a `std::string` as its input and returns a `std::string` made from the decoded binary. For example:

```cpp
auto decoded = binary::decode("01001000 01100101 01101100 01101100 01101111 00101100 00100000 01010111 01101111 01110010 01101100 01100100 00100001"); // decoded contains the string "Hello, World!"
```

The templated variant takes a `std::string` as its input but returns an integral value of the type specified. The number of bytes worth of bits in the input must exactly match the size of the return type. For example:

```cpp
auto decoded_1 = binary::decode<uint8_t>("11111111"); // decoded_1 is a uint8_t with the value 255 (0xFF)

auto decoded_2 = binary::decode<uint32_t>("11111111 00000000 11111111 00000000"); // decoded_2 is a uint32_t with the value 4278255360 (0xFF00FF00)

auto decoded_3 = binary::decode<int16_t>("11111111 00000000"); // decoded_3 is a int16_t with the value -256 (0xFF00)
```

If the input to `binary::decode()` contains anything other than 1s and 0s (and whitespace), or its length is not divisible by 8 (not counting any whitespace), an empty string is returned (or in the case of the templated variant, a default value is returned).

- Todo: Allow the templated variant to work even if the input does not exactly match the size of the return type, e.g. allow an input of "11111111" to produce a `uint16_t` with the value 255 (0x00FF)
- Todo: Should the return type of the templated variant (or indeed, of all these functions?) be a `std::optional`? Would that make handling incorrect inputs easier?


### Base64

To convert to/from base64 string representations of data, there are two functions:

`base64::encode()`

`base64::decode()`

These both take a `std::string` input, and return a `std::string` output. They use the standard base64 alphabet of A-Za-z0-9+/ with = as the padding character. For example:

```cpp
auto encoded = base64::encode("Hello, World!"); // encoded contains the string "SGVsbG8sIFdvcmxkIQ=="

auto decoded = base64::decode("SGVsbG8sIFdvcmxkIQ=="); // decoded contains the string "Hello, World!"
```

For both functions, you can optionally specify a custom base64 alphabet by passing this as the second argument to the relevant function. This alphabet must contain exactly 64 characters, the final character of which will be used as the padding character. For example:

```cpp
auto encoded = base64::encode("Hello, World!", "abcdefgh0123456789ijklmnopqrstuvwxyz=/ABCDEFGHIJKLMNOPQRSTUVWXYZ+"); // encoded contains the string "iglGrgWG0ftJsAL=08++"

auto decoded = base64::decode("iglGrgWG0ftJsAL=08++", "abcdefgh0123456789ijklmnopqrstuvwxyz=/ABCDEFGHIJKLMNOPQRSTUVWXYZ+"); // decoded contains the string "Hello, World!"
```

- Todo: Allow the user to toggle on/off the insertion of padding characters
- Todo: Add checks to ensure that padding characters are not found in the middle of the input data
- Todo: Add checks to ensure that custom base64 alphabets do not contain duplicate characters


### URL encoding

To convert to/from URL encoded string representations of data, there are two functions:

`url::encode()`

`url::decode()`

These both take a `std::string` input, and return a `std::string` output. Any characters that fall outside the list of unreserved characters (as defined in RFC 3986, i.e. the alphanumeric chars, plus '-', '.', '_', and '~') are percent-encoded. For example:

```cpp
auto encoded = url::encode("Hello, World!"); // encoded contains the string "Hello%2C%20World%21"

auto decoded = url::encode("Hello%2C%20World%21"); // decoded contains the string "Hello, World!"
```

According to RFC 3986, URL encoding routines should convert non-ASCII characters to their UTF-8 byte sequence before percent-encoding. For single bytes, this means any value between \x80-\xFF gets turned into the appropriate two-byte UTF-8 sequence. This is the default behaviour for `url::encode()` (and for `url::decode()` in reverse), but you can toggle lazy mode by adding the argument `true` to the function, which will prevent any conversions to/from UTF-8. For example:

```cpp
auto encoded = url::encode(hex::decode("33 44 55 66 77 88 99 AA BB")); // encoded contains the string "3DUfw%C2%88%C2%99%C2%AA%C2%BB" - hex values above \x7F have been converted into two-byte UTF-8 sequences

encoded = url::encode(hex::decode("33 44 55 66 77 88 99 AA BB"), true); // encoded contains the string "3DUfw%88%99%AA%BB" - with lazy encoding enable, no conversion to UTF-8 takes place
```

When decoding, make sure to enable or disable lazy mode as appropriate, otherwise your data will be corrupted. For example:

```cpp
auto decoded = url::decode("3DUfw%C2%88%C2%99%C2%AA%C2%BB"); // UTF-8 conversion is on by default, so decoded contains the hex value: 33 44 55 66 77 88 99 AA BB

decoded = url::decode("3DUfw%C2%88%C2%99%C2%AA%C2%BB", true); // Lazy mode is on, so decoded contains the hex value: 33 44 55 66 77 C2 88 C2 99 C2 AA C2 BB
```

When decoding, if the input string ends prematurely, or an invalid UTF-8 sequence is found, an empty string is returned.

- Todo: Add support for user-defined lists of reserved/unreserved characters


### Random number generator

To generate various kinds of pseudo-random numbers, there are two templated functions:

`prng::number<>()`

`prng::number_between<>()`;

Both functions require you to specify the type of random number you want the function to return (`int`, `double`, `uint32_t`, etc.). `prng::number<>()` will return a randomly chosen value of the specified type between the minimum and maximum possible values for that type. For example:

```cpp
auto random_double = prng::number<double>(); // random_double will be a double set to any of the possible values for a double

auto random_uint8t = prng::number<uint8_t>(); // random_uint8t will be a uint8_t set to any of the possible values for a uint8_t
```

`prng::number_between<>()` allows you to specify upper and lower limits for the randomly generated number. These limits are inclusive. For example:

```cpp
auto random_double = prng::number_between<double>(0, 10); // random_double will be a double between 0 and 10 inclusive

auto random_float = prng::number_between<float>(-1, 1); // random_float will be a float between -1 and 1 inclusive
```

To generate a string of random bytes, there is the following function:

`prng::bytes()`

This takes a `std::size_t` as its input, and returns a `std::string` populated with the specified number of randomly generated bytes. For example:

```cpp
auto random_string = prng::bytes(16); // random_string will be a std::string of length 16 populated with randomly selected byte values
```

- Todo: Add a templated variant to support returning a different kind of container filled with random data, e.g. a `std::vector<uint8_t>`


### Bitwise operations

##### XOR

To apply various kinds of XOR, there are various overloads of three functions:

`bitwise::xor_with_key()`

`bitwise::xor_rolling()`

`bitwise::xor_counter()`

The function `bitwise::xor_with_key()` takes two inputs - a `std::string` for the data, and either a `std::string` or `uint8_t` for the key. It performs an XOR across the data using the key, and returns the result as a `std::string`. For example:

```cpp
auto xord = bitwise::xor_with_key("Example data", hex::decode("01 2A 8C")); // XORs "Example data" with the 3-byte key \x01\x2A\x8C -> xord is a std::string containing "\x44\x52\xED\x6C\x5A\xE0\x64\x0A\xE8\x60\x5E\xED"

xord = bitwise::xor_with_key("Example data", 0x42); // XORs "Example data" with the single byte key \x42
```

The function `bitwise::xor_rolling()` takes a `std::string` input and returns a `std::string` output. It performs a rolling XOR where each byte of the input is XORed against the previous byte (the very first byte is un-changed). By default, this uses input differential mode, where each byte of the input is XORed against the original pre-XOR value of the previous byte. To enable output differential mode, where each byte is XORed with the resulting post-XOR value of the previous byte, add the argument `bitwise::xor_differential::output` to the function. For example:

```cpp
auto xord = bitwise::xor_rolling("Hello!"); // Input differential mode is used by default, so xord contains the hex value: 48 2D 09 00 03 4E

xord = bitwise::xor_rolling("Hello!", bitwise::xor_differential::output); // The scoped enum bitwise::xor_differential::output enables output differential mode, so xord contains the hex value: 48 2D 41 2D 42 63
```

The function `bitwise::xor_counter()` takes a `std::string` input and returns a `std::string` output. It implements a counter-based XOR where the value of the key counts up or down by the specified incrementation amount after each byte of the input has been processed. The starting value for the key defaults to \x00, and the incrementation defaults to +1, but both of these can be changed. The initial key value is specified by passing a `uint8_t` as the second argument to the function, and the incrementation value is specified by passing a signed `int` as the third argument. If the incremention would cause the key value to increase above \xFF, or decrease below \x00, then the key value wraps back around. For example:

```cpp
auto xord = bitwise::xor_counter("Hello, World!"); // Defaults to initial key value \x00 and incremention value +1, resulting in key stream: 00 01 02 03 04 05...etc., meaning xord contains the hex value: 48 64 6E 6F 6B 29 26 50 67 7B 66 6F 2D

xord = bitwise::xor_counter("Hello, World!", '\xAA'); // Initial key value \xAA and default incremention value +1, resulting in key stream: AA AB AC AD AE AF...etc., meaning xord contains the hex value: E2 CE C0 C1 C1 83 90 E6 DD C1 D8 D1 97

xord = bitwise::xor_counter("Hello, World!", '\xEF', 6); // Initial key value \xEF and incremention value +6, resulting in key stream: EF F5 FB 01 07 0D...etc., meaning xord contains the hex value: A7 90 97 6D 68 21 33 4E 70 57 47 55 16

xord = bitwise::xor_counter("Hello, World!", '\x04', -2); // Initial key value \x04 and incremention value -2, resulting in key stream: 04 02 00 FE FC FA...etc., meaning xord contains the hex value: 4C 67 6C 92 93 D6 D8 A1 9B 80 9C 8A CD
```

- Todo: Allow `bitwise::xor_counter()` to take a `std::string` for the initial key value

##### Bit Shift

To perform left/right bit shifts, there are two functions:

`bitwise::shift_left()`

`bitwise::shift_right()`

There are two variants of each function. The un-templated variant takes a `std::string` input and a `std::size_t` specifying the shift amount (this defaults to 1 if omitted), and returns a `std:string` output which contains the result of shifting each byte of the input by the specified amount. For example:

```cpp
auto shifted_left = bitwise::shift_left(binary::decode("00110011 00000001")); // shifted_left is a string containing bytes with the binary value: 01100110 00000010
shifted_left = bitwise::shift_left(binary::decode("11110000 00001111"), 4); // shifted_left is a string containing bytes with the binary value: 00000000 11110000

auto shifted_right = bitwise::shift_right(binary::decode("11110000"), 3); // shifted_right is a string containing bytes with the binary value: 00011110
```

The templated variant takes an integral value as its input and a `std::size_t` specifying the shift amount, and returns an integral value of the same type. For example:

```cpp
auto shifted_right = bitwise::shift_right<uint8_t>(27, 3); // shifted_right is a uint8_t with the value 3 (binary 00000011)

```

##### Bit Rotation

To perform left/right bit rotation, there are two function:

`bitwise::rotate_left()`

`bitwise::rotate_right()`

These both take a `std::string` input and a `std::size_t` specifying the rotation amount (this defaults to 1 if omitted), and return a `std::string` output containing the result of rotating each byte in the input by the specified amount. By default, these functions perform a bitwise rotation on each byte of the input in isolation (without carry through). To enable carry through, where bits from one byte can be carried through to the next byte, add the argument `bitwise::carry_through::enabled` to the function. For example:

```cpp
auto rotate_left = bitwise::rotate_left(binary::decode("11110000 00111100", 4)); // rotate_left is a string containing bytes with the binary value: 00001111 11000011

rotate_left = bitwise::rotate_left(binary::decode("11110000 00111100", 4), bitwise::carry_through::enabled); // rotate_left is a string containing bytes with the binary value: 00000011 11001111

auto rotate_right = bitwise::rotate_left(binary::decode("11110000 00111100", 2)); // rotate_right is a string containing bytes with the binary value: 11000011 11110000

rotate_right = bitwise::rotate_left(binary::decode("11110000 00111100", 2), bitwise::carry_through::enabled); // rotate_rotate_rightleft is a string containing bytes with the binary value: 11000000 11110011
```


### Hamming distance

The Hamming distance between two equal-length inputs is the number of bits that differ. This can be calculated using the following function:

`analysis::hamming_distance()`

This takes two `std::string` inputs, and return a `std::size_t` output set to the number of bits that differ between each input. For example:

```cpp
auto diff = analysis::hamming_distance("this is a test", "wokka wokka!!!"); // diff is set to 37
```

If the inputs are not of equal length, only the shorter length input's worth of bits will be compared. For example:

```cpp
auto diff = analysis::hamming_distance("this is a testEXTRASTUFF", "wokka wokka!!!"); // diff is still set to 37, as EXTRASTUFF is ignored
```

- Todo: Add support for non-string inputs


### Character frequency

The function `analysis::character_frequency()` can be used to count the number of times each character occurs in a given string. This takes a `std::string` input, and returns a `std::vector<std::size_t>` containing a count for each byte. The index into the `std::vector` is the byte value, and the value at that index is the count. By default this is case sensitive, but to make it case insensitive add the argument `analysis::case_sensitivity::disabled` to the function. For example:

```cpp
auto freqs = analysis::character_frequency("Mix OF upPer AND LOWER"); // freqs['p'] == 1, freqs['P'] == 1, freqs['e'] == 1

freqs = analysis::character_frequency("Mix OF upPer AND LOWER", analysis::case_sensitivity::disabled); // freqs['p'] == 2, freqs['e'] == 2
```


### Code profiling

To measure the execution time for a given bit of code, there is the following function:

`profile::benchmark()`

This function takes any invocable as its input, and returns a `std::size_t` populated with the number of nanoseconds that the invocable took to run. For example:

```cpp
void do_stuff()
{
  // Do some stuff
}

// Measure how many nanoseconds it takes to execute do_stuff()
auto nanoseconds_taken = profile::benchmark(do_stuff);
```

Using a lambda is a convenient way to benchmark more complex behaviour. For example:

```cpp
// Measure how many nanoseconds it takes to execute a series of steps 100,000 times
auto nanoseconds_taken = profile::benchmark([]()
{
  for (int i = 0; i < 100000; ++i)
  {
    auto result = some_very_complicated_function();
    do_even_more_stuff(result);
    do_different_stuff(result);
  }
});
```

- Todo: Allow the user to give names to benchmark tests?


### PKCS7 padding

To apply or remove PKCS7 padding, there are two functions:

`pkcs7::pad()`

`pkcs7::unpad()`

These both take a `std::string` input and a `std::size_t` specifying the block size (this defaults to 16 bytes if omitted), and return a `std::string` output. The `pkcs7::pad()` function will, if given an input whose length is not a multiple of the specified block size, append as many bytes as necessary to bring the length up to the next multiple of said block size. The value of the appended bytes will be equal to the quantity of byte added.

To check for the presence of PKCS7 padding, there is the following function:

`pkcs7::padded()`

This takes a `std::string` input and a `std::size_t` specifying the block size (this defaults to 16 bytes if omitted), and returns a `bool` output set to `true` if the input is PKCS7 padded, and `false` if not. It is called from within `pkcs7::unpad()`, which will return the input string unchanged if the call to `pkcs7::padded()` returns `false`.

For example:

```cpp
auto result = pkcs7::pad("Hello, World!"); // result is a string containing bytes with the hex value: "48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21 03 03 03". This is because the input is 13 bytes long, and the default block size is 16 bytes, so 3 padding bytes with the hex value \x03 are appended.

bool has_padding = pkcs7::padded(result); // has_padding is true

result = pkcs7::unpad(result); // The padding is removed from the input

has_padding = pkcs7::padded(result); // has_padding is now false

result = pkcs7::unpad(result); // Because pkcs7::unpad() calls pkcs7::padded() internally, and because the input here is not padded, pkcs7::unpad() returns the input unchanged
```
