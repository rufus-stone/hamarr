#include "hamarr/uuid.hpp"

#include "hamarr/prng.hpp"
#include "hamarr/hex.hpp"

namespace hmr::uuid
{

////////////////////////////////////////////////////////////
std::string generate() noexcept
{
  auto uuid = hmr::hex::encode(hmr::prng::bytes(16), false);

  uuid.insert(8, 1, '-');
  uuid.insert(13, 1, '-');
  uuid.insert(18, 1, '-');
  uuid.insert(23, 1, '-');

  return uuid;
}

} // namespace hmr::uuid
