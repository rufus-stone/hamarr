#include "hamarr/bitwise.hpp"

#include <algorithm>

namespace hmr::bitwise
{

static constexpr std::size_t bits_per_byte = 8;

////////////////////////////////////////////////////////////
auto xor_with_key(std::string_view input, std::string_view key) noexcept -> std::string
{
  auto output = std::string{};
  output.reserve(input.size());

  auto const len = input.size();

  for (std::size_t i = 0; i < len; ++i)
  {
    output.push_back(static_cast<uint8_t>(input[i]) ^ static_cast<uint8_t>(key[i % key.size()]));
  }

  return output;
}


////////////////////////////////////////////////////////////
auto xor_with_key(std::string_view input, uint8_t key) noexcept -> std::string
{
  auto output = std::string{};
  output.reserve(input.size());

  std::transform(std::begin(input), std::end(input), std::back_inserter(output), [&key](uint8_t const &ch)
    { return (ch ^ key); });

  return output;
}


////////////////////////////////////////////////////////////
auto xor_rolling(std::string_view input, xor_differential differential) noexcept -> std::string
{
  auto output = std::string{};
  output.reserve(input.size());

  auto const len = input.size();

  // Insert the first byte unchanged
  output.push_back(input[0]);

  // Start the loop on the second byte
  for (std::size_t i = 1; i < len; ++i)
  {
    // In input differential mode, XOR each byte with the original pre-XOR value of the previous byte
    if (differential == xor_differential::input)
    {
      output.push_back(static_cast<uint8_t>(input[i]) ^ static_cast<uint8_t>(input[i - 1]));

      // In output differential mode, XOR each byte with the resulting post-XOR value of the previous byte
    } else if (differential == xor_differential::output)
    {
      output.push_back(static_cast<uint8_t>(input[i]) ^ static_cast<uint8_t>(output[i - 1]));
    }
  }

  return output;
}


////////////////////////////////////////////////////////////
auto xor_counter(std::string_view input, uint8_t key, int increment) noexcept -> std::string
{
  static constexpr std::size_t max_key_val = 256;

  auto output = std::string{};
  output.reserve(input.size());

  for (auto const &ch : input)
  {
    uint8_t a = static_cast<uint8_t>(ch) ^ key;

    output.push_back(a);

    key = (key + increment) % max_key_val;
  }

  return output;
}


////////////////////////////////////////////////////////////
auto shift_left(std::string_view input, std::size_t amount) noexcept -> std::string
{
  auto output = std::string{};
  output.reserve(input.size());

  if (amount > bits_per_byte)
  {
    amount %= bits_per_byte;
  }

  std::transform(std::begin(input), std::end(input), std::back_inserter(output), [&amount](char const &ch)
    { return (static_cast<uint8_t>(ch) << amount); });

  return output;
}


////////////////////////////////////////////////////////////
auto shift_right(std::string_view input, std::size_t amount) noexcept -> std::string
{
  auto output = std::string{};
  output.reserve(input.size());

  if (amount > bits_per_byte)
  {
    amount %= bits_per_byte;
  }

  std::transform(std::begin(input), std::end(input), std::back_inserter(output), [&amount](char const &ch)
    { return (static_cast<uint8_t>(ch) >> amount); });

  return output;
}


////////////////////////////////////////////////////////////
auto rotate_left(std::string_view input, std::size_t amount, carry_through carry) noexcept -> std::string
{
  auto const len = input.size();

  auto output = std::string{};

  if (carry == carry_through::disabled)
  {
    if (amount > bits_per_byte)
    {
      amount %= bits_per_byte;
    }

    output.reserve(len);

    std::transform(std::begin(input), std::end(input), std::back_inserter(output), [&amount](uint8_t const ch)
      {
        auto tmp = ch;

        for (std::size_t n = 0; n < amount; ++n)
        {
          // Is there a bit that needs wrapping around?
          uint8_t carry_bit = (tmp >> 7U) & 1U;
          tmp = (tmp << 1U) | carry_bit;
        }

        return tmp;
      });

  } else if (carry == carry_through::enabled)
  {
    output = input;

    if (amount > (len * bits_per_byte))
    {
      amount %= (len * bits_per_byte);
    }

    // Loop through the input the amount of times we want to perform the rotation
    for (std::size_t n = 0; n < amount; ++n)
    {
      uint8_t previous_carry_bit = 0;

      for (auto iter = std::rbegin(output); iter < std::rend(output); ++iter)
      {
        auto tmp = static_cast<uint8_t>(*iter);

        // Check if there is a bit on the end of this byte that will need carrying forwards to the next byte
        uint8_t next_carry_bit = (tmp >> 7U) & 1U;

        // Now shift the current byte 1 space to the right, and add on any bit that was carried forward from the previous char
        tmp = (tmp << 1U) | previous_carry_bit;

        // Update previous_carry_bit ahead of the next run through
        previous_carry_bit = next_carry_bit;

        // Finally, update the current output char with the newly calculated value
        *iter = static_cast<char>(tmp);
      }

      // At the end of each run through the whole data, wrap any final carry bit back around to the start if necessary
      output[output.size() - 1] = static_cast<char>((static_cast<uint8_t>(output[output.size() - 1]) | previous_carry_bit));
    }
  }

  return output;
}


////////////////////////////////////////////////////////////
auto rotate_right(std::string_view input, std::size_t amount, carry_through carry) noexcept -> std::string
{
  auto const len = input.size();

  auto output = std::string{};

  if (carry == carry_through::disabled)
  {
    if (amount > bits_per_byte)
    {
      amount %= bits_per_byte;
    }

    output.reserve(len);

    std::transform(std::begin(input), std::end(input), std::back_inserter(output), [&amount](uint8_t const ch)
      {
        auto tmp = ch;

        for (std::size_t n = 0; n < amount; ++n)
        {
          // Is there a bit that needs wrapping around?
          uint8_t carry_bit = (tmp & 1U) << 7U;
          tmp = (tmp >> 1U) | carry_bit;
        }

        return tmp;
      });

  } else if (carry == carry_through::enabled)
  {
    output = input;

    if (amount > (len * bits_per_byte))
    {
      amount %= (len * bits_per_byte);
    }

    // Loop through the input the amount of times we want to perform the rotation
    for (std::size_t n = 0; n < amount; ++n)
    {
      uint8_t previous_carry_bit = 0;

      for (auto &ch : output)
      {
        auto tmp = static_cast<uint8_t>(ch);

        // Check if there is a bit on the end of this byte that will need carrying forwards to the next byte
        uint8_t next_carry_bit = (tmp & 1U) << 7U;

        // Now shift the current byte 1 space to the right, and add on any bit that was carried forward from the previous char
        tmp = (tmp >> 1U) | previous_carry_bit;

        // Update previous_carry_bit ahead of the next run through
        previous_carry_bit = next_carry_bit;

        // Finally, update the current output char with the newly calculated value
        ch = static_cast<char>(tmp);
      }

      // At the end of each run through the whole data, wrap any final carry bit back around to the start if necessary
      output[0] = static_cast<char>((static_cast<uint8_t>(output[0]) | previous_carry_bit));
    }
  }

  return output;
}

} // namespace hmr::bitwise
