#include "hamarr/prng.hpp"


namespace hmr::prng
{

////////////////////////////////////////////////////////////
auto bytes(std::size_t const quantity) -> std::string
{
  auto output = std::string{};
  output.reserve(quantity);

  std::mt19937 random_engine{std::random_device{}()};

  std::uniform_int_distribution<int> distribution(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max()); // MSVC doesn't like std::uniform_int_distribution<uint8_t>

  // Generate quantity-worth of random bytes and add to the output string.
  for (std::size_t n = 0; n < quantity; ++n)
  {
    output.push_back(static_cast<char>(distribution(random_engine)));
  }

  return output;
}

} // namespace hmr::prng
