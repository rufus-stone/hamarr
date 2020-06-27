#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <algorithm>

#include "format.hpp"
#include "logger.hpp"

namespace hmr::kvp
{

using kvps = std::map<std::string_view, std::string_view>;

////////////////////////////////////////////////////////////
std::string serialise(kvps input, char kv_delimiter = '=', char kvp_delimiter = '&')
{
  auto output = std::string{};

  for (const auto &kvp : input)
  {
    auto tmp = std::string{kvp.first} + kv_delimiter + std::string{kvp.second} + kvp_delimiter;
    output += tmp;
  }

  // Pop the final delimiter char off the end of the string
  output.pop_back();

  return output;
};


////////////////////////////////////////////////////////////
kvps deserialise(std::string_view input, char kv_delimiter = '=', char kvp_delimiter = '&')
{
  auto output = kvps{};

  // First divide the input up into groups of pairs by splitting around the kvp_delimiter
  auto pairs = hmr::format::split(input, kvp_delimiter);

  // Now divide each group into a key-value pair and add to the output map
  for (const auto pair : pairs)
  {
    auto key_value_pair = hmr::format::split(pair, kv_delimiter);

    if (key_value_pair.size() != 2)
    {
      LOG_ERROR("Failed to split into key-value pair!");
      return kvps{};
    }

    output[key_value_pair[0]] = key_value_pair[1];
  }

  return output;
};

} // namespace kvp