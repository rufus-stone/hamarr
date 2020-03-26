#pragma once

#include <random>
#include <type_traits>


namespace prng
{
// We can reuse the random engine, so declare once statically here
static std::mt19937 _random_engine { std::random_device{}() };

////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
static T number()
{
  // Is it an integer type? Use std::uniform_int_distribution if so
  if constexpr (std::is_integral_v<T>)
  {
    static std::uniform_int_distribution<T> _distribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return _distribution(_random_engine);

  // ...or is it a floating point type? Use std::uniform_real_distribution if so
  } else if constexpr (std::is_floating_point_v<T>)
  {
    static std::uniform_real_distribution<T> _distribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return _distribution(_random_engine);
  }
}

////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
static T number_between(T min, T max)
{
  // Is it an integer type? Use std::uniform_int_distribution if so
  if constexpr (std::is_integral_v<T>)
  {
    static std::uniform_int_distribution<T> _distribution(min, max);
    return _distribution(_random_engine);

  // ...or is it a floating point type? Use std::uniform_real_distribution if so
  } else if constexpr (std::is_floating_point_v<T>)
  {
    static std::uniform_real_distribution<T> _distribution(min, max);
    return _distribution(_random_engine);
  }
}
  
} // namespace prng