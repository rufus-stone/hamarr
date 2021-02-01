#pragma once

#include <string>
#include <string_view>


namespace hmr::url
{

using namespace std::string_view_literals;

constexpr auto unreserved_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~"sv;

////////////////////////////////////////////////////////////
auto encode(std::string_view input, bool lazy = false) noexcept -> std::string;

////////////////////////////////////////////////////////////
auto decode(std::string_view input, bool lazy = false) -> std::string;

} // namespace hmr::url
