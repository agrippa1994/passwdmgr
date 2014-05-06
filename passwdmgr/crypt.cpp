#include "crypt.h"

#include <crypto++/modes.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>
#include <crypto++/base64.h>

std::string base64_encode(const std::string & plain)
{
	std::string encoded;
	CryptoPP::StringSource(plain, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(encoded)));
	return encoded;
}

std::string base64_decode(const std::string & cypher)
{
	std::string decoded;
	CryptoPP::StringSource(cypher, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(decoded)));
	return decoded;
}

std::string encode(const std::string& key, const std::string & data)
{
	byte iv[CryptoPP::AES::BLOCKSIZE];
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

	std::string ciphertext;

	CryptoPP::AES::Encryption aesEncryption((const byte *) key.c_str(), 32);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(data.c_str()), data.length());
	stfEncryptor.MessageEnd();

	return base64_encode(ciphertext);
}

std::string decode(const std::string& key, const std::string& cipher)
{
	std::string ciphertext = base64_decode(cipher);

	byte iv[CryptoPP::AES::BLOCKSIZE];
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

	CryptoPP::AES::Decryption aesDecryption((const byte *) key.c_str(), 32);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

	std::string decryptedtext;

	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedtext));
	stfDecryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
	stfDecryptor.MessageEnd();

	return decryptedtext;
}