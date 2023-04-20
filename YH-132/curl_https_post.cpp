#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

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
    std::string readBuffer;
    char errbuf[CURL_ERROR_SIZE];

    if (argc != 6 )
    {
        std::cout << "Usage: " << argv[0] << " <url> <Post Fields>"
                               << "<cert. file> <verify_peer 0|1> <verify_host 0|1|2>" << std::endl;
        std::cout << "Example : ";
        std::cout << argv[0] << " http://192.168.0.107:1234/MyEchoWS \"{ Name : Susan Collin, Gender : Famale }\" "
                  << "./my_certificate.crt 0 0 " << std::endl;
        std::cout << argv[0] << " http://192.168.0.107:1234/MyEchoWS \"{ Name : Susan Collin, Gender : Famale }\" "
                  << "./my_certificate.crt 0 2 " << std::endl;
        return (EXIT_FAILURE);
    }

    std::string url_string = argv[1];
    std::string ws_cert_file = argv[3];
    long verify_peer = atoi(argv[4]);
    long verify_host = atoi(argv[5]);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {

        /*
         *  if only allow HTTP, HTTPS Protocol
         */

        curl_easy_setopt(curl, CURLOPT_URL, url_string.c_str());
        curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

        /*
         *  CURLOPT_ERRORBUFFER - error buffer for error messages
         */

        memset(errbuf, '\0', CURL_ERROR_SIZE);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

        /*
         *   CURLOPT_WRITEFUNCTION - callback for writing received data
         */

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        /*
         *   Set CURLOPT_POSTFIELDS
         *   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{ Department : General Office, Position : Assisstant }");
         *   curl_easy_setopt(curl, CURLOPT_POST, 1L); <- specify post method
         */

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, argv[2]);

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
        curl_easy_cleanup(curl);

         std::cout << readBuffer << std::endl;

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
