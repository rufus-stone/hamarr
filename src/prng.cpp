#include "hamarr/prng.hpp"


namespace hmr::prng
{

////////////////////////////////////////////////////////////
std::string bytes(std::size_t quantity)
{
  std::string output;
  output.reserve(quantity);

  static std::uniform_int_distribution<uint8_t> distribution(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max());

  // Generate quantity-worth of random bytes and add to the output string.
  for (std::size_t n = 0; n < quantity; ++n)
  {
    output.push_back(static_cast<char>(distribution(random_engine)));
  }

  return output;
}

} // namespace hmr::prng
