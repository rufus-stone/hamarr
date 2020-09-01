#pragma once

#include <string>
#include <string_view>
#include <sstream>
#include <iomanip>
#include <type_traits>
#include <iostream>

#include "format.hpp"

namespace hmr::hex
{

using namespace std::string_view_literals;

constexpr auto hex_alphabet = "0123456789ABCDEF"sv;

////////////////////////////////////////////////////////////
std::string encode(std::string_view input, bool delimited = true) noexcept
{
  std::string output;
  output.reserve(input.size() * 2);

  for (const auto &ch : input)
  {
    output.push_back(hex_alphabet[(ch & 0xF0) >> 4]);
    output.push_back(hex_alphabet[(ch & 0x0F)]);

    // Optionally add a space after each hex pair
    if (delimited)
    {
      output.push_back(' ');
    }
  }

  // Get rid of the final trailling space
  if (delimited)
  {
    output.pop_back();
  }

  return output;
}

////////////////////////////////////////////////////////////
std::string encode(const char *input, bool delimited = true) noexcept
{
  const std::string tmp(input);

  return encode(tmp, delimited);
}

////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
std::string encode(T input, bool delimited)
{
  // How many bytes is the integral value?
  const std::size_t s = sizeof(input);

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
std::string decode(std::string_view input)
{
  const std::size_t len = input.size();

  std::string output;
  output.reserve(len / 2); // If there are space chars then we'll actually need less space, but over-reserving probably hurts less than under-reserving

  // Step through the input two chars at a time
  for (std::size_t i = 0; i < len; ++i)
  {
    // Skip any space chars
    if (std::isspace(input[i]))
    {
      continue;
    }

    // Abort condition - is there enough data left?
    if (i + 2 > len)
    {
      std::cerr << "Not enough data left!\n";
      return std::string{};
    }

    // Get the next pair of nibbles, checking for invalid hex chars and normalising the input char to uppercase
    auto a = hex_alphabet.find(std::toupper(input[i++]));
    if (a == std::string::npos)
    {
      std::cerr << "Invalid hex char " << input[i - 1] << " at index " << i - 1 << "!\n";
      return std::string{};
    }

    auto b = hex_alphabet.find(std::toupper(input[i]));
    if (b == std::string::npos)
    {
      std::cerr << "Invalid hex char " << input[i] << " at index " << i << "!\n";
      return std::string{};
    }

    // Because the hex alphabet string is in order, for any given hex char the result of hex_alphabet.find() will be its numerical equivalent taken from the index into the alphabet string where the char is found
    // E.g. char '0' is at index 0 in the alphabet string, char 'F' is at index 15 (i.e. the decimal value of hex F), etc.
    // Bit shift the first index number by 4, and OR against the second index number to build a new value from variables a and b
    auto c = (a << 4) | b;

    output.push_back(static_cast<char>(c));
  }

  return output;
}


////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
T decode(std::string_view input)
{
  // Normalize the string by converting to uppercase
  std::string tmp = hmr::format::to_upper(input);

  // Strip any spaces
  tmp.erase(std::remove(std::begin(tmp), std::end(tmp), ' '), std::end(tmp));

  const std::size_t len = tmp.size();

  // Abort condition - must be even length
  if (len & 1)
  {
    std::cerr << "Hex strings must be even in length!\n";
    return T{};
  }

  // Abort condition - must contain valid hex chars
  auto e = tmp.find_first_not_of(hex_alphabet);
  if (e != std::string::npos)
  {
    std::cerr << "Invalid hex char " << tmp[e] << " at index " << e << "!\n";
    return T{};
  }

  T output = 0;

  // Abort condition - the input can be shorter than the number of bytes taken up by the output, but it cannot be longer
  if (tmp.size() / 2 > sizeof(T))
  {
    std::cerr << "Input hex string contains too much data to fit into a " << sizeof(T) << " byte type!\n";
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
