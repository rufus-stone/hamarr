#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <iostream>

#include "hex.hpp"
#include "bitwise.hpp"

namespace hmr::analysis
{

enum class case_sensitivity { enabled, disabled };

////////////////////////////////////////////////////////////
constexpr std::size_t hamming_distance(std::string_view lhs, std::string_view rhs) noexcept
{
  auto lhs_bytes = lhs.data(); //const uint8_t *lhs_bytes = reinterpret_cast<const uint8_t*>(lhs.data());
  auto rhs_bytes = rhs.data(); //const uint8_t *rhs_bytes = reinterpret_cast<const uint8_t*>(rhs.data());

  std::size_t lhs_len = lhs.size();
  std::size_t rhs_len = rhs.size();

  // Hamming distance calculation expects inputs of equal length. If the inputs are NOT of equal length, ignore any excess data.
  std::size_t len = std::min(lhs_len, rhs_len);

  std::size_t ham = 0;
  uint8_t xord = 0;

  for (std::size_t i = 0; i < len; ++i)
  {
    xord = lhs_bytes[i] ^ rhs_bytes[i];

    for (int j = 0; j < 8; ++j)
    {
      if ((xord & 0x01) == 1)
      {
        ++ham;
      }
      xord >>= 1;
    }
  }

  return ham;
}

////////////////////////////////////////////////////////////
constexpr double entropy(std::string_view input)
{
  const std::size_t len = input.size();

  // I'm not sure if std::array default initalizes ints to 0, so doing that here just in case
  auto char_freqs = std::array<std::size_t, 256>{
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  for (const auto &ch : input)
  {
    char_freqs[static_cast<uint8_t>(ch)]++;
  }

  double entropy = 0;

  for (const auto &freq : char_freqs)
  {
    if (freq != 0)
    {
      double f = static_cast<double>(freq) / len;
      entropy -= (f * std::log2(f));
    }
  }

  return entropy;
}

////////////////////////////////////////////////////////////
std::vector<std::size_t> character_frequency(std::string_view input, analysis::case_sensitivity sensitivity = analysis::case_sensitivity::enabled)
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
      // Increment the value at the index of the current char by one - normalise to lower case first
      for (const auto &ch : input)
      {
        freqs[static_cast<uint8_t>(std::tolower(ch))] += 1;
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
        std::cout << "'" << static_cast<char>(i) << "' occurs " << static_cast<int>(freqs[i]) << " times\n";

      } else
      {
        std::cout << "'" << hex::encode(static_cast<uint8_t>(i)) << "' occurs " << static_cast<int>(freqs[i]) << " times\n";
      }

    } else
    {
      if (freqs[i] != 0x00)
      {
        if (i >= 0x20 && i <= 0x7E)
        {
          std::cout << "'" << static_cast<char>(i) << "' occurs " << static_cast<int>(freqs[i]) << " times\n";

        } else
        {
          std::cout << "'" << hex::encode(static_cast<uint8_t>(i)) << "' occurs " << static_cast<int>(freqs[i]) << " times\n";
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////
constexpr bool looks_like_english(std::string_view input, bool debug_flag = false)
{
  std::size_t spaces = 0;
  std::size_t punctuation = 0;
  std::size_t numbers = 0;
  std::size_t lowercase = 0;
  std::size_t uppercase = 0;

  for (const auto &c : input)
  {
    auto ch = static_cast<uint8_t>(c);

    // Abort condition - there should be no un-printable byte values (apart from space, tab, newline, carriage return, etc.)
    if ((ch < 0x20 && ch != 0x09 && ch != 0x0A && ch != 0x0B && ch != 0x0C && ch != 0x0D) || ch > 0x7E)
    {
      if (debug_flag) std::cout << "Failed unprintable\n";
      return false;
    }

    // Is it a space char?
    if (ch == 0x20)
    {
      ++spaces;
    }

    // Is it a punctuation char?
    if ((ch >= 0x21 && ch <= 0x2F) || (ch >= 0x3A && ch <= 0x40) || (ch >= 0x5B && ch <= 0x60) || (ch >= 0x7B && ch <= 0x7E))
    {
      ++punctuation;
    }

    // Is it a number?
    if (ch >= 0x30 && ch <= 0x39)
    {
      ++numbers;
    }

    // Is it a lowercase letter?
    if (ch >= 0x61 && ch <= 0x7A)
    {
      ++lowercase;
    }

    // Is it an uppercase letter?
    if (ch >= 0x41 && ch <= 0x5A)
    {
      ++uppercase;
    }
  }

  // There should be more spaces than punctuation
  if (spaces < punctuation)
  {
    if (debug_flag) std::cout << "Failed spaces vs punc\n";
    return false;
  }

  // There should be more alphanumerics than punctuation
  if (numbers + lowercase + uppercase < punctuation)
  {
    if (debug_flag) std::cout << "Failed alphanum vs punc\n";
    return false;
  }

  // There should be more lowercase letters than uppercase
  if (lowercase < uppercase)
  {
    if (debug_flag) std::cout << "Failed lower vs upper\n";
    return false;
  }

  // There should be more letters than numbers
  if (lowercase + uppercase < numbers)
  {
    if (debug_flag) std::cout << "Failed letters vs numbers\n";
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////
auto find_candidate_keysize(std::string_view input, std::size_t min = 2, std::size_t max = 40, bool debug_flag = false)
{
  const std::size_t len = input.size();

  const std::size_t min_key_size = min;
  const std::size_t max_key_size = max;

  auto average_hams = std::vector<std::pair<std::size_t, double>>{};
  average_hams.reserve(max_key_size - min_key_size);

  // For all possible key_sizes between min and max, figure out the most likely key_size
  for (std::size_t key_size = min_key_size; key_size <= max_key_size; ++key_size)
  {
    // How many complete pairs of key_size worth of bytes is there room for?
    auto num_pairs = len / (key_size * 2);

    auto hams = std::vector<double>{};
    hams.reserve(num_pairs);

    // Take each pair of key_size worth of bytes
    for (std::size_t i = 0; i + (key_size * 2) < len; i += (key_size * 2))
    {
      auto lhs = input.substr(i, key_size);
      auto rhs = input.substr(i + key_size, key_size);

      // Compute the hamming distance, and normalise by dividing by key_size
      double ham_norm = hmr::analysis::hamming_distance(lhs, rhs);
      ham_norm /= key_size;

      hams.push_back(ham_norm);
    }

    // Now average the hamming distances
    auto average_ham = std::accumulate(std::begin(hams), std::end(hams), 0.0) / hams.size();

    // Make a note of the hamming distance for the current key_size
    average_hams.emplace_back(std::make_pair(key_size, average_ham));
  }

  // Pick the key_size with the lowest average hamming distance - this is the best candidate for the actual key size
  auto best_candidate = std::min_element(std::begin(average_hams), std::end(average_hams), [](const auto &lhs, const auto &rhs) { return lhs.second < rhs.second; });
  if (debug_flag) std::cout << "Best candidate key size: " << best_candidate->first << " (average Hamming distance: " << best_candidate->second << ")\n";

  return *best_candidate;
}

////////////////////////////////////////////////////////////
auto solve_single_byte_xor(std::string_view input, bool debug_flag = false)
{
  uint8_t key = 0x00;

  auto possible_keys = std::vector<uint8_t>{};

  // Iterate through all possible single byte keys
  for (std::size_t n = 0; n <= std::numeric_limits<uint8_t>::max(); ++n)
  {
    auto result = hmr::bitwise::xor_with_key(input, key);

    if (hmr::analysis::looks_like_english(result, debug_flag))
    {
      possible_keys.push_back(key);
    }

    key++;
  }

  if (possible_keys.empty() && debug_flag)
  {
    std::cout << "Failed to find any possible keys!\n";
    std::cout << "Input was: " << input << '\n';
  }

  return possible_keys;
}

////////////////////////////////////////////////////////////
constexpr bool repeated_blocks(std::string_view input, std::size_t block_size = 16)
{
  auto len = input.size();
  assert(len % block_size == 0);

  // Ignore the final block, as we'll already know that isn't repeated by then
  for (std::size_t offset = 0; offset + block_size <= (len - block_size); offset += block_size)
  {
    // Get the next block
    auto block = input.substr(offset, block_size);

    // Does this block occur again in the data
    auto pos = input.find(block, offset + block_size);
    if (pos != std::string_view::npos)
    {
      return true;
    }
  }

  return false;
}

} // namespace hmr::analysis
