#include "hamarr/analysis.hpp"


namespace hmr::analysis
{

////////////////////////////////////////////////////////////
std::vector<std::size_t> character_frequency(std::string_view input, analysis::case_sensitivity sensitivity)
{
  auto freqs = std::vector<std::size_t>(256, 0); // Create a vector of 256 entries (one for each possible byte value), and set all to 0

  switch (sensitivity)
  {
    case analysis::case_sensitivity::enabled:
    {
      // Increment the value at the index of the current char by one
      for (auto const &ch : input)
      {
        freqs[static_cast<uint8_t>(ch)] += 1;
      }
    }
    break;

    case analysis::case_sensitivity::disabled:
    {
      // Increment the value at the index of the current char by one - normalise to lower case first
      for (auto const &ch : input)
      {
        freqs[static_cast<uint8_t>(std::tolower(ch))] += 1;
      }
    }
    break;
  }

  return freqs;
}


////////////////////////////////////////////////////////////
void print_character_frequency(std::vector<std::size_t> freqs, bool const show_zeros)
{
  for (std::size_t i = 0; i < freqs.size(); ++i)
  {
    if (show_zeros)
    {
      if (i >= 0x20 && i <= 0x7E)
      {
        spdlog::info("'{}' occurs {} times", static_cast<char>(i), static_cast<int>(freqs[i]));

      } else
      {
        spdlog::info("'{}' occurs {} times", hmr::hex::encode(static_cast<uint8_t>(i)), static_cast<int>(freqs[i]));
      }

    } else
    {
      if (freqs[i] != 0x00)
      {
        if (i >= 0x20 && i <= 0x7E)
        {
          spdlog::info("'{}' occurs {} times", static_cast<char>(i), static_cast<int>(freqs[i]));

        } else
        {
          spdlog::info("'{}' occurs {} times", hmr::hex::encode(static_cast<uint8_t>(i)), static_cast<int>(freqs[i]));
        }
      }
    }
  }
}


////////////////////////////////////////////////////////////
auto find_candidate_keysize(std::string_view input, std::size_t const min, std::size_t const max, bool const debug_flag) -> std::pair<std::size_t, double>
{
  auto const len = input.size();

  auto const min_key_size = min;
  auto const max_key_size = max;

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
  auto best_candidate = std::min_element(std::begin(average_hams), std::end(average_hams), [](auto const &lhs, auto const &rhs) { return lhs.second < rhs.second; });
  if (debug_flag) spdlog::info("Best candidate key size: {} (average Hamming distance: {})", best_candidate->first, best_candidate->second);

  return *best_candidate;
}

////////////////////////////////////////////////////////////
auto solve_single_byte_xor(std::string_view input, bool const debug_flag) -> std::vector<uint8_t>
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
    spdlog::warn("Failed to find any possible keys!");
    spdlog::warn("Input was: {}", input);
  }

  return possible_keys;
}

} // namespace hmr::analysis
