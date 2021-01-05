#include "hamarr/format.hpp"

#include <algorithm>

#include "hamarr/hex.hpp"
#include "hamarr/exceptions.hpp"

namespace hmr::fmt
{

////////////////////////////////////////////////////////////
std::string to_upper(std::string_view input)
{
  std::string output;
  output.reserve(input.size());

  std::transform(input.begin(), input.end(), std::back_inserter(output), [](unsigned char const ch)
    { return std::toupper(ch); });
  return output;
}


////////////////////////////////////////////////////////////
std::string to_lower(std::string_view input)
{
  std::string output;
  output.reserve(input.size());

  std::transform(input.begin(), input.end(), std::back_inserter(output), [](unsigned char const ch)
    { return std::tolower(ch); });
  return output;
}


////////////////////////////////////////////////////////////
std::string escape(std::string_view input)
{
  std::string output;
  output.reserve(input.size());

  for (auto const &c : input)
  {
    auto const ch = static_cast<uint8_t>(c);

    switch (ch)
    {
      case 0x0A:
        output += "\\n";
        break;
      case 0x0D:
        output += "\\r";
        break;
      case '\\':
        output += "\\\\";
        break;
      default:
        if (ch <= 0x1F || ch >= 0x7F)
        {
          output += ("\\x" + hmr::hex::encode(ch));
        } else
        {
          output += static_cast<char>(ch);
        }
        break;
    }
  }

  return output;
}


////////////////////////////////////////////////////////////
std::string unescape(std::string_view input)
{
  std::string output;
  output.reserve(input.size());

  for (auto pos = std::begin(input); pos != std::end(input); ++pos)
  {
    // Is it an escape sequence?
    if (*pos == '\\')
    {
      // Is there at least one char remaining?
      if (pos + 1 >= std::end(input))
      {
        throw hmr::xcpt::format::need_more_data("Need at least 1 more byte for escape sequence!");
      }

      // What kind of escape sequence is it?
      switch (*++pos)
      {
        // Control characters
        case 'a':
          output += '\a';
          break;
        case 'b':
          output += '\b';
          break;
        case 't':
          output += '\t';
          break;
        case 'n':
          output += '\n';
          break;
        case 'v':
          output += '\v';
          break;
        case 'f':
          output += '\f';
          break;
        case 'r':
          output += '\r';
          break;

        // Hex sequences
        case 'x': {
          // Are there at least two chars remaining?
          if (pos + 2 >= std::end(input))
          {
            throw hmr::xcpt::format::need_more_data("Ran out of data for hex escape sequence!");
          }

          output += hmr::hex::decode({++pos, 2});
          ++pos;
          break;
        }

        // Everything else
        default:
          output += *pos;
          break;
      }


    } else
    {
      output += *pos;
    }
  }

  return output;
}


////////////////////////////////////////////////////////////
auto split(std::string_view input, char delimiter, bool collapse_adjacent_delimiters, bool ignore_leading_delimiter) -> std::vector<std::string>
{
  // Divide the input up into segments by splitting around the delimiter
  auto segments = std::vector<std::string>{};

  auto start = std::begin(input);
  auto pos = std::begin(input);
  auto previous_pos = pos;

  while (pos != std::end(input))
  {
    pos = std::find(start, std::end(input), delimiter);

    // If the very first char was a delimiter, are we ignoring leading delimiters?
    if (pos == std::begin(input) && ignore_leading_delimiter)
    {
      // Update the start position and move onto the next iteration of the loop
      previous_pos = pos;
      start = pos + 1;
      continue;
    }

    // Does this delimiter directly follow the previous delimiter?
    if (pos == previous_pos + 1)
    {
      if (!collapse_adjacent_delimiters)
      {
        segments.emplace_back(start, pos);
      }

    } else
    {
      segments.emplace_back(start, pos);
    }

    previous_pos = pos;
    start = pos + 1;
  }

  return segments;
}


auto split(std::string_view input, std::string_view delimiters, bool collapse_adjacent_delimiters, bool ignore_leading_delimiter) -> std::vector<std::string>
{
  // Divide the input up into segments by splitting around any of the specified delimiters
  auto segments = std::vector<std::string>{};

  auto start = std::begin(input);
  auto pos = std::begin(input);
  auto previous_pos = pos;

  while (pos != std::end(input))
  {
    pos = std::find_first_of(start, std::end(input), std::begin(delimiters), std::end(delimiters));

    // If the very first char was a delimiter, are we ignoring leading delimiters?
    if (pos == std::begin(input) && ignore_leading_delimiter)
    {
      // Update the start position and move onto the next iteration of the loop
      previous_pos = pos;
      start = pos + 1;
      continue;
    }

    // Does this delimiter follow the previous delimiter?
    if (pos == previous_pos + 1)
    {
      if (!collapse_adjacent_delimiters)
      {
        segments.emplace_back(start, pos);
      }

    } else
    {
      segments.emplace_back(start, pos);
    }

    previous_pos = pos;
    start = pos + 1;
  }

  return segments;
}


////////////////////////////////////////////////////////////
auto lstrip(std::string_view input, std::string_view any_of_these) -> std::string_view
{
  std::size_t const pos = input.find_first_not_of(any_of_these);
  if (pos == std::string_view::npos)
  {
    return input;
  } else
  {
    return input.substr(pos);
  }
}


////////////////////////////////////////////////////////////
auto rstrip(std::string_view input, std::string_view any_of_these) -> std::string_view
{
  std::size_t const pos = input.find_last_not_of(any_of_these);
  if (pos == std::string_view::npos)
  {
    return input;
  } else
  {
    return input.substr(0, pos + 1);
  }
}

////////////////////////////////////////////////////////////
auto strip(std::string_view input, std::string_view any_of_these) -> std::string_view
{
  return rstrip(lstrip(input, any_of_these), any_of_these);
}

} // namespace hmr::fmt
