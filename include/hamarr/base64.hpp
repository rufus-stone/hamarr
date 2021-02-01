#pragma once

#include <array>
#include <string>
#include <string_view>

namespace hmr::base64
{

using namespace std::string_view_literals;

constexpr auto base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="sv;

////////////////////////////////////////////////////////////
constexpr auto is_invalid_alphabet = [](std::string_view input) noexcept -> bool
{
  auto counts = std::array<std::size_t, 265>{};

  for (auto const &ch : input)
  {
    if (counts[static_cast<std::size_t>(ch)] > 1)
    {
      return true;
    }

    counts[static_cast<std::size_t>(ch)]++;
  }

  return false;
};

////////////////////////////////////////////////////////////
auto encode(std::string_view input, std::string_view alphabet = base64_alphabet) -> std::string;

////////////////////////////////////////////////////////////
auto decode(std::string_view input, std::string_view alphabet = base64_alphabet) -> std::string;

} // namespace hmr::base64
