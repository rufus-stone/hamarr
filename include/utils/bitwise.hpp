#pragma once

#include <string>
#include <algorithm>

namespace bitwise
{

enum class xor_differential { input, output };

enum class carry_through { enabled, disabled };

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


////////////////////////////////////////////////////////////
std::string shift_left(const std::string &input, std::size_t amount = 1)
{
  std::string output;
  output.reserve(input.size());

  if (amount > 8)
  {
    amount %= 8;
  }

  std::transform(std::begin(input), std::end(input), std::back_inserter(output), [&amount](const char &ch)
  {
    return (ch << amount);
  });

  return output;
}


////////////////////////////////////////////////////////////
template <typename T, typename = std::enable_if<std::is_integral<T>::value>>
T shift_left(T input, std::size_t amount = 1)
{
  if (amount > (sizeof(T) * 8))
  {
    amount %= (sizeof(T) * 8);
  }

  return (input << amount);
}


////////////////////////////////////////////////////////////
std::string shift_right(const std::string &input, std::size_t amount = 1)
{
  std::string output;
  output.reserve(input.size());

  if (amount > 8)
  {
    amount %= 8;
  }

  std::transform(std::begin(input), std::end(input), std::back_inserter(output), [&amount](const char &ch)
  {
    return (ch >> amount);
  });

  return output;
}


////////////////////////////////////////////////////////////
template <typename T, typename = std::enable_if<std::is_integral<T>::value>>
T shift_right(T input, std::size_t amount = 1)
{
  if (amount > (sizeof(T) * 8))
  {
    amount %= (sizeof(T) * 8);
  }

  return (input >> amount);
}


////////////////////////////////////////////////////////////
std::string rotate_left(const std::string &input, std::size_t amount = 1, carry_through carry = carry_through::disabled)
{
  const auto len = input.size();

  std::string output;

  if (carry == carry_through::disabled)
  {
    if (amount > 8)
    {
      amount %= 8;
    }

    output.reserve(len);

    std::transform(std::begin(input), std::end(input), std::back_inserter(output), [&amount](const uint8_t ch)
    {
      auto tmp = ch;

      for (std::size_t n = 0; n < amount; ++n)
      {
        // Is there a bit that needs wrapping around?
        uint8_t carry_bit = (tmp >> 7) & 1;
        tmp = (tmp << 1) | carry_bit;
      }

      return tmp;
    });

    return output;

  } else if (carry == carry_through::enabled)
  {
    output = input;

    if (amount > (len * 8))
    {
      amount %= (len * 8);
    }

    // Loop through the input the amount of times we want to perform the rotation
    for (std::size_t n = 0; n < amount; ++n)
    {
      uint8_t previous_carry_bit = 0;

      for (auto iter = std::rbegin(output); iter < std::rend(output); ++iter)
      {
        uint8_t tmp = *iter;
        
        // Check if there is a bit on the end of this byte that will need carrying forwards to the next byte
        uint8_t next_carry_bit = (tmp >> 7) & 1;

        // Now shift the current byte 1 space to the right, and add on any bit that was carried forward from the previous char
        tmp = (tmp << 1) | previous_carry_bit;

        // Update previous_carry_bit ahead of the next run through
        previous_carry_bit = next_carry_bit;

        // Finally, update the current output char with the newly calculated value
        *iter = tmp;
      }

      // At the end of each run through the whole data, wrap any final carry bit back around to the start if necessary
      output[output.size()-1] = (static_cast<uint8_t>(output[output.size()-1]) | previous_carry_bit);
    }
  }
  
  return output;
}


////////////////////////////////////////////////////////////
std::string rotate_right(const std::string &input, std::size_t amount = 1, carry_through carry = carry_through::disabled)
{
  const auto len = input.size();

  std::string output;

  if (carry == carry_through::disabled)
  {
    if (amount > 8)
    {
      amount %= 8;
    }

    output.reserve(len);

    std::transform(std::begin(input), std::end(input), std::back_inserter(output), [&amount](const uint8_t ch)
    {
      auto tmp = ch;

      for (std::size_t n = 0; n < amount; ++n)
      {
        // Is there a bit that needs wrapping around?
        uint8_t carry_bit = (tmp & 1) << 7;
        tmp = (tmp >> 1) | carry_bit;
      }

      return tmp;
    });

    return output;

  } else if (carry == carry_through::enabled)
  {
    output = input;

    if (amount > (len * 8))
    {
      amount %= (len * 8);
    }

    // Loop through the input the amount of times we want to perform the rotation
    for (std::size_t n = 0; n < amount; ++n)
    {
      uint8_t previous_carry_bit = 0;

      for (auto &ch : output)
      {
        uint8_t tmp = ch;
        
        // Check if there is a bit on the end of this byte that will need carrying forwards to the next byte
        uint8_t next_carry_bit = (tmp & 1U) << 7U;

        // Now shift the current byte 1 space to the right, and add on any bit that was carried forward from the previous char
        tmp = (tmp >> 1U) | previous_carry_bit;

        // Update previous_carry_bit ahead of the next run through
        previous_carry_bit = next_carry_bit;

        // Finally, update the current output char with the newly calculated value
        ch = tmp;
      }

      // At the end of each run through the whole data, wrap any final carry bit back around to the start if necessary
      output[0] = (static_cast<uint8_t>(output[0]) | previous_carry_bit);
    }
  }
  
  return output;
}

} // namespace bitwise
