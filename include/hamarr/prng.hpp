#pragma once

#include <random>
#include <type_traits>
#include <string>
#include <vector>

namespace hmr::prng
{

////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
auto number() noexcept -> T
{
  std::mt19937 random_engine{std::random_device{}()};

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
auto number_between(T min, T max) noexcept -> T
{
  std::mt19937 random_engine{std::random_device{}()};

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
auto bytes(std::size_t const quantity) -> std::string;

} // namespace hmr::prng
