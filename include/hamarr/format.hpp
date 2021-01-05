#pragma once

#include <string>
#include <string_view>
#include <vector>


namespace hmr::fmt
{

using namespace std::string_view_literals;

auto to_upper(std::string_view input) -> std::string;
auto to_lower(std::string_view input) -> std::string;

auto escape(std::string_view input) -> std::string;
auto unescape(std::string_view input) -> std::string;

auto split(std::string_view input, char delimiter, bool collapse_adjacent_delimiters = true, bool ignore_leading_delimiter = true) -> std::vector<std::string>;
auto split(std::string_view input, std::string_view delimiters, bool collapse_adjacent_delimiters = true, bool ignore_leading_delimiter = true) -> std::vector<std::string>;

auto lstrip(std::string_view input, std::string_view any_of_these = " \n\r\t\f\v"sv) -> std::string;
auto rstrip(std::string_view input, std::string_view any_of_these = " \n\r\t\f\v"sv) -> std::string;
auto strip(std::string_view input, std::string_view any_of_these = " \n\r\t\f\v"sv) -> std::string;

} // namespace hmr::fmt
