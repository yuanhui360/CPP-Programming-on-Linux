In previous example it has discussed new library libcurl - client side url library, the example in previous video are using libcurl to write a C++ program that sending HTTP GET and HTTP POST request. 
As we know HTTP protcol is possible to get man-in--middle attack and it has been considered as not secure protcol. and many
API and project are disable of using HTTP and switched to using HTTPS protcol.

https://curl.se/libcurl/
Since there are more web security concerning and it looks like HTTPS is replacing HTTP in these days.
In this short video it will discuss how to use libcurl write a C++ program  to send HTTPS GET and HTTPS POST request.

Open SSL Create Certificate command :

openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout my_private.key -out my_x509_certificate.crt
openssl dhparam -out my_dh2048.pem 2048

curl command for fetch HTTPS web page :
curl -k -v -w'\n' --get https://your_web_url/

The video link of this example : https://youtu.be/6Ul0Lz5vNws
