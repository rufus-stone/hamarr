#include "hamarr/hex.hpp"

#include <sstream>

namespace hmr::hex
{

////////////////////////////////////////////////////////////
auto encode(std::string_view input, bool delimited) noexcept -> std::string
{
  auto output = std::string{};
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
auto encode(char const *input, bool delimited) noexcept -> std::string
{
  auto const tmp = std::string{input};

  return encode(tmp, delimited);
}


////////////////////////////////////////////////////////////
auto decode(std::string_view input) -> std::string
{
  auto const len = input.size();

  auto output = std::string{};
  output.reserve(len / 2); // If there are space chars then we'll actually need less space, but over-reserving probably hurts less than under-reserving

  // Step through the input two chars at a time
  for (std::size_t i = 0; i < len; ++i)
  {
    // Skip any space chars
    if (std::isspace(input[i]) != 0)
    {
      continue;
    }

    // Abort condition - is there enough data left?
    if (i + 2 > len)
    {
      throw hmr::xcpt::hex::need_more_data("Not enough data left for valid hex pair!");
    }

    // Get the next pair of nibbles, checking for invalid hex chars and normalising the input char to uppercase
    auto a = hex_alphabet.find(std::toupper(input[i++]));
    if (a == std::string::npos)
    {
      auto ss = std::stringstream{};
      ss << "Invalid hex char " << input[i - 1] << " at index " << i - 1 << "!";
      throw hmr::xcpt::hex::invalid_input(ss.str());
    }

    auto b = hex_alphabet.find(std::toupper(input[i]));
    if (b == std::string::npos)
    {
      auto ss = std::stringstream{};
      ss << "Invalid hex char " << input[i] << " at index " << i << "!";
      throw hmr::xcpt::hex::invalid_input(ss.str());
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
auto dump(std::string_view input) -> std::string
{
  auto output = std::stringstream{};

  constexpr std::size_t bytes_per_line = 16; // TODO: Allow the user to specify this

  std::size_t const len = input.size();

  uint32_t line_num = 0;

  // How many lines-worth of bytes are there in the input?
  std::size_t const lines = len / bytes_per_line;

  // How many extra bytes for a partial line?
  std::size_t const leftovers = len % 16;

  // How many bytes is each half of a hexdump line?
  std::size_t const half_line = bytes_per_line / 2;

  // Lambda to escape unprintable chars as '.'
  auto escape = [](std::string_view unescaped) -> std::string
  {
    auto escaped = std::string{};
    std::transform(std::cbegin(unescaped), std::cend(unescaped), std::back_inserter(escaped), [](char const ch)
      {
        if (ch <= 0x1F || ch >= 0x7F)
        {
          return '.';
        } else
        {
          return ch;
        }
      });

    return escaped;
  };

  // Process all the complete lines
  for (std::size_t i = 0; i < lines; ++i)
  {
    std::size_t start = i * bytes_per_line;

    auto const lhs = hmr::hex::encode(input.substr(start, half_line));
    auto const rhs = hmr::hex::encode(input.substr(start + half_line, half_line));

    auto const line = input.substr(start, bytes_per_line);

    auto const escaped = escape(line);

    if (i != 0)
    {
      output << '\n';
    }

    output << hmr::hex::encode(line_num, false) << "  " << lhs << "  " << rhs << "  |" << escaped << '|';

    line_num += bytes_per_line;
  }

  // Add on any bytes for a final partial line
  if (leftovers > 0 && leftovers <= half_line)
  {
    auto const lhs = hmr::hex::encode(input.substr(len - leftovers));

    auto const escaped = escape(input.substr(len - leftovers));

    std::size_t remaining = ((bytes_per_line - leftovers) * 3) + 1;

    if (lines > 1)
    {
      output << '\n';
    }

    output << hmr::hex::encode(line_num, false) << "  " << lhs << std::string(remaining, ' ') << "  |" << escaped << '|';

  } else if (leftovers > half_line)
  {
    auto const lhs = hmr::hex::encode(input.substr(len - leftovers, half_line));
    auto const rhs = hmr::hex::encode(input.substr(len - leftovers + half_line, half_line));

    auto const escaped = escape(input.substr(len - leftovers));

    std::size_t remaining = ((bytes_per_line - leftovers) * 3);

    if (lines > 1)
    {
      output << '\n';
    }

    output << hmr::hex::encode(line_num, false) << "  " << lhs << "  " << rhs << std::string(remaining, ' ') << "  |" << escaped << '|';
  }

  // Add a final line just with the total size in bytes
  line_num += leftovers;
  output << '\n'
         << hmr::hex::encode(line_num, false);


  return output.str();
}


} // namespace hmr::hex
