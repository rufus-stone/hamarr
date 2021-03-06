#pragma once

#include <array>
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

enum class case_sensitivity { enabled,
  disabled };

////////////////////////////////////////////////////////////////
constexpr auto hamming_distance(std::string_view lhs, std::string_view rhs) noexcept -> std::size_t
{
  char const *lhs_bytes = lhs.data(); //const uint8_t *lhs_bytes = reinterpret_cast<const uint8_t*>(lhs.data());
  char const *rhs_bytes = rhs.data(); //const uint8_t *rhs_bytes = reinterpret_cast<const uint8_t*>(rhs.data());

  std::size_t const lhs_len = lhs.size();
  std::size_t const rhs_len = rhs.size();

  // Hamming distance calculation expects inputs of equal length. If the inputs are NOT of equal length, ignore any excess data.
  std::size_t const len = std::min(lhs_len, rhs_len);

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

////////////////////////////////////////////////////////////////
constexpr auto entropy(std::string_view input) -> double
{
  std::size_t const len = input.size();

  // I'm not sure if std::array default initalizes ints to 0, so doing that here just in case
  // clang-format off
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
  // clang-format on

  for (auto const &ch : input)
  {
    char_freqs[static_cast<uint8_t>(ch)]++;
  }

  double entropy = 0;

  for (auto const &freq : char_freqs)
  {
    if (freq != 0)
    {
      double f = static_cast<double>(freq) / len;
      entropy -= (f * std::log2(f));
    }
  }

  return entropy;
}

////////////////////////////////////////////////////////////////
auto character_frequency(std::string_view input, analysis::case_sensitivity sensitivity = analysis::case_sensitivity::enabled) -> std::vector<std::size_t>;

////////////////////////////////////////////////////////////////
void print_character_frequency(std::vector<std::size_t> freqs, bool const show_zeros = false);

////////////////////////////////////////////////////////////////
constexpr auto looks_like_english(std::string_view input, bool const debug_flag = false) -> bool
{
  std::size_t spaces = 0;
  std::size_t punctuation = 0;
  std::size_t numbers = 0;
  std::size_t lowercase = 0;
  std::size_t uppercase = 0;

  for (auto const &c : input)
  {
    auto ch = static_cast<uint8_t>(c);

    // Abort condition - there should be no un-printable byte values (apart from space, tab, newline, carriage return, etc.)
    if ((ch < 0x20 && ch != 0x09 && ch != 0x0A && ch != 0x0B && ch != 0x0C && ch != 0x0D) || ch > 0x7E)
    {
      if (debug_flag) std::cout << "Found unprintable char: " << hmr::hex::encode(ch) << '\n';
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
    if (debug_flag) std::cout << "Failed spaces vs punc check: " << input << '\n';
    return false;
  }

  // There should be more alphanumerics than punctuation
  if (numbers + lowercase + uppercase < punctuation)
  {
    if (debug_flag) std::cout << "Failed alphanum vs punc check: " << input << '\n';
    return false;
  }

  // There should be more lowercase letters than uppercase
  if (lowercase < uppercase)
  {
    if (debug_flag) std::cout << "Failed lower vs upper check: " << input << '\n';
    return false;
  }

  // There should be more letters than numbers
  if (lowercase + uppercase < numbers)
  {
    if (debug_flag) std::cout << "Failed letters vs numbers check: " << input << '\n';
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////
auto find_candidate_keysize(std::string_view input, std::size_t const min = 2, std::size_t const max = 40, bool const debug_flag = false) -> std::pair<std::size_t, double>;

////////////////////////////////////////////////////////////////
auto solve_single_byte_xor(std::string_view input, bool const debug_flag = false) -> std::vector<uint8_t>;

////////////////////////////////////////////////////////////////
constexpr auto repeated_blocks(std::string_view input, std::size_t const block_size = 16) -> bool
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

////////////////////////////////////////////////////////////////
auto repeats(std::string_view input, std::size_t const min_len = 4, std::size_t max_len = 16) -> std::vector<std::string_view>;

} // namespace hmr::analysis
