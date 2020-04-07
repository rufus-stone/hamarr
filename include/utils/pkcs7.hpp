#pragma once

#include <string>

namespace pkcs7
{

////////////////////////////////////////////////////////////
bool padded(const std::string &input, std::size_t block_size = 16)
{
  uint8_t pad_byte = input[input.size() - 1];

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
std::string pad(const std::string &input, std::size_t block_size = 16)
{
  uint8_t pad_byte = input.size() % block_size;

  if (pad_byte == 0)
  {
    return input;
  }

  pad_byte = block_size - pad_byte;

  std::string output;
  output.reserve(input.size() + pad_byte);

  output = input;

  for (std::size_t n = 0; n < pad_byte; ++n)
  {
    output.push_back(pad_byte);
  }
  
  return output;
}

////////////////////////////////////////////////////////////
std::string unpad(const std::string &input, std::size_t block_size = 16)
{
  // Is the input not actually pkcs7 padded?
  if (!pkcs7::padded(input, block_size))
  {
    return input;
  }

  uint8_t pad_byte = input[input.size() - 1];

  std::string output;
  output.reserve(input.size() - pad_byte);

  output = input.substr(0, input.size() - pad_byte);
  
  return output;
}

} // namespace pad
