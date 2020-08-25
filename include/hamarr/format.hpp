#pragma once

#include <string>
#include <string_view>
#include <algorithm>


namespace hmr::format
{

////////////////////////////////////////////////////////////
std::string to_upper(std::string_view input)
{
  std::string output;
  output.reserve(input.size());

  std::transform(input.begin(), input.end(), std::back_inserter(output), [](const unsigned char ch) { return std::toupper(ch); });
  return output;
}


////////////////////////////////////////////////////////////
std::string to_lower(std::string_view input)
{
  std::string output;
  output.reserve(input.size());

  std::transform(input.begin(), input.end(), std::back_inserter(output), [](const unsigned char ch) { return std::tolower(ch); });
  return output;
}


////////////////////////////////////////////////////////////
std::string escape(std::string_view input)
{
  std::string output;
  output.reserve(input.size());

  for (const auto &c : input)
  {
    auto ch = static_cast<uint8_t>(c);

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
        LOG_ERROR("Ran out of data for escape sequence!");
        return std::string{};
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
            LOG_ERROR("Ran out of data for hex escape sequence!");
            return std::string{};
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
auto split(std::string_view input, char delimiter) -> std::vector<std::string_view>
{
  auto len = input.size();
  auto first = input.data();
  auto second = input.data();
  auto last = first + len;

  // Divide the input up into segments by splitting around the delimiter
  auto segments = std::vector<std::string_view>{};

  while (second < last && first < last)
  {
    second = std::find(first, last, delimiter);

    if (first != second)
    {
      segments.emplace_back(first, second - first);
    }

    first = second + 1;
  }

  return segments;
}

} // namespace hmr::format
