This is example of another very important topic of OpenSSl API, the digital Certificate, also know as Public key Certificate, it is an electronic document used to prove the validity of a public key.
The x.509 certificate, one of widely used certificate format in TSL/SSl the base of SSL protocol.

Following are reference link 
1) https://en.wikipedia.org/wiki/Certificate_authority
2) https://en.wikipedia.org/wiki/Certificate_signing_request
3) https://en.wikipedia.org/wiki/Public_key_certificate
4) https://en.wikipedia.org/wiki/X.509

The C++ exaample can also be performaed through OpenSSL command line:
<-- Generate x509 certificate and private key:
#~> openssl req -x509 -sha256 -days 365 -nodes -newkey rsa:2048 -keyout api_private_key.pem -out api_x509.pem

<-- Extract public key from private key:
#~> openssl pkey -in api_private.key -pubout -out api_public_key.pem 

<-- View Certificates:
#~> openssl x509 -text -noout -in my_x509.pem

The video of this example link : https://youtu.be/b6yBB8M49PA
