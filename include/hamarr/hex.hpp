#pragma once

#include <string>
#include <string_view>
#include <sstream>
#include <iomanip>
#include <type_traits>
#include <algorithm>

#include <spdlog/spdlog.h>

namespace hmr::hex
{

using namespace std::string_view_literals;

constexpr auto hex_alphabet = "0123456789ABCDEF"sv;

////////////////////////////////////////////////////////////
std::string encode(std::string_view input, bool delimited = true) noexcept;

////////////////////////////////////////////////////////////
std::string encode(char const *input, bool delimited = true) noexcept;

////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
std::string encode(T input, bool delimited = true)
{
  // How many bytes is the integral value?
  std::size_t const s = sizeof(input);

  std::string output;
  output.reserve(s);

  // There's probably a smarter way to do this, but for now just manually code in options for 1, 2, 4 and 8 byte integrals
  switch (s)
  {
    case 1:
      output.push_back(hex_alphabet[(input & 0xF0) >> 4]);
      output.push_back(hex_alphabet[(input & 0x0F)]);
      break;

    case 2:
      output.push_back(hex_alphabet[(input & 0xF000) >> 12]);
      output.push_back(hex_alphabet[(input & 0x0F00) >> 8]);
      if (delimited) { output.push_back(' '); }
      output.push_back(hex_alphabet[(input & 0x00F0) >> 4]);
      output.push_back(hex_alphabet[(input & 0x000F)]);
      break;

    case 4:
      output.push_back(hex_alphabet[(input & 0xF0000000) >> 28]);
      output.push_back(hex_alphabet[(input & 0x0F000000) >> 24]);
      if (delimited) { output.push_back(' '); }
      output.push_back(hex_alphabet[(input & 0x00F00000) >> 20]);
      output.push_back(hex_alphabet[(input & 0x000F0000) >> 16]);
      if (delimited) { output.push_back(' '); }
      output.push_back(hex_alphabet[(input & 0x0000F000) >> 12]);
      output.push_back(hex_alphabet[(input & 0x00000F00) >> 8]);
      if (delimited) { output.push_back(' '); }
      output.push_back(hex_alphabet[(input & 0x000000F0) >> 4]);
      output.push_back(hex_alphabet[(input & 0x0000000F)]);
      break;

    case 8:
      output.push_back(hex_alphabet[(input & 0xF000000000000000) >> 60]);
      output.push_back(hex_alphabet[(input & 0x0F00000000000000) >> 56]);
      if (delimited) { output.push_back(' '); }
      output.push_back(hex_alphabet[(input & 0x00F0000000000000) >> 52]);
      output.push_back(hex_alphabet[(input & 0x000F000000000000) >> 48]);
      if (delimited) { output.push_back(' '); }
      output.push_back(hex_alphabet[(input & 0x0000F00000000000) >> 44]);
      output.push_back(hex_alphabet[(input & 0x00000F0000000000) >> 40]);
      if (delimited) { output.push_back(' '); }
      output.push_back(hex_alphabet[(input & 0x000000F000000000) >> 36]);
      output.push_back(hex_alphabet[(input & 0x0000000F00000000) >> 32]);
      if (delimited) { output.push_back(' '); }
      output.push_back(hex_alphabet[(input & 0x00000000F0000000) >> 28]);
      output.push_back(hex_alphabet[(input & 0x000000000F000000) >> 24]);
      if (delimited) { output.push_back(' '); }
      output.push_back(hex_alphabet[(input & 0x0000000000F00000) >> 20]);
      output.push_back(hex_alphabet[(input & 0x00000000000F0000) >> 16]);
      if (delimited) { output.push_back(' '); }
      output.push_back(hex_alphabet[(input & 0x000000000000F000) >> 12]);
      output.push_back(hex_alphabet[(input & 0x0000000000000F00) >> 8]);
      if (delimited) { output.push_back(' '); }
      output.push_back(hex_alphabet[(input & 0x00000000000000F0) >> 4]);
      output.push_back(hex_alphabet[(input & 0x000000000000000F)]);
      break;

    default:
      return std::string{};
  }

  return output;
}


////////////////////////////////////////////////////////////
std::string decode(std::string_view input);


////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
T decode(std::string_view input)
{
  // Normalize the string by converting to uppercase
  auto to_upper = [](std::string_view input_)
  {
    std::string output_;
    output_.reserve(input_.size());

    std::transform(input_.begin(), input_.end(), std::back_inserter(output_), [](unsigned char const ch) { return std::toupper(ch); });
    return output_;
  };

  std::string tmp = to_upper(input);

  // Strip any spaces
  tmp.erase(std::remove(std::begin(tmp), std::end(tmp), ' '), std::end(tmp));

  auto const len = tmp.size();

  // Abort condition - must be even length
  if (len & 1)
  {
    spdlog::error("Hex strings must be even in length!");
    return T{};
  }

  // Abort condition - must contain valid hex chars
  auto e = tmp.find_first_not_of(hex_alphabet);
  if (e != std::string::npos)
  {
    spdlog::error("Invalid hex char {} at index {} !", tmp[e], e);
    return T{};
  }

  T output = 0;

  // Abort condition - the input can be shorter than the number of bytes taken up by the output, but it cannot be longer
  if (tmp.size() / 2 > sizeof(T))
  {
    spdlog::error("Input hex string contains too much data to fit into a {} byte type!", sizeof(T));
    return T{};
  }

  // Step through the input two chars at a time
  for (std::size_t i = 0; i < len; i += 2)
  {
    auto a = hex_alphabet.find(tmp[i]);
    auto b = hex_alphabet.find(tmp[i + 1]);

    // Because the hex alphabet string is in order, for any given hex char the result of hex_alphabet.find() will be its numerical equivalent taken from the index into the alphabet string where the char is found
    // E.g. char '0' is at index 0 in the alphabet string, char 'F' is at index 15 (i.e. the decimal value of hex F), etc.
    // Bit shift the first index number by 4, and OR against the second index number to build a new value from variables a and b
    auto c = (a << 4) | b;

    // Shift the output one byte to the left and tack on the next byte - this will keep adding bytes to the output until we exhaust the input
    output = (output << 8) | static_cast<T>(c);
  }

  return output;
}

} // namespace hmr::hex
