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

} // namespace format
