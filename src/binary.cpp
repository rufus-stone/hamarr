#include "hamarr/binary.hpp"


namespace hmr::binary
{

////////////////////////////////////////////////////////////
std::string encode(std::string_view input, bool delimited)
{
  std::vector<std::string> out_vec;

  std::transform(input.begin(), input.end(), std::back_inserter(out_vec), [](uint8_t const c) -> std::string {
    auto const bits = std::bitset<8>(c);
    return bits.to_string();
  });

  return std::accumulate(out_vec.begin(), out_vec.end(), std::string(), [&delimited](auto lhs, auto rhs) -> std::string {
    if (lhs.empty())
    {
      return rhs; // This avoids inserting a blank space before the very first byte
    } else
    {
      return delimited ? lhs + " " + rhs : lhs + rhs;
    }
  });
}


////////////////////////////////////////////////////////////
std::string decode(std::string const &input)
{
  auto const len = input.size();

  std::string output;
  output.reserve(len / 8); // If there are space chars then we'll actually need less space, but over-reserving probably hurts less than under-reserving

  for (std::size_t i = 0; i < len; ++i)
  {
    // Skip any space chars
    if (std::isspace(input[i]))
    {
      continue;
    }

    // Abort condition - is there enough data left?
    if (i + 8 > len)
    {
      spdlog::error("Not enough data left! i+8 == {} but len == {}", (i + 8), len);
      return std::string{};
    }

    auto const bits = std::bitset<8>(input.substr(i, 8));
    output.push_back(bits.to_ulong());

    i += 7;
  }

  return output;
}

} // namespace hmr::binary
