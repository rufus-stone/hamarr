#pragma once

#include <random>
#include <type_traits>

namespace hmr::prng
{
// We can reuse the random engine, so declare once statically here
static std::mt19937 random_engine{std::random_device{}()};

////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
T number() noexcept
{
  // Is it an integer type? Use std::uniform_int_distribution if so
  if constexpr (std::is_integral_v<T>)
  {
    std::uniform_int_distribution<T> distribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return distribution(random_engine);

    // ...or is it a floating point type? Use std::uniform_real_distribution if so
  } else if constexpr (std::is_floating_point_v<T>)
  {
    std::uniform_real_distribution<T> distribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return distribution(random_engine);
  }
}

////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
T number_between(T min, T max) noexcept
{
  // Is it an integer type? Use std::uniform_int_distribution if so
  if constexpr (std::is_integral_v<T>)
  {
    std::uniform_int_distribution<T> distribution(min, max);
    return distribution(random_engine);

    // ...or is it a floating point type? Use std::uniform_real_distribution if so
  } else if constexpr (std::is_floating_point_v<T>)
  {
    std::uniform_real_distribution<T> distribution(min, max);
    return distribution(random_engine);
  }
}


////////////////////////////////////////////////////////////
std::string bytes(std::size_t quantity) noexcept;

} // namespace hmr::prng
