#pragma once

#include <string>
#include <string_view>

namespace hmr::pkcs7
{

////////////////////////////////////////////////////////////
constexpr auto padded(std::string_view input, std::size_t block_size = 16) noexcept -> bool
{
  uint8_t pad_byte = static_cast<uint8_t>(input[input.size() - 1]);

  // Is pad_byte \x00? This should never happen for pkcs7 padded data
  if (!pad_byte)
  {
    return false;
  }

  // Is the input too short to accomodate pad_byte's worth of padding bytes?
  if (input.size() <= pad_byte)
  {
    return false;
  }

  // Is the pad_byte too large for the given block_size?
  if (pad_byte >= block_size)
  {
    return false;
  }

  // Are any of the last pad_byte's worth of bytes not equal to pad_byte?
  for (std::size_t i = 0; i < pad_byte; ++i)
  {
    if (input[input.size() - 1 - i] != pad_byte)
    {
      return false;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////
auto pad(std::string_view input, std::size_t block_size = 16) noexcept -> std::string;

////////////////////////////////////////////////////////////
auto unpad(std::string_view input, std::size_t block_size = 16) -> std::string;

} // namespace hmr::pkcs7
