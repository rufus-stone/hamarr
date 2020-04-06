#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <bitset>

#include "format.hpp"
#include "logger.hpp"
#include "hex.hpp"

namespace binary
{

////////////////////////////////////////////////////////////
std::string encode(const std::string &input, bool delimited = true)
{
  std::vector<std::string> out_vec;

  std::transform(input.begin(), input.end(), std::back_inserter(out_vec), [](const unsigned char c)
  {
    const auto bits = std::bitset<8>(c);
    return bits.to_string();
  });

  return std::accumulate(out_vec.begin(), out_vec.end(), std::string(), [&delimited](auto lhs, auto rhs) -> std::string
  {
    if (lhs.empty())
    {
      return rhs; // This avoids inserting a blank space before the very first byte
    } else
    {
      return delimited ? lhs + " " + rhs : lhs + rhs;
    }
  });
}


////////////////////////////////////////////////////////////
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
std::string encode(T input, bool delimited = true)
{
  if (delimited)
  {
    // How many bytes is the integral value?
    const std::size_t s = sizeof(input);

    std::string output;
    output.reserve(s + (s-1));

    // There's probably a smarter way to do this, but for now just manually code in options for 1, 2, 4 and 8 byte integrals
    switch (s)
    {
      case 1:
      {
        auto bits = std::bitset<8>(input);
        return bits.to_string();
      }
      case 2:
      {
        uint8_t a = (input & 0xFF00) >> 8;
        uint8_t b = (input & 0x00FF);

        auto bits = std::bitset<8>(a);
        output += bits.to_string();

        output.push_back(' ');

        bits = std::bitset<8>(b);
        output += bits.to_string();
        return output;
      }
      case 4:
      {
        uint8_t a = (input & 0xFF000000) >> 24;
        uint8_t b = (input & 0x00FF0000) >> 16;
        uint8_t c = (input & 0x0000FF00) >> 8;
        uint8_t d = (input & 0x000000FF);

        auto bits = std::bitset<8>(a);
        output += bits.to_string();
        
        output.push_back(' ');

        bits = std::bitset<8>(b);
        output += bits.to_string();

        output.push_back(' ');

        bits = std::bitset<8>(c);
        output += bits.to_string();
        
        output.push_back(' ');

        bits = std::bitset<8>(d);
        output += bits.to_string();

        return output;
      }
      case 8:
      {
        uint8_t a = (input & 0xFF00000000000000) >> 56;
        uint8_t b = (input & 0x00FF000000000000) >> 48;
        uint8_t c = (input & 0x0000FF0000000000) >> 40;
        uint8_t d = (input & 0x000000FF00000000) >> 32;
        uint8_t e = (input & 0x00000000FF000000) >> 24;
        uint8_t f = (input & 0x0000000000FF0000) >> 16;
        uint8_t g = (input & 0x000000000000FF00) >> 8;
        uint8_t h = (input & 0x00000000000000FF);

        auto bits = std::bitset<8>(a);
        output += bits.to_string();
        
        output.push_back(' ');

        bits = std::bitset<8>(b);
        output += bits.to_string();

        output.push_back(' ');

        bits = std::bitset<8>(c);
        output += bits.to_string();
        
        output.push_back(' ');

        bits = std::bitset<8>(d);
        output += bits.to_string();

        output.push_back(' ');

        bits = std::bitset<8>(e);
        output += bits.to_string();
        
        output.push_back(' ');

        bits = std::bitset<8>(f);
        output += bits.to_string();

        output.push_back(' ');

        bits = std::bitset<8>(g);
        output += bits.to_string();
        
        output.push_back(' ');

        bits = std::bitset<8>(h);
        output += bits.to_string();
        
        return output;
      }
      default:
      {
        return std::string{};
      }
    }

  } else
  {
    auto b = std::bitset<sizeof(T) * 8>(input);

    return b.to_string();
  }
}


////////////////////////////////////////////////////////////
std::string decode(const std::string &input)
{
  // Strip any spaces
  std::string tmp = input;
  tmp.erase(std::remove(std::begin(tmp), std::end(tmp), ' '), std::end(tmp));

  std::string output;

  if (tmp.size() % 8 != 0)
  {
    LOG_ERROR("Not divisible by 8!");
    return std::string{};
  }

  for (std::size_t i = 0; i < tmp.size(); i += 8)
  {
    const auto bits = std::bitset<8>(tmp.substr(i, i + 8));
    output.push_back(bits.to_ulong());
  }

  return output;
}

////////////////////////////////////////////////////////////
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
T decode(const std::string &input)
{
  // Strip any spaces
  std::string tmp = input;
  tmp.erase(std::remove(std::begin(tmp), std::end(tmp), ' '), std::end(tmp));

  // Input binary string must be divisible by 8
  if (tmp.size() % 8 != 0)
  {
    LOG_ERROR("Not divisible by 8!");
    return T{};
  }

  // Input binary string must contain the correct number of bits to fit into the desired return type
  // TODO: For inputs that are shorter than the desired return type, maybe we could just convert to that type anyway, e.g. an input of "11111111" could be turned into a uint16_t output with the value 0x00FF ??
  if ((tmp.size() / 8) != sizeof(T))
  {
    LOG_ERROR("Input binary string does not contain the correct number of bits to fit into a " << sizeof(T) << " byte type!");
    return T{};
  }

  const auto bits = std::bitset<sizeof(T) * 8>(tmp);

  const auto output = static_cast<T>(bits.to_ullong());

  return output;
}

} // namespace binary
