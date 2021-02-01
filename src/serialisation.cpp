#include "hamarr/serialisation.hpp"

#include <algorithm>
#include <vector>

#include "hamarr/format.hpp"
#include "hamarr/exceptions.hpp"

namespace hmr::kvp
{

////////////////////////////////////////////////////////////
auto serialise(kvps const &input, char kv_delimiter, char kvp_delimiter) -> std::string
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
auto deserialise(std::string_view input, char kv_delimiter, char kvp_delimiter) -> kvps
{
  auto output = kvps{};

  // First divide the input up into groups of pairs by splitting around the kvp_delimiter
  auto const pairs = hmr::fmt::split(input, kvp_delimiter);

  // Now divide each group into a key-value pair and add to the output map
  for (auto const &pair : pairs)
  {
    auto key_value_pair = hmr::fmt::split(pair, kv_delimiter);

    if (key_value_pair.size() != 2)
    {
      throw hmr::xcpt::kvp::parse_error("Failed to split into a single key-value pair!");
    }

    output[key_value_pair[0]] = key_value_pair[1];
  }

  return output;
}

} // namespace hmr::kvp
