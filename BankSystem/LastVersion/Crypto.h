#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: Crypto.h                                         ||
//  || Section: Encryption & Decryption                       ||
//  || Functions for generating keys, encrypting and          ||
//  || decrypting sensitive data using libsodium.             ||
//  ||========================================================||

#include "Globals.h"

//=====================================================
//=============== Encryption & Decryption =============
//=====================================================

// Forward declare session folder getter (defined in Session.h)
string getSessionFolder();
void   createSessionFolder();

// Generate random encryption key
vector<unsigned char> generateEncryptionKey() {
    vector<unsigned char> key(crypto_secretbox_KEYBYTES);
    randombytes_buf(key.data(), key.size());
    return key;
}
// Load or create encryption key file securely
vector<unsigned char> getEncryptionKey() {
    vector<unsigned char> key(crypto_secretbox_KEYBYTES);
    string keyFolder = getSessionFolder();

#ifdef _WIN32
    string keyFile = keyFolder + "encryption_key.bsess";
#else
    string keyFile = keyFolder + ".encryption_key.bsess";
#endif

    ifstream file(keyFile, ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(key.data()), key.size());
        file.close();
        return key;
    }

    key = generateEncryptionKey();

    createSessionFolder();
    ofstream outFile(keyFile, ios::binary);
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char*>(key.data()), key.size());
        outFile.close();

#ifdef _WIN32
        DWORD attributes = GetFileAttributesA(keyFile.c_str());
        if (attributes != INVALID_FILE_ATTRIBUTES) {
            SetFileAttributesA(keyFile.c_str(), attributes | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
        }
        string permissionCommand = "icacls \"" + keyFile + "\" /inheritance:r /grant:r \"%USERNAME%:F\"";
        system(permissionCommand.c_str());
#else
        string permissionCommand = "chmod 600 \"" + keyFile + "\"";
        system(permissionCommand.c_str());
#endif
    }

    return key;
}
// Encrypt plaintext using libsodium secretbox
string encryptData(const string& plaintext, const vector<unsigned char>& key) {
    if (key.size() != crypto_secretbox_KEYBYTES) {
        throw runtime_error("Invalid key size for encryption");
    }
    if (plaintext.empty()) {
        throw runtime_error("Cannot encrypt empty data");
    }

    try {
        vector<unsigned char> nonce(crypto_secretbox_NONCEBYTES);
        randombytes_buf(nonce.data(), nonce.size());

        vector<unsigned char> ciphertext(plaintext.size() + crypto_secretbox_MACBYTES);

        if (crypto_secretbox_easy(ciphertext.data(),
            reinterpret_cast<const unsigned char*>(plaintext.c_str()),
            plaintext.size(),
            nonce.data(),
            key.data()) != 0) {
            throw runtime_error("Encryption operation failed");
        }

        string result;
        result.append(reinterpret_cast<const char*>(nonce.data()), nonce.size());
        result.append(reinterpret_cast<const char*>(ciphertext.data()), ciphertext.size());

        return result;
    }
    catch (const exception& e) {
        throw runtime_error(string("Encryption error: ") + e.what());
    }
}
// Decrypt ciphertext using libsodium secretbox
string decryptData(const string& encryptedData, const vector<unsigned char>& key) {
    if (key.size() != crypto_secretbox_KEYBYTES) {
        throw runtime_error("Invalid key size");
    }
    if (encryptedData.size() < crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES) {
        throw runtime_error("Encrypted data too short");
    }

    vector<unsigned char> nonce(crypto_secretbox_NONCEBYTES);
    copy(encryptedData.begin(), encryptedData.begin() + crypto_secretbox_NONCEBYTES, nonce.begin());

    vector<unsigned char> ciphertext(encryptedData.size() - crypto_secretbox_NONCEBYTES);
    copy(encryptedData.begin() + crypto_secretbox_NONCEBYTES, encryptedData.end(), ciphertext.begin());

    vector<unsigned char> plaintext(ciphertext.size() - crypto_secretbox_MACBYTES);

    if (crypto_secretbox_open_easy(plaintext.data(),
        ciphertext.data(),
        ciphertext.size(),
        nonce.data(),
        key.data()) != 0) {
        throw runtime_error("Decryption failed - tampered or corrupted data");
    }

    return string(reinterpret_cast<const char*>(plaintext.data()), plaintext.size());
}
