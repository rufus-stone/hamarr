#include "hamarr/serialisation.hpp"


namespace hmr::kvp
{

////////////////////////////////////////////////////////////
std::string serialise(kvps input, char kv_delimiter, char kvp_delimiter)
{
  auto output = std::string{};

  for (auto const &kvp : input)
  {
    auto tmp = std::string{kvp.first} + kv_delimiter + std::string{kvp.second} + kvp_delimiter;
    output += tmp;
  }

  // Pop the final delimiter char off the end of the string
  output.pop_back();

  return output;
}


////////////////////////////////////////////////////////////
kvps deserialise(std::string_view input, char kv_delimiter, char kvp_delimiter)
{
  auto output = kvps{};

  // First divide the input up into groups of pairs by splitting around the kvp_delimiter
  auto const pairs = hmr::format::split(input, kvp_delimiter);

  // Now divide each group into a key-value pair and add to the output map
  for (auto const pair : pairs)
  {
    auto key_value_pair = hmr::format::split(pair, kv_delimiter);

    if (key_value_pair.size() != 2)
    {
      spdlog::error("Failed to split into a single key-value pair!");
      return kvps{};
    }

    output[key_value_pair[0]] = key_value_pair[1];
  }

  return output;
}

} // namespace hmr::kvp
