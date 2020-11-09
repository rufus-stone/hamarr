#include "hamarr/pkcs7.hpp"


namespace hmr::pkcs7
{

////////////////////////////////////////////////////////////
std::string pad(std::string_view input, std::size_t block_size) noexcept
{
  uint8_t pad_byte = input.size() % block_size;

  if (pad_byte == 0)
  {
    return std::string{input.data()};
  }

  pad_byte = block_size - pad_byte;

  std::string output;
  output.reserve(input.size() + pad_byte);

  output = input;

  for (std::size_t n = 0; n < pad_byte; ++n)
  {
    output.push_back(static_cast<char>(pad_byte));
  }

  return output;
}

////////////////////////////////////////////////////////////
std::string unpad(std::string_view input, std::size_t block_size)
{
  // Is the input not actually pkcs7 padded?
  if (!pkcs7::padded(input, block_size))
  {
    return std::string{input.data()};
  }

  uint8_t pad_byte = static_cast<uint8_t>(input[input.size() - 1]);

  std::string output;
  output.reserve(input.size() - pad_byte);

  output = input.substr(0, input.size() - pad_byte);

  return output;
}

} // namespace hmr::pkcs7
