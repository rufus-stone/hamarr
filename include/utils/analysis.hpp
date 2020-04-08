#pragma once

#include <vector>
#include <string>
#include <numeric>

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

////////////////////////////////////////////////////////////
bool looks_like_english(const std::string &input)
{
  // There should be more alphanumeric chars than punctuation chars
  std::size_t alphanumerics = 0;
  std::size_t punctuation = 0;

  // There should be at least some space chars
  bool spaces = false;

  // Compute the case-insensitive character frequencies
  auto freqs = analysis::character_frequency(input, analysis::case_sensitivity::disabled);

  // Todo - maybe change the output of analysis::character_frequency() to a std::array to ensure it's always exactly 256 elements in size, and can't be modified
  assert(freqs.size() == 256);

  // Iterate through the freqs vector and run some checks
  for (std::size_t i = 0; i < freqs.size(); ++i)
  {
    auto ch = static_cast<uint8_t>(i);

    // Were there any of the current char present?
    if (freqs[i] > 0)
    {
      // Abort condition - there should be no un-printable byte values (apart from space, tab, newline, carriage return, etc.)
      if ((ch < 0x20 && ch != 0x09 && ch != 0x0A && ch != 0x0B && ch != 0x0C && ch != 0x0D) || ch > 0x7E)
      {
        return false;
      }

      // Are there any space chars?
      if (ch == 0x20)
      {
        spaces = true;
      }

      // Update the count of punctuation chars vs alphanumeric chars
      if ((ch >= 0x21 && ch <= 0x2F) || (ch >= 0x3A && ch <= 0x40) || (ch >= 0x5B && ch <= 0x60) || (ch >= 0x7B && ch <= 0x7E))
      {
        ++punctuation;

      } else
      {
        ++alphanumerics;
      }
    }
  }

  // The average word length in English is apparently 4.7 - we'll accept between 3.5 and 6.5 (this is an arbitary choice, and might need revising!)
  auto word_lengths = std::vector<std::size_t>{};
  std::size_t current_word_length = 0;
  double average_word_length = 0;

  // Iterate through the raw input and run some more checks
  for (const auto &c : input)
  {
    auto ch = static_cast<uint8_t>(c);

    // Is it an uppercase or lowercase alphabet char, or an apostraphe? If so, increment the length of the current word. Todo: this will need refining, so that, e.g. an input string of all apostraphes doesn't get badged as English-like!
    if ((ch >= 0x41 && ch <= 0x5A) || (ch >= 0x61 && ch <= 0x7A) || ch == 0x27)
    {
      ++current_word_length;

    // Otherwise the word must be over
    } else
    {
      // If appropriate, update the vector of all word_lengths with the length of this word
      if (current_word_length > 0)
      {
        word_lengths.push_back(current_word_length);
      }
      
      // Reset the current word length
      current_word_length = 0;
    }
  }

  // Add any final word length
  if (current_word_length > 0)
  {
    word_lengths.push_back(current_word_length);
    current_word_length = 0;
  }

  // Abort condition - there should be more alphanumerics than punctuation chars
  if (punctuation > alphanumerics)
  {
    return false;
  }

  // Abort condition - there should be at least some space chars
  if (spaces == false)
  {
    return false;
  }

  // Compute the mean average word length
  average_word_length = std::accumulate(std::begin(word_lengths), std::end(word_lengths), 0.0) / word_lengths.size();

  // Abort condition - the average word length should be between 3.5 and 6.5 chars (the real average for English is apparently 4.7)
  if (average_word_length <= 3.5 || average_word_length >= 6.5)
  {
    return false;
  }

  return true;
}

} // namespace analysis
