#include "hamarr/url.hpp"

#include <algorithm>
#include <sstream>

#include "hamarr/hex.hpp"
#include "hamarr/exceptions.hpp"

namespace hmr::url
{

////////////////////////////////////////////////////////////
auto encode(std::string_view input, bool lazy) noexcept -> std::string
{
  auto output = std::string{};
  output.reserve(input.size()); // The output will almost certainly be larger than this, however...

  for (auto const &c : input)
  {
    // Is it an unreserved char? If so, append unchanged
    if (unreserved_chars.find(c) != std::string_view::npos)
    {
      output.push_back(c);
      continue;
    }

    // If we get to here, then it must be a reserved char
    if (lazy) // If we're being lazy, just convert to hex and append
    {
      output += ("%" + hmr::hex::encode(c));

    } else // If not lazy, convert to UTF8 first and then append
    {
      if (static_cast<uint8_t>(c) < 0x80) // Char values less than 0x80 remain unchanged
      {
        output += ("%" + hmr::hex::encode(c));

      } else if (static_cast<uint8_t>(c) >= 0x80 && static_cast<uint8_t>(c) < 0xC0)
      {
        output += ("%C2%" + hmr::hex::encode(c));

      } else if (static_cast<uint8_t>(c) >= 0xC0)
      {
        output += ("%C3%" + hmr::hex::encode(static_cast<uint8_t>(c ^ 0x40))); // Casting to uint8_t is necessary because, even though c is a char, c ^ 40 produces an int, which is not just a single byte, so the hex::encode() function messes it up

      } else // This shouldn't happen
      {
        return std::string{};
      }
    }
  }

  return output;
}


////////////////////////////////////////////////////////////
auto decode(std::string_view input, bool lazy) -> std::string
{
  auto const len = input.size();

  auto output = std::string{};
  output.reserve(len); // If there are any percent-encoded elements then we'll actually need less space, but over-reserving probably hurts less than under-reserving

  auto is_valid_hex = [](uint8_t c) -> bool
  { return std::isxdigit(c) != 0; };

  for (std::size_t i = 0; i < len; ++i)
  {
    // Is this an escape sequence?
    if (input[i] == '%')
    {
      if (i + 2 >= len) // Need room for at least 2 more chars
      {
        throw hmr::xcpt::url::need_more_data("Not enough chars remaining to parse escape sequence!");
      }

      if (lazy) // Are we being lazy? If so, just convert back from hex and append
      {
        // Abort condition - expect valid hex chars
        if (!is_valid_hex(input[i + 1]) || !is_valid_hex(input[i + 2]))
        {
          auto ss = std::stringstream{};
          ss << "Invalid hex escape sequence: " << input.substr(i, 3);
          throw hmr::xcpt::url::invalid_input(ss.str());
        }

        output += hmr::hex::decode(std::string(input.data() + i + 1, 2));
        i += 2;
      } else
      {
        // Otherwise check for a %C2 or %C3 two-byte UTF-8 sequence
        if (input[i + 1] == 'C')
        {
          // Abort condition - need room for 5 more chars
          if (i + 5 >= len)
          {
            throw hmr::xcpt::url::need_more_data("Not enough chars remaining to parse two-byte UTF-8 escape sequence - expected 5 but only " + std::to_string(len - i) + " left!");
          }

          // Abort condition - of the following 5 chars, the 1st, 2nd, 4th and 5th must be valid hex chars
          if (!is_valid_hex(input[i + 1]) || !is_valid_hex(input[i + 2]) || !is_valid_hex(input[i + 4]) || !is_valid_hex(input[i + 5]))
          {
            auto ss = std::stringstream{};
            ss << "Invalid two-byte UTF-8 hex escape sequence: " << input.substr(i, 6);
            throw hmr::xcpt::url::invalid_input(ss.str());
          }

          if (input[i + 2] == '2')
          {
            auto adj = hmr::hex::decode(std::string(input.data() + i + 4, 2));
            output += hmr::hex::decode(std::string(input.data() + i + 4, 2));

          } else if (input[i + 2] == '3')
          {
            auto adj = static_cast<uint8_t>(hmr::hex::decode<uint8_t>(std::string(input.data() + i + 4, 2)) | 0x40);
            output.push_back(adj);

          } else
          {
            auto ss = std::stringstream{};
            ss << "No valid UTF-8 -> ASCII conversion for two-byte hex escape sequences starting: " << input.substr(i, 3);
            throw hmr::xcpt::url::invalid_input(ss.str());
          }

          i += 5;
        } else // If it wasn't a %C2 or %C3 sequence, then just convert back from hex and append
        {
          // Abort condition - expect valid hex chars
          if (!is_valid_hex(input[i + 1]) || !is_valid_hex(input[i + 2]))
          {
            auto ss = std::stringstream{};
            ss << "Invalid hex escape sequence: " << input.substr(i, 3);
            throw hmr::xcpt::url::invalid_input(ss.str());
          }

          output += hmr::hex::decode(std::string(input.data() + i + 1, 2));
          i += 2;
        }
      }
    } else // Otherwise it must be a regular character
    {
      output.push_back(input[i]);
    }
  }

  return output;
}

} // namespace hmr::url
