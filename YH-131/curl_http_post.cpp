#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

/*
 * https://gist.github.com/alghanmi/c5d7b761b2c9ab199157
 * https://curl.se/libcurl/c/libcurl-tutorial.html
 * https://curl.se/libcurl/c/curl_easy_setopt.html
 * https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 * http://192.168.0.107/cgi-enabled/myWebApp.cgi
 */

/*
 *
 *   The callback function gets called by libcurl as soon as there is data received
 *   that needs to be saved
 *    1) The maximum amount of body data is defined by CURL_MAX_WRITE_SIZE
 *    2) Set the userdata argument with the CURLOPT_WRITEDATA option
 *    3) If your callback function returns CURL_WRITEFUNC_PAUSE it will cause this
 *       transfer to become paused.
 *    4) this callback gets called many times and each invoke delivers another chunk of data.
 *
 *   size_t write_callback(char *ptr, size_t size, size_t nmemb, void *out);
 *
 *   ptr    − This is the pointer of in data.
 *   size   − This is the size in bytes of each element to be written.
 *   nmemb  − This is the number of elements, each one with a size of size bytes.
 *   out    − This is the pointer of out data.
 *
 */

static size_t WriteCallback(void *in_data, size_t size, size_t nmemb, void *out_data)
{
    size_t realsize = size * nmemb;
    ((std::string*)out_data)->append((char*)in_data, realsize);
    ((std::string*)out_data)->append((char*)in_data, realsize);
    return realsize;
}

int main(int argc, char* argv[])
{
    CURL *curl;
    CURLcode rc;
    std::string recvBuffer;
    char errbuf[CURL_ERROR_SIZE];
    memset(errbuf, '\0', CURL_ERROR_SIZE);

    if (argc != 3 )
    {
        std::cout << "Usage: " << argv[0] << " <url> <Post Fields>" << std::endl;
        std::cout << "Example : ";
        std::cout << argv[0] << " http://192.168.0.107:1234/MyEchoWS \"{ Name : Susan Collin, Gender : Famale }\" " << std::endl;
        return (EXIT_FAILURE);
    }

    std::string url_string = argv[1];

    curl_global_init(CURL_GLOBAL_ALL);   // <-- Global libcurl initialization
    curl = curl_easy_init();             // <-- init easy-session and get a handle
    if(curl) {

        //  1) setting all the options, https://curl.se/libcurl/c/easy_setopt_options.html
        curl_easy_setopt(curl, CURLOPT_URL, url_string.c_str());
        curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &recvBuffer);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, argv[2]);

        //  2) Perform synchronously data transfer request
        rc = curl_easy_perform(curl);
        if ( rc != CURLE_OK ) {
            std::cout << "curl_easy_perform failed HTTP GET request " << rc << std::endl;
        }

        //  3) Print HTTP fetch received data
        std::cout << recvBuffer << std::endl;

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return (EXIT_SUCCESS);
}
