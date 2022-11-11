#include "cryptopp/cryptlib.h"
#include "cryptopp/osrng.h"
#include "cryptopp/files.h"
#include "cryptopp/ecp.h"
#include "cryptopp/eccrypto.h"
#include "cryptopp/osrng.h"
#include "cryptopp/oids.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace CryptoPP;
using namespace std;


void KeyGen(ECDSA<ECP, SHA256>::PrivateKey& privateKey, ECDSA<ECP, SHA256>::PublicKey& publicKey)
{
	AutoSeededRandomPool prng;
	privateKey.Initialize(prng, ASN1::secp256r1());

	privateKey.MakePublicKey(publicKey);
}

void Sign(ECDSA<ECP, SHA256>::PrivateKey& privateKey, string& plaintext, string& signature)
{

	AutoSeededRandomPool prng;
	ECDSA<ECP, SHA256>::Signer signer(privateKey);

	size_t siglen = signer.MaxSignatureLength();
	signature.assign(siglen, 0x00);
	siglen = signer.SignMessage(prng, (const byte*)&plaintext[0], plaintext.size(), (byte*)&signature[0]);
	signature.resize(siglen);
}

void Verify(PublicKey& publicKey, string& plaintext, string& signature)
{
	ECDSA<ECP, SHA256>::Verifier verifier(publicKey);

	if (verifier.VerifyMessage((const byte*)&plaintext[0], plaintext.size(), (const byte*)&signature[0], signature.size()))
		std::cout << "Good" << std::endl;
	else
		cerr << "Not good" << std::endl;
}


int main()
{
	string plaintext = "lab05";

	ECDSA<ECP, SHA256>::PrivateKey privateKey;
	ECDSA<ECP, SHA256>::PublicKey publicKey;
	KeyGen(privateKey, publicKey);
	string signature;
	Sign(privateKey, plaintext, signature);

	// ++signature[0];
	// ++plaintext[0];

	cout << "plaintext: " << plaintext << endl;
	cout << "signature: " << signature << endl;
	Verify(publicKey, plaintext, signature);
}