#include <iostream>
#include <string>

#include "cryptopp/cryptlib.h"
#include "cryptopp/osrng.h"
#include "cryptopp/secblock.h"
#include "cryptopp/files.h"
#include "cryptopp/hex.h"
#include "cryptopp/hmac.h"
#include "cryptopp/modes.h"

using namespace CryptoPP;

void KeyGen(SecByteBlock& key)
{
	AutoSeededRandomPool prng;
	prng.GenerateBlock(key, key.size());
}

void Sign(SecByteBlock& key, std::string& plaintext, std::string& mac, std::string& hmacText)
{
	HMAC<SHA256> hmac(key, key.size());

	StringSource ss2(plaintext, true,
		new HashFilter(hmac,
			new StringSink(mac)));

	StringSource ss3(mac, true,
		new HexEncoder(
			new StringSink(hmacText)));
}

bool Verify(SecByteBlock& key, std::string& plainText, std::string& mac)
{
	try
	{
		HMAC<SHA256> hmac(key, key.size());
		const int flags = HashVerificationFilter::THROW_EXCEPTION | HashVerificationFilter::HASH_AT_END;

		StringSource(plainText + mac, true,
			new HashVerificationFilter(hmac, NULL, flags));

		return true;
	}
	catch (const CryptoPP::Exception& e)
	{
		return false;
	}

}


int main()
{
	SecByteBlock key(32);
	KeyGen(key);
	std::string plainText = "lab03", mac, hmacText;

	Sign(key, plainText, mac, hmacText);

	std::cout << "PlainText: " << plainText << std::endl;

	std::cout << "HMAC: " << hmacText << std::endl;

	mac[0] = 'x';

	if (Verify(key, plainText, mac))
	{
		std::cout << "Verified message" << std::endl;
	}
	else
	{
		std::cout << "Unverified message" << std::endl;
	}

	return 0;
}