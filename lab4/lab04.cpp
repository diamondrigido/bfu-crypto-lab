#include <iostream>
#include <string>
#include <iomanip>
#include <string>
#include <sstream>

#include "cryptopp/cryptlib.h"
#include "cryptopp/osrng.h"
#include "cryptopp/secblock.h"
#include "cryptopp/files.h"
#include "cryptopp/hex.h"
#include "cryptopp/modes.h"
#include "cryptopp/gcm.h"
#include "cryptopp/aes.h"

using namespace CryptoPP;

void KeyGen(SecByteBlock& key, SecByteBlock& iv)
{
	AutoSeededRandomPool prng;
	prng.GenerateBlock(key, key.size());
	prng.GenerateBlock(iv, iv.size());
}

void EncAndSign(SecByteBlock& key, SecByteBlock& iv, std::string& plaintext, std::string& aad, std::string& ciphertext, std::string& tag)
{
	GCM<AES>::Encryption enc;
	enc.SetKeyWithIV(key, key.size(), iv, iv.size());

	tag.resize(enc.DigestSize());
	ciphertext.resize(plaintext.size());

	enc.EncryptAndAuthenticate(
		(byte *) &ciphertext[0],
		(byte *) &tag[0],
		tag.size(),
		iv,
		sizeof(iv),
		(byte *) &aad[0],
		aad.size(),
		(byte *) &plaintext[0],
		plaintext.size()
	);
}

bool DecAndVerify(SecByteBlock& key, SecByteBlock& iv, std::string& ciphertext, std::string& tag, std::string& aad, std::string& decrypted)
{
	GCM<AES>::Decryption dec;
	dec.SetKeyWithIV(key, key.size(), iv, iv.size());

	decrypted.resize(ciphertext.size());

	return 	dec.DecryptAndVerify(
		(byte*)&decrypted[0],
		(byte*)&tag[0],
		tag.size(),
		iv,
		sizeof(iv),
		(byte*)&aad[0],
		aad.size(),
		(byte*)&ciphertext[0],
		ciphertext.size()
	);
}


int main()
{
	std::string plaintext = "lab04";
	std::string aad = "additional auth data";
	std::string tag, ciphertext, decrypted;

	SecByteBlock key(AES::DEFAULT_KEYLENGTH), iv(AES::BLOCKSIZE);
	KeyGen(key, iv);

	std::cout << "Plaintext: " << plaintext << std::endl;

	EncAndSign(key, iv, plaintext, aad, ciphertext, tag);

	HexEncoder hex(new FileSink(std::cout));

	std::cout << "Tag: ";
	hex.Put((byte*)&tag[0], tag.size());
	hex.MessageEnd();
	std::cout << std::endl;

	std::cout << "Ciphertext: ";
	hex.Put((byte*)&ciphertext[0], ciphertext.size());
	hex.MessageEnd();
	std::cout << std::endl;

	if (DecAndVerify(key, iv, ciphertext, tag, aad, decrypted))
	{
		std::cout << "Verified." << std::endl;
		std::cout << "Decrypted Text: " << decrypted << std::endl;
	}
	else
	{
		std::cout << "Verification failed" << std::endl;
	}

	return 0;
}