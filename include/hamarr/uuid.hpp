#pragma once

#include <string>
#include <cassert>

#include "prng.hpp"
#include "hex.hpp"

namespace hmr::uuid
{

////////////////////////////////////////////////////////////
std::string generate()
{
  auto uuid = hmr::hex::encode(hmr::prng::bytes(16), false);
  assert(uuid.size() == 32);

  uuid.insert(8, 1, '-');
  uuid.insert(13, 1, '-');
  uuid.insert(18, 1, '-');
  uuid.insert(23, 1, '-');

  return uuid;
}

} // namespace hmr::uuid
