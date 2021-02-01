#include "hamarr/binary.hpp"


namespace hmr::binary
{

static constexpr std::size_t bits_per_byte = 8;

////////////////////////////////////////////////////////////
auto encode(std::string_view input, bool delimited) noexcept -> std::string
{
  auto out_vec = std::vector<std::string>{};

  std::transform(input.begin(), input.end(), std::back_inserter(out_vec), [](uint8_t const c) -> std::string
    {
      auto const bits = std::bitset<8>(c);
      return bits.to_string();
    });

  return std::accumulate(out_vec.begin(), out_vec.end(), std::string(), [&delimited](auto lhs, auto rhs) -> std::string
    {
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
auto decode(std::string const &input) -> std::string
{
  auto const len = input.size();

  auto output = std::string{};
  output.reserve(len / bits_per_byte); // If there are space chars then we'll actually need less space, but over-reserving probably hurts less than under-reserving

  for (std::size_t i = 0; i < len; ++i)
  {
    // Skip any space chars
    if (std::isspace(input[i]) != 0)
    {
      continue;
    }

    // Abort condition - is there enough data left?
    if (i + bits_per_byte > len)
    {
      auto ss = std::stringstream{};
      ss << "Not enough data left! i+8 == " << (i + bits_per_byte) << " but len == " << len;
      throw hmr::xcpt::binary::need_more_data(ss.str());
    }

    auto const bits = std::bitset<8>(input.substr(i, 8));
    output.push_back(bits.to_ulong());

    i += (bits_per_byte - 1);
  }

  return output;
}

} // namespace hmr::binary
