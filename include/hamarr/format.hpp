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

  std::transform(input.begin(), input.end(), std::back_inserter(output), [](const unsigned char ch) { return std::toupper(ch); } );
  return output;
}

////////////////////////////////////////////////////////////
std::string to_lower(std::string_view input)
{
  std::string output;
  output.reserve(input.size());

  std::transform(input.begin(), input.end(), std::back_inserter(output), [](const unsigned char ch) { return std::tolower(ch); } );
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
      case 0x0A: output += "\\n"; break;
      case 0x0D: output += "\\r"; break;
      case '\\': output += "\\\\"; break;
      default:
        if ((ch >= 0x00 && ch <= 0x1F) || ch >= 0x7F)
        {
          output += ("\\x" + hmr::hex::encode(ch));
        } else
        {
          output += ch;
        }
        break;
    }
  }

  return output;
}

} // namespace format
