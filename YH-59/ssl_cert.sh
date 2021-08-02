#!/usr/bin/sh
openssl req -x509 -sha256 -nodes -newkey rsa:2048 -keyout server.key -out server.pem

