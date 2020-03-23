#pragma once

#include <string>
#include <algorithm>


namespace format
{

////////////////////////////////////////////////////////////
std::string to_upper(const std::string &input)
{
  std::string output;
  output.reserve(input.size());

  std::transform(input.begin(), input.end(), std::back_inserter(output), [](const unsigned char ch) { return std::toupper(ch); } );
  return output;
}

////////////////////////////////////////////////////////////
std::string to_lower(const std::string &input)
{
  std::string output;
  output.reserve(input.size());

  std::transform(input.begin(), input.end(), std::back_inserter(output), [](const unsigned char ch) { return std::tolower(ch); } );
  return output;
}

} // namespace format