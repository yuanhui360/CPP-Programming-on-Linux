#include<iostream>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

class udpSocket
{
public:
    udpSocket( char * inServer, int inPort) : port(inPort), sByte(0), rByte(0) {
        memset(bufferSend, '\0', sizeof(bufferSend)+1);
        memset(bufferRecv, '\0', sizeof(bufferRecv)+1);
        memset(server, '\0', sizeof(server)+1);
        memcpy(server, inServer, strlen(inServer));
    }

    ssize_t sendRecv( char * inMsg) {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        struct sockaddr_in servAddr;
        struct sockaddr_in  cliAddr;
        socklen_t cLen = sizeof(cliAddr);
        socklen_t sLen = sizeof(servAddr);

        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(port);
        servAddr.sin_addr.s_addr = inet_addr(server);
        memcpy(bufferSend, inMsg, strlen(inMsg));
        sByte = sendto(sockfd,bufferSend,sizeof(bufferSend),0,(struct sockaddr * )&servAddr,sLen);
        std::cout << "[" << sByte << "] Bytes Sent : " << std::endl;
        rByte = recvfrom(sockfd,bufferRecv,sizeof(bufferRecv),0,(struct sockaddr *)&cliAddr,&cLen);
        close(sockfd);
        return sByte;
    }

    void  printMsg() {
        std::cout << "[" << rByte << "] Bytes Rcvd : " << bufferRecv << std::endl;
    }

    char* getRecvMsg() {
        return bufferRecv;
    }

    ssize_t getRecvBytes() {
        return rByte;
    }

    ~udpSocket() {
    }

private:
    int     port;
    ssize_t sByte;
    ssize_t rByte;
    char    server[100];
    char bufferSend[256];
    char bufferRecv[256];
};

int main(int argc, char* argv[])
{
    if ( argc != 4 ) {
        std::cout << "Usage:udp_client [server] [port] [Message]" << std::endl;
        exit(-1);
    }

    udpSocket mUDP(argv[1], std::stoi(argv[2]));
    if ( mUDP.sendRecv(argv[3]) > 0 ) {
        // mUDP.printMsg();
        std::cout << "[" << mUDP.getRecvBytes() << "] " << mUDP.getRecvMsg() << std::endl;
    }

    exit(0);
}
 
