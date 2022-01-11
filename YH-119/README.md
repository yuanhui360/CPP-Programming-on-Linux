 OpenSSL Message Digest :
     A Message Digest or Hash Function takes any arbitrary message
     (with any content or length) as an input and provides a fixed size
     hash value as a result
 
     1) Message Digest implements a secure one-way function.
        Original Data -> Secure Hash Value
     2) From Hash Value there is no indication about the
        original data that produced it.
     3) The only way to identify the original data is to “brute-force”
        through every  possible combination of inputs.
     4) Every message digest algorithm has a fixed-length output
        Example:
        SHA256  : Secure Hash Algorithm 256 bits hash value 32 Bytes
        MD5     : The MD5 message-digest algorithm 128-bits hash value 16 bytes.
        SHA512  : Secure Hash Algorithm 512 bits hash value 64 Bytes (Longest Known)
     5) Two distinct inputs the probability that both yield the same output is
        highly unlikely.
 
   Usage Example :
     1) create unique identifiers for arbitrary data or binary documents.
     2) ideal for public key signature algorithms
     3) To validate the integrity of a signed document
     4) Digital signatures or Digital certificates
 
   Supported digest
   SHA, SHA1, SHA224, SHA256, SHA384 and SHA512, MD2, MD4, MDC2 and MD5
   
   
