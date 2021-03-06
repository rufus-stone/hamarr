#include "hamarr/crypto.hpp"

#include <algorithm>

#include <openssl/aes.h>
#include <openssl/evp.h>

#include "hamarr/format.hpp"
#include "hamarr/hex.hpp"
#include "hamarr/pkcs7.hpp"
#include "hamarr/bitwise.hpp"
#include "hamarr/exceptions.hpp"

namespace hmr::crypto
{

static constexpr std::size_t md5_len = 16;
static constexpr std::size_t sha1_len = 20;
static constexpr std::size_t sha256_len = 32;

static constexpr std::size_t aes_block_size = 16;
static constexpr std::size_t aes_128_key_bits = 128;

////////////////////////////////////////////////////////////////
auto aes_ecb_encrypt_block(std::string_view input, std::string_view key) -> std::string
{
  std::size_t const len = input.size();

  // AES-128 keys and blocks must be 16 bytes long
  if (len != aes_block_size)
  {
    throw hmr::xcpt::crypto::invalid_input("Input block must be 16 bytes long!");
  }

  if (key.size() != aes_block_size)
  {
    throw hmr::xcpt::crypto::invalid_key_length("AES-128 keys must be 16 bytes long!");
  }

  auto const *plaintext_ptr = reinterpret_cast<uint8_t const *>(input.data());
  auto const *key_ptr = reinterpret_cast<uint8_t const *>(key.data());

  auto encrypted = std::vector<uint8_t>(len, 0x00); // We have to initialise the vector with something to start with

  AES_KEY aes_key;
  AES_set_encrypt_key(key_ptr, aes_128_key_bits, &aes_key);

  AES_encrypt(plaintext_ptr, encrypted.data(), &aes_key);

  auto result = std::string{};
  result.reserve(encrypted.size());

  std::copy(std::begin(encrypted), std::end(encrypted), std::back_inserter(result));

  return result;
}


////////////////////////////////////////////////////////////////
auto aes_ecb_decrypt_block(std::string_view input, std::string_view key) -> std::string
{
  std::size_t const len = input.size();

  // AES-128 keys and blocks must be 16 bytes long
  if (len != aes_block_size)
  {
    throw hmr::xcpt::crypto::invalid_input("Input block must be 16 bytes long!");
  }

  if (key.size() != aes_block_size)
  {
    throw hmr::xcpt::crypto::invalid_key_length("Key must be 16 bytes long!");
  }

  auto const *ciphertext_ptr = reinterpret_cast<uint8_t const *>(input.data());
  auto const *key_ptr = reinterpret_cast<uint8_t const *>(key.data());

  auto decrypted = std::vector<uint8_t>(len, 0x00); // We have to initialise the vector with something to start with

  AES_KEY aes_key;
  AES_set_decrypt_key(key_ptr, aes_128_key_bits, &aes_key);

  AES_decrypt(ciphertext_ptr, decrypted.data(), &aes_key);

  auto result = std::string{};
  result.reserve(decrypted.size());

  std::copy(std::begin(decrypted), std::end(decrypted), std::back_inserter(result));

  return result;
}


////////////////////////////////////////////////////////////////
auto aes_ecb_encrypt(std::string_view input, std::string_view key) -> std::string
{
  // AES-128 keys must be 16 bytes long
  if (key.size() != aes_block_size)
  {
    throw hmr::xcpt::crypto::invalid_key_length("Key must be 16 bytes long!");
  }

  // Will the input need padding? Make sure we account for this when initialising the output vector
  std::size_t const len = input.size();
  std::size_t const padding = ((len % aes_block_size) == 0) ? 0 : aes_block_size - (len % aes_block_size);

  auto result = std::string{};
  result.reserve(len + padding);

  // How many complete blocks are there
  std::size_t const num_blocks = len / 16;

  // Encrypt all the complete blocks first
  std::size_t offset = 0;
  for (offset = 0; offset < (num_blocks * aes_block_size); offset += aes_block_size)
  {
    auto encrypted_block = aes_ecb_encrypt_block(input.substr(offset, aes_block_size), key);
    result += encrypted_block;
  }

  // If the input wasn't a multiple of 16 bytes, there will be a final block that needs padding before it can be encrypted
  if (len % aes_block_size != 0)
  {
    auto final_block = hmr::pkcs7::pad(std::string(input.data() + offset, len - offset));
    auto encrypted_block = aes_ecb_encrypt_block(final_block, key);
    result += encrypted_block;
  }

  return result;
}


////////////////////////////////////////////////////////////////
auto aes_ecb_decrypt(std::string_view input, std::string_view key, bool remove_padding) -> std::string
{
  std::size_t const len = input.size();

  // AES encrypted data should be a multiple of 16 bytes
  if (len % aes_block_size != 0)
  {
    throw hmr::xcpt::crypto::invalid_input("Ciphertext should be a multiple of 16 bytes in length!");
  }

  auto result = std::string{};

  // How many blocks are there
  std::size_t const num_blocks = len / aes_block_size;
  std::size_t current_block = 1;

  // Decrypt all the blocks
  for (std::size_t offset = 0; offset < len; offset += aes_block_size, ++current_block)
  {
    auto decrypted_block = aes_ecb_decrypt_block(input.substr(offset, aes_block_size), key);

    // If this was the last block, check whether we need to remove padding
    if (current_block == num_blocks)
    {
      if (remove_padding)
      {
        result += hmr::pkcs7::unpad(decrypted_block);
      } else
      {
        result += decrypted_block;
      }
    } else
    {
      result += decrypted_block;
    }
  }

  return result;
}


////////////////////////////////////////////////////////////////
auto aes_cbc_encrypt(std::string_view input, std::string_view key, std::string const &iv) -> std::string
{
  // AES-128 keys and IVs must be 16 bytes long
  if (key.size() != aes_block_size)
  {
    throw hmr::xcpt::crypto::invalid_key_length("Key must be 16 bytes long!");
  }

  if (iv.size() != aes_block_size)
  {
    throw hmr::xcpt::crypto::invalid_iv_length("IV must be 16 bytes long!");
  }

  // Will the input need padding? Make sure we account for this when initialising the output string
  std::size_t const len = input.size();
  std::size_t const padding = ((len % aes_block_size) == 0) ? 0 : aes_block_size - (len % aes_block_size);

  auto result = std::string{};
  result.reserve(len + padding);

  // How many complete blocks are there
  std::size_t const num_blocks = len / aes_block_size;

  // In CBC mode, each block of ciphertext is XORed against the next block of plaintext before that plaintext is encrypted. The IV is used as a fake block of ciphertext to kick things off
  auto previous_ciphertext = iv;

  // Encrypt all the complete blocks first
  std::size_t offset = 0;
  for (offset = 0; offset < (num_blocks * aes_block_size); offset += aes_block_size)
  {
    // XOR the plaintext against the previous ciphertext
    auto xord = hmr::bitwise::xor_with_key(input.substr(offset, aes_block_size), previous_ciphertext);
    auto encrypted_block = aes_ecb_encrypt_block(xord, key);

    previous_ciphertext = encrypted_block;

    result += encrypted_block;
  }

  // If the input wasn't a multiple of 16 bytes, there will be a final block that needs padding before it can be encrypted
  if (len % aes_block_size != 0)
  {
    auto final_block = hmr::pkcs7::pad(std::string(input.data() + offset, len - offset));
    auto xord = hmr::bitwise::xor_with_key(final_block, previous_ciphertext);
    auto encrypted_block = aes_ecb_encrypt_block(xord, key);

    result += encrypted_block;
  }

  return result;
}


////////////////////////////////////////////////////////////////
auto aes_cbc_decrypt(std::string_view input, std::string_view key, std::string const &iv, bool remove_padding) -> std::string
{
  std::size_t const len = input.size();

  // AES-128 keys and IVs must be 16 bytes long
  if (key.size() != aes_block_size)
  {
    throw hmr::xcpt::crypto::invalid_key_length("Key must be 16 bytes long!");
  }

  if (iv.size() != aes_block_size)
  {
    throw hmr::xcpt::crypto::invalid_iv_length("IV must be 16 bytes long!");
  }

  // Ciphertext must be a multiple of 16 bytes long
  if (len % aes_block_size != 0)
  {
    throw hmr::xcpt::crypto::invalid_input("Ciphertext should be a multiple of 16 bytes in length!");
  }

  auto result = std::string{};
  result.reserve(len); // If the decrypted data is padded then the actual result will be smaller than len, but it can't hurt to over reserve

  // How many blocks are there
  std::size_t const num_blocks = len / aes_block_size;
  std::size_t current_block = 1;

  // In CBC mode, each block of ciphertext is XORed against the next block of plaintext after that plaintext is decrypted. The IV is used as a fake block of ciphertext to kick things off
  auto previous_ciphertext = iv;

  // Decrypt all the blocks
  for (std::size_t offset = 0; offset < len; offset += aes_block_size, ++current_block)
  {
    // Decrypt the block
    auto decrypted_block = aes_ecb_decrypt_block(input.substr(offset, aes_block_size), key);

    // XOR the plaintext against the previous ciphertext
    auto xord = hmr::bitwise::xor_with_key(decrypted_block, previous_ciphertext);

    previous_ciphertext = std::string(input.data() + offset, aes_block_size);

    // If this was the last block, check whether we need to remove padding
    if (current_block == num_blocks)
    {
      if (remove_padding)
      {
        result += hmr::pkcs7::unpad(xord);
      } else
      {
        result += xord;
      }
    } else
    {
      result += xord;
    }
  }

  return result;
}


////////////////////////////////////////////////////////////////
auto md5_raw(std::string_view input) -> std::string
{
  auto const *input_ptr = reinterpret_cast<uint8_t const *>(input.data());
  std::size_t const input_len = input.size();

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  EVP_MD const *md5 = EVP_md5();

  auto hash = std::array<uint8_t, md5_len>{};
  unsigned int hash_len = 0;

  EVP_DigestInit_ex(ctx, md5, nullptr);

  EVP_DigestUpdate(ctx, input_ptr, input_len);
  EVP_DigestFinal_ex(ctx, hash.data(), &hash_len);
  EVP_MD_CTX_free(ctx);

  auto result = std::string{};
  result.reserve(hash.size());

  std::copy(std::begin(hash), std::end(hash), std::back_inserter(result));

  return result;
}


////////////////////////////////////////////////////////////////
auto md5(std::string_view input) -> std::string
{
  return hmr::fmt::to_lower(hmr::hex::encode(md5_raw(input), false));
}


////////////////////////////////////////////////////////////////
auto sha1_raw(std::string_view input) -> std::string
{
  auto const *input_ptr = reinterpret_cast<uint8_t const *>(input.data());
  std::size_t const input_len = input.size();

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  EVP_MD const *sha1 = EVP_sha1();

  auto hash = std::array<uint8_t, sha1_len>{};
  unsigned int hash_len = 0;

  EVP_DigestInit_ex(ctx, sha1, nullptr);

  EVP_DigestUpdate(ctx, input_ptr, input_len);
  EVP_DigestFinal_ex(ctx, hash.data(), &hash_len);
  EVP_MD_CTX_free(ctx);

  auto result = std::string{};
  result.reserve(hash.size());

  std::copy(std::begin(hash), std::end(hash), std::back_inserter(result));

  return result;
}


////////////////////////////////////////////////////////////////
auto sha1(std::string_view input) -> std::string
{
  return hmr::fmt::to_lower(hmr::hex::encode(sha1_raw(input), false));
}


////////////////////////////////////////////////////////////////
auto sha256_raw(std::string_view input) -> std::string
{
  auto const *input_ptr = reinterpret_cast<uint8_t const *>(input.data());
  std::size_t const input_len = input.size();

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  EVP_MD const *sha256 = EVP_sha256();

  auto hash = std::array<uint8_t, sha256_len>{};
  unsigned int hash_len = 0;

  EVP_DigestInit_ex(ctx, sha256, nullptr);

  EVP_DigestUpdate(ctx, input_ptr, input_len);
  EVP_DigestFinal_ex(ctx, hash.data(), &hash_len);
  EVP_MD_CTX_free(ctx);

  auto result = std::string{};
  result.reserve(hash.size());

  std::copy(std::begin(hash), std::end(hash), std::back_inserter(result));

  return result;
}


////////////////////////////////////////////////////////////////
auto sha256(std::string_view input) -> std::string
{
  return hmr::fmt::to_lower(hmr::hex::encode(sha256_raw(input), false));
}

} // namespace hmr::crypto
