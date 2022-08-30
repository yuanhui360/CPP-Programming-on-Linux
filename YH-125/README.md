In this example, it demonstrated how to use Crypto++ Library to perform Message Digest. 
The example of this video use 2 typical message digest algorithms, MD5 and SHA256.
In Crypto++ Library, MD5 is no longer considered secure, and it has been declared in the Weak namespace. 
Weak is part of the CryptoPP namespace. it contains weak and wounded algorithms. Schemes and algorithms are moved 
into Weak when their security level is reduced to an unacceptable level by contemporary standards.
To use an algorithm in the Weak namespace, you must #define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1 before including a header for a weak or wounded algorithm.
SHA256 algorithm is not part of Weak namespace, program is able to include the header file directly.

All Crypto++ hashes derive from HashTransformation. The base class provides functions like Update, Final and Verify. 
You can swap-in any hash for any other hash in your program.

The example video link :
https://youtu.be/8PQvGpAQjbA 2022-08-29
