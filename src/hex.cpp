#include "hamarr/hex.hpp"


namespace hmr::hex
{

////////////////////////////////////////////////////////////
std::string encode(std::string_view input, bool delimited) noexcept
{
  std::string output;
  output.reserve(input.size() * 2);

  for (auto const &ch : input)
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
std::string encode(char const *input, bool delimited) noexcept
{
  std::string const tmp(input);

  return encode(tmp, delimited);
}


////////////////////////////////////////////////////////////
std::string decode(std::string_view input)
{
  auto const len = input.size();

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
      spdlog::error("Not enough data left for valid hex pair!");
      return std::string{};
    }

    // Get the next pair of nibbles, checking for invalid hex chars and normalising the input char to uppercase
    auto a = hex_alphabet.find(std::toupper(input[i++]));
    if (a == std::string::npos)
    {
      spdlog::error("Invalid hex char {} at index {} !", input[i - 1], i - 1);
      return std::string{};
    }

    auto b = hex_alphabet.find(std::toupper(input[i]));
    if (b == std::string::npos)
    {
      spdlog::error("Invalid hex char {} at index {} !", input[i], i);
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


} // namespace hmr::hex
