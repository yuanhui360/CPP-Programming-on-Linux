#include <crypto++/cryptlib.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>

#include <iostream>

int main (int argc, char* argv[])
{
    using namespace CryptoPP;

    Weak::MD5 hash;
    std::cout << "Name: " << hash.AlgorithmName() << std::endl;
    std::cout << "Digest size: " << hash.DigestSize() << std::endl;
    std::cout << "Block size: " << hash.BlockSize() << std::endl;
    std::cout << "Optim size: " << hash.OptimalBlockSize() << std::endl;

    return 0;
}
