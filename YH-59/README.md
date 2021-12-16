SSL Server and Client Example

Following is the video link of this example : 

* Simple SSL Server : https://youtu.be/P18wPeWtYfQ  
* Simple SSL Client : https://youtu.be/xTJRdn4tznM  

1) Generate the private key of the root CA:
* openssl genrsa -out rootCAKey.pem 2048

2) Generate the self-signed root CA certificate:
* openssl req -x509 -sha256 -new -nodes -key rootCAKey.pem -days 3650 -out rootCACert.pem

In this example, the validity period is 3650 days. Set the appropriate number of days for your company. Make a reminder to renew the certificate before it expires.

