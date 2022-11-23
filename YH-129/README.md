In this short video, it will discuss one of another common topic in crypto systems,
Digital Signature, there are many schemes and algorithms for processing digital sign and verify.
In this short video, we just pick one popular used digital signature schemes as concept.
The RSA Signature Schemes RSASS. 

the example we are using will be RSA classic Bob sign message and send to Alice:
Alice ask Bob to sign original document and she will verify after received from Bob
Bob sign the document using his private key and send both original and sinature document to ALice 
Alice receive both original and signature document, and she can use Bob's public key to verify signature is OK or not.

In privious OpenSSL examples videos, we have demonstrated similar exmple using OpenSSL library, 
you can compare both examples and have your own opponion on
what is advantage and disadvantage between two libries.

Here on Linux, we have create two programes, 
rsass_sign.cpp    <- Bob sign document using his private key and generate signature file
rsass_verify.cpp  <- Alice verify original and signature document using Bob's public key

rsass_sign.cpp
{
    // This program is Bob use his private key to sign original document file
    1) Usage : there will be 3 arguments from command line.
    2) Load private key from key file (
    3) Instancialize Signer Object using RSA, PKCS1v15, SHA256 algorithm
    4) Perform pup line : argv[2] -> SingnerFilter(signer) -> HexEncoder -> argv[3]
}

rsass_verify.cpp
{
    // This program is Alice use Bob's public key to verify original document against signature file
    1) Usage : there will be 3 arguments from command line
    2) Load public key from key file
    3) Instancialize Verifier Object (RSA PKCS1v15 SHA256) <- same algorithm as signing
    4) concatenation of signature+message, DEFAULT_FLAGS = SIGNATURE_AT_BEGIN
    5) Verify result

    https://www.cryptopp.com/wiki/ArraySink
    An ArraySink, is a sink for byte arrays, introduced in version 5.6 of Crypto++, 
    since an array is fixed size, the ArraySink will effectively discard data once 
    the array is full. 

}

In this example, original document is text file, in real project, original document can be any format, 
for example, MS word, Excel or PDF file. Signature file can be merged together original document, 
for example x509 digital certificate
We use text file as example, just because it is easy to demonstrated the concept.

The video of this example has been uploaded on YouTube for your reference: 
https://youtu.be/1gah6glSe5s
