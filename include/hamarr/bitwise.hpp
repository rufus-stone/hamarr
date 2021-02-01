#pragma once

#include <string>
#include <string_view>

namespace hmr::bitwise
{

enum class xor_differential { input,
  output };

enum class carry_through { enabled,
  disabled };

////////////////////////////////////////////////////////////
auto xor_with_key(std::string_view input, std::string_view key) noexcept -> std::string;


////////////////////////////////////////////////////////////
auto xor_with_key(std::string_view input, uint8_t key) noexcept -> std::string;


////////////////////////////////////////////////////////////
auto xor_rolling(std::string_view input, xor_differential differential = xor_differential::input) noexcept -> std::string;


////////////////////////////////////////////////////////////
auto xor_counter(std::string_view input, uint8_t key = 0x00, int increment = 1) noexcept -> std::string;


////////////////////////////////////////////////////////////
auto shift_left(std::string_view input, std::size_t amount = 1) noexcept -> std::string;


////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> && !std::is_base_of_v<std::string, T>>>
auto shift_left(T input, std::size_t amount = 1) noexcept -> T
{
  if (amount > (sizeof(T) * 8))
  {
    amount %= (sizeof(T) * 8);
  }

  return (input << amount);
}


////////////////////////////////////////////////////////////
auto shift_right(std::string_view input, std::size_t amount = 1) noexcept -> std::string;


////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> && !std::is_base_of_v<std::string, T>>>
auto shift_right(T input, std::size_t amount = 1) noexcept -> T
{
  if (amount > (sizeof(T) * 8))
  {
    amount %= (sizeof(T) * 8);
  }

  return (input >> amount);
}


////////////////////////////////////////////////////////////
auto rotate_left(std::string_view input, std::size_t amount = 1, carry_through carry = carry_through::disabled) noexcept -> std::string;


////////////////////////////////////////////////////////////
auto rotate_right(std::string_view input, std::size_t amount = 1, carry_through carry = carry_through::disabled) noexcept -> std::string;

} // namespace hmr::bitwise
