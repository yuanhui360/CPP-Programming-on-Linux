#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#include <libssh/libssh.h>
#include <libssh/libsshpp.hpp>

static size_t WriteCallback(void *buffer, size_t size, size_t nmemb, void *filename)
{
    size_t rc;
    FILE *fp = std::fopen((char*)filename, "wb");
    rc = fwrite(buffer, size, nmemb, fp);
    fclose(fp);
    return rc;
}

int main(int argc, char* argv[])
{
    CURL *curl;
    std::string readBuffer;
    char      my_PassWd[30];
    int ret =0;

   /*
    * libcurl url format : ftp://username:passwd@server_ip/from_filename
    *
    * To execute FTP download it requires <server IP>, <cert> <key> <login ID>, <password>,
    *                                     <from file> and [to file]
    * From file is on FTP server, to file is local
    *
    * compare to SFTP example, here it adds 2 more arguments. <cert> <key> for SSL
    * password will be securely keyed in from screen
    */

    if (argc < 6 )
    {
        std::cout << "Usage: " << argv[0] << " <url> <cert> <key> <usr> <file from> [file to]" << std::endl;
        std::cout << "Example : ";
        std::cout << argv[0] << " 192.168.0.107 my_cert.crt my_test.txt /tmp/my_test.txt " << std::endl;
        return (EXIT_FAILURE);
    }

    /*
     *   read password from screen
     */

    memset(my_PassWd, '\0', sizeof(my_PassWd));
    ret = ssh_getpass("Password : ", my_PassWd, sizeof(my_PassWd), 0, 0);
    if ( ret != 0 ) {
        std::cout << "Error Password : " << ret << std::endl;
        return (EXIT_FAILURE);
    }

    /*
     *   url format : ftp://username:passwd@server_ip/path/from_filename
     */

    std::string url_string = "ftp://" + std::string(argv[4]) + ":" + my_PassWd + "@" + std::string(argv[1]);
    url_string += std::string(argv[5]);

    curl_global_init(CURL_GLOBAL_ALL); // <-- Global libcurl initialization
    curl = curl_easy_init();           // <-- init easy interface and get a handle
    if(curl) {

        /*
         *  1) setup url, and default Protocol SFTP
         *  2) CURLOPT_ERRORBUFFER - error buffer for error messages
         *  3) CURLOPT_WRITEFUNCTION - callback writing function
         *  4) CURLOPT_WRITEDATA - callback for writing received dat
         *  5) CURLOPT_SSLCERT, CURLOPT_SSLKEY, CURLOPT_SSL_VERIFYPEER, CURLOPT_SSL_VERIFYHOST
         *  6) Perform the request
         *  7) Clean allocated memory for easy interface
         */
        curl_easy_setopt(curl, CURLOPT_URL, url_string.c_str());
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "ftps");

        char errbuf[CURL_ERROR_SIZE];
        memset(errbuf, '\0', CURL_ERROR_SIZE);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        if (argc == 7 )
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, argv[6]);
        else
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, argv[5]);


        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLCERT, argv[2]);
        curl_easy_setopt(curl, CURLOPT_SSLKEY, argv[3]);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        CURLcode rc = curl_easy_perform(curl);
        if ( rc != CURLE_OK ) {
            std::cout << "curl_easy_perform() not OK " << std::endl;
            std::cout << " --> " << curl_easy_strerror(rc) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return (EXIT_SUCCESS);
}
