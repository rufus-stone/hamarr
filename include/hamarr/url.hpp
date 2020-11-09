#pragma once

#include <string>
#include <string_view>


namespace hmr::url
{

using namespace std::string_view_literals;

constexpr auto unreserved_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~"sv;

////////////////////////////////////////////////////////////
std::string encode(std::string_view input, bool lazy = false) noexcept;

////////////////////////////////////////////////////////////
std::string decode(std::string_view input, bool lazy = false);

} // namespace hmr::url
