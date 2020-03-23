#pragma once

#include <string>
#include <algorithm>

#include "logger.hpp"
#include "hex.hpp"

namespace bitwise
{

enum class xor_differential { input, output };

////////////////////////////////////////////////////////////
std::string xor_with_key(const std::string &input, const std::string &key)
{
	std::string output;
  output.reserve(input.size());

	const std::size_t len = input.size();

	for (std::size_t i = 0; i < len; ++i)
	{
		output.push_back(input[i] ^ key[i % key.size()]);
	}

	return output;
}

////////////////////////////////////////////////////////////
std::string xor_rolling(const std::string &input, xor_differential differential = xor_differential::input)
{
  std::string output;
  output.reserve(input.size());

	const std::size_t len = input.size();

  // Insert the first byte unchanged
  output.push_back(input[0]);

  // Start the loop on the second byte
  for (std::size_t i = 1; i < len; ++i)
	{
    // In input differential mode, XOR each byte with the original pre-XOR value of the previous byte
    if (differential == xor_differential::input)
    {
      output.push_back(input[i] ^ input[i-1]);

    // In output differential mode, XOR each byte with the resulting post-XOR value of the previous byte
    } else if (differential == xor_differential::output)
    {
      output.push_back(input[i] ^ output[i-1]);
    }
		
	}

  return output;
}


////////////////////////////////////////////////////////////
std::string xor_counter(const std::string &input, uint8_t key = 0x00, int increment = 1)
{

  std::string output;
  output.reserve(input.size());  

  for (const auto &ch : input)
  {
    uint8_t a = (uint8_t)(ch ^ key);

    output.push_back(char(a));

    key = (key + increment) % 256;
  }

  return output;
}

} // namespace bitwise