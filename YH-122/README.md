This example will discuss another popular usage of EVP_PKEY RSA public/private key algorithm. Digital signature sign/verify algorithm.

A digital signature is a mathematical scheme for verifying the authenticity of digital messages or documents.

The example of this video is classical concept of digital signature usage. Alice send (document) contract offer to Bob, and ask him to sign the document and send back.
Bob signed document, he created digital signature file using his private key and send both original document and signature file back to ALice.
Alice received both original ducument and digital signature file and then she can use Bob's publick key to verify if the signature is OK (valid) or not.

High Level Sign Algorithm (process):
 1)  Generate a Message Digest hash of original data file
 2)  Encrypt the hash with a private key and producing the signature file
 3)  Distribute the original data, signature and RSA public key files

High Level Verify Signature Algorithm (process):
  1) Generate a Message Digest hash of the original data file
  2) Use the signer’s public key to decrypt the signature file
  3) Check if the two hash files match

Obviously the crypto hash algorithm has to be same in both signing and verifing process.

This C++ example can also be performed through openssl command line API.
* openssl dgst -sha256 -sign my_rsa_private_key.pem -out api_signature.dat original_document.txt
* openssl dgst -sha256 -verify my_rsa_public_key.pem -signature api_signature.dat original_document.txt

The video of this example cab be found on YouTube: https://youtu.be/sbWbfJjfjaU
