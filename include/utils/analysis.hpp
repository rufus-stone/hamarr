#pragma once

#include <vector>

#include "logger.hpp"
#include "hex.hpp"

namespace analysis
{

enum class case_sensitivity { enabled, disabled };

////////////////////////////////////////////////////////////
std::size_t hamming_distance(const std::string &lhs, const std::string &rhs)
{
	const uint8_t *lhs_bytes = reinterpret_cast<const uint8_t*>(lhs.data());
	const uint8_t *rhs_bytes = reinterpret_cast<const uint8_t*>(rhs.data());

	std::size_t lhs_len = lhs.size();
	std::size_t rhs_len = rhs.size();

  // Hamming distance calculation expects inputs of equal length. If the inputs are NOT of equal length, ignore any excess data.
  std::size_t len = std::min(lhs_len, rhs_len);

	std::size_t ham = 0;
	uint8_t xord;

	for (std::size_t i = 0; i < len; ++i)
	{
		xord = lhs_bytes[i] ^ rhs_bytes[i];

		for(int j = 0; j < 8; ++j)
		{
			if ((xord & 0x1) == 1)
			{
				++ham;
			}
			xord >>= 1;
		}
	}

  return ham;
}

////////////////////////////////////////////////////////////
std::vector<std::size_t> character_frequency(const std::string &input, analysis::case_sensitivity sensitivity = analysis::case_sensitivity::enabled)
{
  auto freqs = std::vector<std::size_t>(256, 0); // Create a vector of 256 entries (one for each possible byte value), and set all to 0

  switch (sensitivity)
  {
    case analysis::case_sensitivity::enabled:
    {
      // Increment the value at the index of the current char by one
      for (const auto &ch : input)
      {
        freqs[static_cast<uint8_t>(ch)] += 1;
      }
    }
    break;

    case analysis::case_sensitivity::disabled:
    {
      // Normalise to all lowercase
      auto tmp = format::to_lower(input);

      // Increment the value at the index of the current char by one
      for (const auto &ch : tmp)
      {
        freqs[static_cast<uint8_t>(ch)] += 1;
      }
    }
    break;
  }

  return freqs;
}

////////////////////////////////////////////////////////////
void print_character_frequency(std::vector<std::size_t> freqs, bool show_zeros = false)
{
  for (std::size_t i = 0; i < freqs.size(); ++i)
  {
    if (show_zeros)
    {
      if (i >= 0x20 && i <= 0x7E)
      {
        LOG_INFO("'" << static_cast<char>(i) << "' occurs " << (int)freqs[i] << " times");

      } else
      {
        LOG_INFO("'" << hex::encode(static_cast<uint8_t>(i)) << "' occurs " << (int)freqs[i] << " times");
      }
      
    } else
    {
      if (freqs[i] != 0x00)
      {
        if (i >= 0x20 && i <= 0x7E)
        {
          LOG_INFO("'" << static_cast<char>(i) << "' occurs " << (int)freqs[i] << " times");

        } else
        {
          LOG_INFO("'" << hex::encode(static_cast<uint8_t>(i)) << "' occurs " << (int)freqs[i] << " times");
        }
      }
    }
  }
}

} // namespace analysis
