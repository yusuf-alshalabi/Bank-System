#pragma once
#include <sodium.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace Bank::Security {

	// RAII escalation guard: ensures libsodium is initialized exactly once for
	// the process (sodium_init is idempotent, so repeated guards are cheap).
	class SodiumRaiiGuard
	{
	public:
		SodiumRaiiGuard()
		{
			if (sodium_init() < 0)
				throw std::runtime_error("CryptoEngine: libsodium failed to initialize");
		}

		SodiumRaiiGuard(const SodiumRaiiGuard&) = delete;
		SodiumRaiiGuard& operator=(const SodiumRaiiGuard&) = delete;
	};

	// Zero-initialized secretbox key buffer. Overwrites its bytes on destruction
	// so plaintext key material does not linger in anonymous memory.
	class SecretKey
	{

	private:
		std::vector<unsigned char> _Bytes;

	public:
		SecretKey() : _Bytes(crypto_secretbox_KEYBYTES, '\0') {}

		unsigned char* Data() noexcept { return _Bytes.data(); }
		const unsigned char* Data() const noexcept { return _Bytes.data(); }
		std::size_t Size() const noexcept { return _Bytes.size(); }

		~SecretKey()
		{
			sodium_memzero(_Bytes.data(), _Bytes.size());
		}
	};

	// XChaCha20-Poly1305 authenticated encryption (migration of LastVersion/Crypto.h).
	// EncryptBlob output layout: [24-byte nonce][ciphertext + 16-byte MAC].
	class CryptoEngine
	{
	public:
		static SecretKey GenerateKey()
		{
			SodiumRaiiGuard guard;
			SecretKey key;
			randombytes_buf(key.Data(), key.Size());
			return key;
		}

		// Loads the key file if present, otherwise creates a fresh key and persists
		// it inside the user-profile directory it was requested from.
		static SecretKey LoadOrCreateKey(const std::filesystem::path& keyFile)
		{
			std::error_code ec;
			if (std::filesystem::exists(keyFile, ec))
			{
				SecretKey key;
				std::ifstream stream(keyFile, std::ios::binary);
				if (stream && stream.read(reinterpret_cast<char*>(key.Data()), key.Size()))
					return key;
				throw std::runtime_error("CryptoEngine: key file is corrupted or unreadable");
			}

			SecretKey key = GenerateKey();
			std::filesystem::create_directories(keyFile.parent_path(), ec);
			std::ofstream stream(keyFile, std::ios::binary | std::ios::trunc);
			stream.write(reinterpret_cast<const char*>(key.Data()), key.Size());
			if (!stream)
				throw std::runtime_error("CryptoEngine: failed to persist encryption key file");
			return key;
		}

		static std::string EncryptBlob(const std::string& plaintext, const SecretKey& key)
		{
			if (plaintext.empty())
				throw std::invalid_argument("CryptoEngine: refusing to encrypt an empty payload");

			SodiumRaiiGuard guard;

			std::vector<unsigned char> nonce(crypto_secretbox_NONCEBYTES);
			randombytes_buf(nonce.data(), nonce.size());

			std::vector<unsigned char> ciphertext(plaintext.size() + crypto_secretbox_MACBYTES);
			if (crypto_secretbox_easy(ciphertext.data(),
				reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size(),
				nonce.data(), key.Data()) != 0)
				throw std::runtime_error("CryptoEngine: encryption failed");

			std::string result;
			result.reserve(nonce.size() + ciphertext.size());
			result.append(reinterpret_cast<const char*>(nonce.data()), nonce.size());
			result.append(reinterpret_cast<const char*>(ciphertext.data()), ciphertext.size());
			return result;
		}

		static std::string DecryptBlob(const std::string& payload, const SecretKey& key)
		{
			constexpr std::size_t Overhead = crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES;
			if (payload.size() < Overhead)
				throw std::runtime_error("CryptoEngine: encrypted payload is too short");

			SodiumRaiiGuard guard;

			const unsigned char* nonce = reinterpret_cast<const unsigned char*>(payload.data());
			const unsigned char* ciphertext = nonce + crypto_secretbox_NONCEBYTES;
			std::size_t ciphertextLength = payload.size() - crypto_secretbox_NONCEBYTES;

			std::vector<unsigned char> plaintext(ciphertextLength - crypto_secretbox_MACBYTES);
			if (crypto_secretbox_open_easy(plaintext.data(), ciphertext, ciphertextLength,
				nonce, key.Data()) != 0)
				throw std::runtime_error("CryptoEngine: decryption failed - payload is corrupted or tampered");

			return std::string(reinterpret_cast<const char*>(plaintext.data()), plaintext.size());
		}
	};

} // namespace Bank::Security