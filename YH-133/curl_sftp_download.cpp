#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

#include <libssh/libssh.h>
#include <libssh/libsshpp.hpp>

static size_t WriteCallback(void *buffer, size_t size, size_t nmemb, void *filename)
{
    size_t rc = 0;
    FILE *fp = std::fopen((char*)filename, "wb");
    rc = fwrite(buffer, size, nmemb, fp);
    fclose(fp);
    return rc;
}

int main(int argc, char* argv[])
{
    CURL *curl;
    std::string recvBuffer;
    char      my_PassWd[30];
    int ret =0;

   /*
    * libcurl url format : sftp://username:passwd@server_ip/from_filename
    *
    * To execute SFTP download it requires <server IP>, <login ID>, <password>, <from file> and [to file]
    * From file is on SFTP server, to file is local
    * usage will have 4 arguments
    * password will be securely keyed in from screen
    */

    if (argc < 4 )
    {
         std::cout << "Usage: " << argv[0] << " <server IP> <login ID> <From File> [To File]" << std::endl;
        std::cout << "Example : " << std::endl;
        std::cout << argv[0] << " 192.168.0.107 hyuan /tmp/my_text.txt" << std::endl;
        std::cout << argv[0] << " 192.168.0.107 hyuan /tmp/my_text.txt my_text.txt" << std::endl;
        return (EXIT_FAILURE);
    }

    memset(my_PassWd, '\0', sizeof(my_PassWd));
    ret = ssh_getpass("Password : ", my_PassWd, sizeof(my_PassWd), 0, 0);
    if ( ret != 0 ) {
        std::cout << "Error Password : " << ret << std::endl;
        return (EXIT_FAILURE);
    }

    /*
     *   libcurl url format : sftp://username:passwd@server_ip/path/from_filename
     */

    std::string url_string = "sftp://" + std::string(argv[2]) + ":" + my_PassWd + "@" + std::string(argv[1]);
    url_string += std::string(argv[3]);


    curl_global_init(CURL_GLOBAL_ALL); // <-- Global libcurl initialization
    curl = curl_easy_init();           // <-- init easy interface and get a handle
    if(curl) {

        /*
         *  1) setup url, and default Protocol SFTP
         *  2) CURLOPT_ERRORBUFFER - error buffer for error messages
         *  3) CURLOPT_WRITEFUNCTION - callback writing function
         *  4) CURLOPT_WRITEDATA - callback for writing received dat
         *  5) Perform the request
         *  6) Clean allocated memory for easy interface
         */
        curl_easy_setopt(curl, CURLOPT_URL, url_string.c_str());
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "sftp");

        char errbuf[CURL_ERROR_SIZE];
        memset(errbuf, '\0', CURL_ERROR_SIZE);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        if (argc == 5 )
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, argv[4]);
        else
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, argv[3]);


        CURLcode rc = curl_easy_perform(curl);
        if ( rc != CURLE_OK ) {
            std::cout << "curl_easy_perform not OK " << rc << std::endl;
            std::cout << "--> " << curl_easy_strerror(rc) << std::endl;
        }
    }

    /*
     *  clean global initialization allocated memory
     */

    curl_global_cleanup();
    return (EXIT_SUCCESS);
}
