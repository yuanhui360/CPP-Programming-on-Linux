#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

static size_t WriteCallback(void *in_data, size_t size, size_t nmemb, void *out_data)
{
    size_t realsize = size * nmemb;
    ((std::string*)out_data)->append((char*)in_data, realsize);
    return realsize;
}

int main(int argc, char* argv[])
{
    CURL *curl;
    CURLcode rc;
    std::string recvBuffer;
    char errbuf[CURL_ERROR_SIZE];

    if (argc < 5 )
    {
        std::cout << "Usage: " << argv[0] << " <url> <cert. file> <verify_peer 0|1> <verfiy_host 0|1|2> [Option: <parameter>]" << std::
        std::cout << "Example : " << std::endl;
        std::cout << argv[0] << " http://192.168.0.107:1234/MyEchoWS" << std::endl;
        std::cout << argv[0] << " http://192.168.0.107:1234/MyEchoWS my_certificate.crt 0 0 \"Name=Allan&Gender=Male\"" << std::endl;
        std::cout << argv[0] << " http://192.168.0.107:1234/MyEchoWS my_certificate.crt 0 2 \"Name=Allan&Gender=Male\"" << std::endl;
        return (EXIT_FAILURE);
    }

    std::string url_string = argv[1];
    std::string ws_cert_file = argv[2];
    long verify_peer = atoi(argv[3]);
    long verify_host = atoi(argv[4]);

    std::cout << "libcurl version : " << curl_version() << std::endl;
    std::cout << "********************************" << std::endl;

    if ( argc == 6 ) {
        url_string += "?";
        url_string += std::string(argv[2]);
        std::cout << "parameter : " << argv[2] << std::endl;
    }

    std::cout << "url : " << url_string << std::endl;

    // https://curl.se/libcurl/c/libcurl-tutorial.html
    curl_global_init(CURL_GLOBAL_ALL); // <-- Global libcurl initialization
    curl = curl_easy_init();           // <-- init easy interface and get a handle
    if(curl) {

        /*
         *  setup url, and if only allow HTTP, HTTPS Protocol
         */

        curl_easy_setopt(curl, CURLOPT_URL, url_string.c_str());
        // curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

        /*
         *  CURLOPT_ERRORBUFFER - error buffer for error messages
         */

        memset(errbuf, '\0', CURL_ERROR_SIZE);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

        /*
         *   CURLOPT_WRITEFUNCTION - callback for writing received data
         */

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &recvBuffer);

        /*
         * Set SSL options
         */
         curl_easy_setopt(curl, CURLOPT_CAPATH, ws_cert_file.c_str());
         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verify_peer);
         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verify_host);

        /*
         *  Perform the request
         */

        rc = curl_easy_perform(curl);
        if ( rc != CURLE_OK ) {
            std::cout << "curl_easy_perform not OK " << rc << std::endl;
            std::cout << "--> " << curl_easy_strerror(rc) << std::endl;
        }

        //  Clean allocated memory
        curl_easy_cleanup(curl);

        /*
         *  Print HTTP received data
         */

         std::cout << recvBuffer << std::endl;

        /*
         * CURLINFO_CONTENT_TYPE - get Content-Type
         */

         char *ct = NULL;
         rc = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
         if(rc == CURLE_OK && ct) {
             printf("Content-Type: %s\n", ct);
         }
    }

    curl_global_cleanup();
    return (EXIT_SUCCESS);
}
