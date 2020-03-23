#pragma once

#include <string>
#include <algorithm>

#include "logger.hpp"
#include "hex.hpp"


namespace url
{

static const auto unreserved_chars = std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~");

////////////////////////////////////////////////////////////
std::string encode(const std::string& input, bool lazy=false)
{
	std::string output;
	output.reserve(input.size()); // The output will almost certainly be larger than this, however...

	for (auto &c : input)
	{
		// Is it an unreserved char? If so, append unchanged
		if (unreserved_chars.find(c) != std::string::npos)
		{
			output.push_back(c);
			continue;
		}

		// If we get to here, then it must be a reserved char
		if (lazy == true) // If we're being lazy, just convert to hex and append
		{
			output += ("%" + hex::encode(c));
      
		} else // If not lazy, convert to UTF8 first and then append
		{
			if (uint8_t(c) < 0x80) // Char values less than 0x80 remain unchanged
			{
				output += ("%" + hex::encode(c));

			} else if (uint8_t(c) >= 0x80 && uint8_t(c) < 0xC0)
			{
				output += ("%C2%" + hex::encode(c));

			} else if (uint8_t(c) >= 0xC0)
			{
				output += ("%C3%" + hex::encode(uint8_t(c | 40))); // Casting to uint8_t is necessary because, even though c is a char, c | 40 produces an int, which is not just a single byte, so the hex::encode() function messes it up

			} else // This shouldn't happen
			{
				return std::string{};
			}
		}
	}

	return output;
}

////////////////////////////////////////////////////////////
std::string decode(const std::string& input, bool lazy=false)
{
	std::string output;

	std::size_t len = input.length();
	for (std::size_t i = 0; i < len; ++i)
	{
		// Is this an escape sequence?
		if (input[i] == '%')
		{
			if (i + 2 >= len) // Need room for at least 2 more chars
			{
				LOG_INFO("Ran out of chars. Uh oh!");
				return std::string{};
			}

			if (lazy == true) // Are we being lazy? If so, just convert back from hex and append
			{
				output += hex::decode(std::string(input.data() + i + 1, 2));
				i += 2;
			} else
			{
				if (input[i + 1] == 'C')
				{
					if (i + 5 >= len) // Need room for 5 more chars
					{
						LOG_INFO("Ran out of chars. Uh oh!");
						return std::string{};
					}

					if (input[i + 2] == '2')
					{
						output += hex::decode(std::string(input.data() + i + 4, 2));

					} else if (input[i + 2] == '3')
					{
						auto adj = hex::decode(std::string(input.data() + i + 4, 2))[0] | 40;
						output.push_back(adj);

					} else
					{
						LOG_INFO("Something went wrong!");
						return std::string{};
					}

					i += 4;
				} else
				{
					output += hex::decode(std::string(input.data() + i + 1, 2));
					i += 2;
				}
			}
		} else // Otherwise it must be a regular character
		{
			output.push_back(input[i]);
		}
	}

  return output;
}

} // namespace url
