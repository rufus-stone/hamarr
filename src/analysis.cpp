#include "hamarr/analysis.hpp"


namespace hmr::analysis
{

static constexpr std::size_t possible_byte_values = 256;

////////////////////////////////////////////////////////////////
auto character_frequency(std::string_view input, analysis::case_sensitivity sensitivity) -> std::vector<std::size_t>
{
  auto freqs = std::vector<std::size_t>(possible_byte_values, 0); // Create a vector of 256 entries (one for each possible byte value), and set all to 0

  switch (sensitivity)
  {
    case analysis::case_sensitivity::enabled: {
      // Increment the value at the index of the current char by one
      for (auto const &ch : input)
      {
        freqs[static_cast<uint8_t>(ch)] += 1;
      }
    }
    break;

    case analysis::case_sensitivity::disabled: {
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


////////////////////////////////////////////////////////////////
void print_character_frequency(std::vector<std::size_t> freqs, bool const show_zeros)
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
        std::cout << "'" << hmr::hex::encode(static_cast<uint8_t>(i)) << "' occurs " << static_cast<int>(freqs[i]) << " times\n";
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
          std::cout << "'" << hmr::hex::encode(static_cast<uint8_t>(i)) << "' occurs " << static_cast<int>(freqs[i]) << " times\n";
        }
      }
    }
  }
}


////////////////////////////////////////////////////////////////
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
  auto best_candidate = std::min_element(std::begin(average_hams), std::end(average_hams), [](auto const &lhs, auto const &rhs)
    { return lhs.second < rhs.second; });

  if (debug_flag)
  {
    std::cout << "Best candidate key size: " << best_candidate->first << " (average Hamming distance: " << best_candidate->second << ")\n";
  }

  return *best_candidate;
}

////////////////////////////////////////////////////////////////
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
    std::cout << "Failed to find any possible keys!\n";
    std::cout << "Input was: " << input << '\n';
  }

  return possible_keys;
}

////////////////////////////////////////////////////////////////
auto repeats(std::string_view input, std::size_t const min_len, std::size_t max_len) -> std::vector<std::string_view>
{
  auto output = std::vector<std::string_view>{};

  std::size_t const data_len = input.size();

  // Is the input at least big enough to fit 2 * min_len?
  if (data_len < min_len * 2)
  {
    std::cout << "Input too short to look for sequences of " << min_len << " bytes!\n";
    return output;
  }

  // Chunk the input into windows of all possible sizes between min_len and max_len (or input.size() if max_len is bigger)
  max_len = std::min(max_len, input.size()) / 2;

  for (std::size_t win_len = min_len; win_len < max_len; ++win_len)
  {
    // Slide the window along
    for (std::size_t offset = 0; offset < data_len / 2; ++offset)
    {
      auto window = input.substr(offset, win_len);
      std::cout << "Window [" << offset << ":" << win_len << "] == " << window << '\n';
    }
  }

  return output;
}

} // namespace hmr::analysis
