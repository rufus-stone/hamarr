#include "hamarr/base64.hpp"
#include "hamarr/exceptions.hpp"

#include <algorithm>
#include <stdexcept>
#include <sstream>

namespace hmr::base64
{

static constexpr std::size_t base64_alphabet_len = 65; // 64 alphabet chars + 1 padding char

////////////////////////////////////////////////////////////
auto encode(std::string_view input, std::string_view alphabet) -> std::string
{
  // Abort condition - is the alphabet exactly 65 chars (64 alphabet chars + 1 padding char)?
  if (alphabet.size() != base64_alphabet_len)
  {
    throw hmr::xcpt::base64::invalid_alphabet("Base64 alphabet is only " + std::to_string(alphabet.size()) + " characters long! Must be exactly 65 (64 alphabet chars + 1 padding char)!");
  }

  // Abort condition - does the alphabet contain duplicate entries?
  if (is_invalid_alphabet(alphabet))
  {
    auto ss = std::stringstream{};
    ss << "Base64 alphabet has duplicate characters: " << alphabet;
    throw hmr::xcpt::base64::invalid_alphabet(ss.str());
  }

  std::size_t len = input.size();

  auto output = std::string{};
  output.reserve(len * (4 / 3)); // Base64 encoding turns 3 bytes into 4, so the resulting data is 1 1/3 times the size of the input

  // Get a uint8_t pointer to the data
  auto const *data = reinterpret_cast<uint8_t const *>(input.data());

  // Loop through the input 3 bytes at a time
  for (std::size_t i = 0; i < len; i += 3)
  {
    // Each 3 bytes is treated as one 24 bit number
    // Use the first available byte, then check to make sure there is a second and third byte left in the input data
    uint32_t n = (*data++) << 16;

    // Is there a second byte available?
    if (i + 1 < len)
    {
      n += (*data++) << 8;
    }

    // Is there a third?
    if (i + 2 < len)
    {
      n += *data++;
    }

    // Then the 24 bit number is split into 4 x 8 bit numbers
    uint8_t a = ((n >> 18) & 63);
    auto b = static_cast<uint8_t>((n >> 12) & 63);
    auto c = static_cast<uint8_t>((n >> 6) & 63);
    auto d = static_cast<uint8_t>(n & 63);

    // Finally, use these 4 numbers to look up the corresponding base64 character
    output.push_back(alphabet[a]);
    output.push_back(alphabet[b]);

    if (i + 1 < len)
    {
      output.push_back(alphabet[c]);
    }

    if (i + 2 < len)
    {
      output.push_back(alphabet[d]);
    }
  }

  // Input length should be a multiple of 3 - if not, pad with 1 or 2 padding chars
  std::size_t pad = len % 3;
  switch (pad)
  {
    case 1:
      output += std::string(2, alphabet[alphabet.size() - 1]);
      break;
    case 2:
      output += std::string(1, alphabet[alphabet.size() - 1]);
      break;
  }

  return output;
}


////////////////////////////////////////////////////////////
auto decode(std::string_view input, std::string_view alphabet) -> std::string
{
  // Abort condition - is the alphabet exactly 65 chars (64 alphabet chars + 1 padding char)?
  if (alphabet.size() != base64_alphabet_len)
  {
    throw hmr::xcpt::base64::invalid_alphabet("Base64 alphabet is only " + std::to_string(alphabet.size()) + " characters long! Must be exactly 65 (64 alphabet chars + 1 padding char)!");
  }

  // Abort condition - does the alphabet contain duplicate entries?
  if (is_invalid_alphabet(alphabet))
  {
    auto ss = std::stringstream{};
    ss << "Base64 alphabet has duplicate characters: " << alphabet;
    throw hmr::xcpt::base64::invalid_alphabet(ss.str());
  }

  auto const len = input.size();

  // Abort condition - must contain at least two chars, as valid base64 encoding always results in at least two chars
  if (len < 2)
  {
    throw hmr::xcpt::base64::need_more_data("Input is too short for valid base64! Must have at least 2 chars!");
  }

  // Abort condition - must contain valid base64 chars
  auto e = input.find_first_not_of(alphabet);
  if (e != std::string::npos)
  {
    auto ss = std::stringstream{};
    ss << "Invalid base64 char '" << input[e] << "' at index " << e << "!";
    throw hmr::xcpt::base64::invalid_input(ss.str());
  }

  // We want to ignore any padding, so check if it's present. If it is, we'll stop our base64 decoding loop at that point, otherwise we'll go until the end
  auto const *end = std::find(std::begin(input), std::end(input), alphabet[alphabet.size() - 1]);

  auto output = std::string{};
  output.reserve(len * (3 / 4)); // Base64 decoding turns 4 bytes into 3, so the resulting data is 3/4 times the size of the input

  // Lambda to perform conversion from a given base64 character to the index within the chosen base64 alphabet for that character
  auto b64_to_uint8_t = [&alphabet](char const n) noexcept -> uint8_t
  {
    auto p = alphabet.find(n); // Todo: I bet there's a more efficient way of doing this...

    // Is the char in the base64 alphabet (previous checks mean it always should be!)
    if (p != std::string::npos)
    {
      return p;
    } else
    {
      // ...or is it an invalid base64 char? Let's use 0xFF to signal this, although earlier checks mean this shouldn't happen
      return 0xFF;
    }
  };

  // We'll iterate through the input grabbing 4 bytes at a time, unless only 3 or 2 bytes remain at the end in which case we handle those slightly differently
  auto const *iter = input.begin();

  while (iter < end)
  {
    // We should always have at least 2 bytes to play with, otherwise this can't be valid base64 encoded data
    if (end - iter == 1)
    {
      throw hmr::xcpt::base64::need_more_data("Only one byte left! Need at least 2 more for valid base64!");
    }

    // How many bytes are left? 2, 3, or 4+
    auto remaining = (end - iter > 4) ? 4 : end - iter;
    switch (remaining)
    {
      case 4: {
        // Take the current block of 4 x base64 chars and look up their positions in the base64 alphabet
        uint8_t a = b64_to_uint8_t(*iter++);
        uint8_t b = b64_to_uint8_t(*iter++);
        uint8_t c = b64_to_uint8_t(*iter++);
        uint8_t d = b64_to_uint8_t(*iter++);

        // Take the 4 x 8 bit numbers from the base64 alphabet index and turn back into 1 x 24 bit number
        uint32_t n = static_cast<uint32_t>(a) << 18 | static_cast<uint32_t>(b) << 12 | static_cast<uint32_t>(c) << 6 | static_cast<uint32_t>(d);

        // Then split the 24 bit number back into 3 bytes
        auto d1 = static_cast<char>(n >> 16);
        auto d2 = static_cast<char>((n >> 8) & 0xFF);
        auto d3 = static_cast<char>(n & 0xFF);

        // Finally, add these 3 bytes of decoded data to the output
        output.push_back(d1);
        output.push_back(d2);
        output.push_back(d3);

        break;
      }
      case 3: {
        // Take the current block of 3 x base64 chars and look up their positions in the base64 alphabet
        uint8_t a = b64_to_uint8_t(*iter++);
        uint8_t b = b64_to_uint8_t(*iter++);
        uint8_t c = b64_to_uint8_t(*iter++);

        // Take the 3 x 8 bit numbers from the base64 alphabet index and turn back into 1 x 24 bit number
        uint32_t n = static_cast<uint32_t>(a) << 18 | static_cast<uint32_t>(b) << 12 | static_cast<uint32_t>(c) << 6;

        // Then split the 24 bit number back into 2 bytes
        auto d1 = static_cast<char>(n >> 16);
        auto d2 = static_cast<char>((n >> 8) & 0xFF);

        // Finally, add these 2 bytes of decoded data to the output
        output.push_back(d1);
        output.push_back(d2);

        break;
      }
      case 2: {
        // Take the current block of 2 x base64 chars and look up their positions in the base64 alphabet
        uint8_t a = b64_to_uint8_t(*iter++);
        uint8_t b = b64_to_uint8_t(*iter++);

        // Take the 3 x 8 bit numbers from the base64 alphabet index and turn back into 1 x 24 bit number
        uint32_t n = static_cast<uint32_t>(a) << 18 | static_cast<uint32_t>(b) << 12;

        // Then turn the 24 bit number back into 1 byte
        auto d1 = static_cast<char>(n >> 16);

        // Finally, add this 1 byte of decoded data to the output
        output.push_back(d1);

        break;
      }
      default:
        throw hmr::xcpt::base64::need_more_data("Only one byte left! Need at least 2 more for valid base64!");
    }
  }

  return output;
}

} // namespace hmr::base64
