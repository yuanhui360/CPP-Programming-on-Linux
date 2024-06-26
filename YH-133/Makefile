CC=g++
CFLAGS=-c -Wall -Werror -g
LDFLAGS = -lcurl -lssh

all: curl_sftp_upload curl_sftp_download curl_ftps_upload curl_ftps_download

curl_sftp_upload: curl_sftp_upload.o
	$(CC) curl_sftp_upload.o -o curl_sftp_upload $(LDFLAGS)
curl_sftp_upload.o: curl_sftp_upload.cpp
	$(CC) $(CFLAGS) curl_sftp_upload.cpp

curl_sftp_download: curl_sftp_download.o
	$(CC) curl_sftp_download.o -o curl_sftp_download $(LDFLAGS)
curl_sftp_download.o: curl_sftp_download.cpp
	$(CC) $(CFLAGS) curl_sftp_download.cpp

curl_ftps_upload: curl_ftps_upload.o
	$(CC) curl_ftps_upload.o -o curl_ftps_upload $(LDFLAGS)
curl_ftps_upload.o: curl_ftps_upload.cpp
	$(CC) $(CFLAGS) curl_ftps_upload.cpp

curl_ftps_download: curl_ftps_download.o
	$(CC) curl_ftps_download.o -o curl_ftps_download $(LDFLAGS)
curl_ftps_download.o: curl_ftps_download.cpp
	$(CC) $(CFLAGS) curl_ftps_download.cpp

clean:
	rm -f *.o curl_sftp_upload curl_sftp_download curl_ftps_upload curl_ftps_download
