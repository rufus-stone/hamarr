#pragma once

#include <string>
#include <string_view>

namespace hmr::bitwise
{

enum class xor_differential { input, output };

enum class carry_through { enabled, disabled };

////////////////////////////////////////////////////////////
std::string xor_with_key(std::string_view input, std::string_view key) noexcept;


////////////////////////////////////////////////////////////
std::string xor_with_key(std::string_view input, uint8_t key) noexcept;


////////////////////////////////////////////////////////////
std::string xor_rolling(std::string_view input, xor_differential differential = xor_differential::input) noexcept;


////////////////////////////////////////////////////////////
std::string xor_counter(std::string_view input, uint8_t key = 0x00, int increment = 1) noexcept;


////////////////////////////////////////////////////////////
std::string shift_left(std::string_view input, std::size_t amount = 1) noexcept;


////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> && !std::is_base_of_v<std::string, T>>>
T shift_left(T input, std::size_t amount = 1) noexcept
{
  if (amount > (sizeof(T) * 8))
  {
    amount %= (sizeof(T) * 8);
  }

  return (input << amount);
}


////////////////////////////////////////////////////////////
std::string shift_right(std::string_view input, std::size_t amount = 1) noexcept;


////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> && !std::is_base_of_v<std::string, T>>>
T shift_right(T input, std::size_t amount = 1) noexcept
{
  if (amount > (sizeof(T) * 8))
  {
    amount %= (sizeof(T) * 8);
  }

  return (input >> amount);
}


////////////////////////////////////////////////////////////
std::string rotate_left(std::string_view input, std::size_t amount = 1, carry_through carry = carry_through::disabled) noexcept;


////////////////////////////////////////////////////////////
std::string rotate_right(std::string_view input, std::size_t amount = 1, carry_through carry = carry_through::disabled) noexcept;

} // namespace hmr::bitwise
