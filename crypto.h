#pragma onces
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1 
#include <string>
#include <cryptopp/hex.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
using namespace std;
namespace CPP = CryptoPP;
string auth(string salt, string pass);