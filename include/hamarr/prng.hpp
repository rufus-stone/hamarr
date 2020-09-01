#pragma once

#include <random>
#include <type_traits>

namespace hmr::prng
{
// We can reuse the random engine, so declare once statically here
static std::mt19937 random_engine{std::random_device{}()};

////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
static T number()
{
  // Is it an integer type? Use std::uniform_int_distribution if so
  if constexpr (std::is_integral_v<T>)
  {
    static std::uniform_int_distribution<T> distribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return distribution(random_engine);

    // ...or is it a floating point type? Use std::uniform_real_distribution if so
  } else if constexpr (std::is_floating_point_v<T>)
  {
    static std::uniform_real_distribution<T> distribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return distribution(random_engine);
  }
}

////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
static T number_between(T min, T max)
{
  // Is it an integer type? Use std::uniform_int_distribution if so
  if constexpr (std::is_integral_v<T>)
  {
    static std::uniform_int_distribution<T> distribution(min, max);
    return distribution(random_engine);

    // ...or is it a floating point type? Use std::uniform_real_distribution if so
  } else if constexpr (std::is_floating_point_v<T>)
  {
    static std::uniform_real_distribution<T> distribution(min, max);
    return distribution(random_engine);
  }
}


////////////////////////////////////////////////////////////
static std::string bytes(std::size_t quantity)
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
