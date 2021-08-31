Rabbist is light weight message broker software, this is example of how to write a C++ code to send and receive message using RabbitMQ.

****** Installation RabitMQ server on Debian/Ubuntu Linux *********
Reference : https://www.rabbitmq.com/install-debian.html#apt-pinning

1) sudo apt-get update -y

2) Install Erlang packages
sudo apt-get install -y erlang-base \
                        erlang-asn1 erlang-crypto erlang-eldap erlang-ftp erlang-inets \
                        erlang-mnesia erlang-os-mon erlang-parsetools erlang-public-key \
                        erlang-runtime-tools erlang-snmp erlang-ssl \
                        erlang-syntax-tools erlang-tftp erlang-tools erlang-xmerl

3) sudo apt-get install rabbitmq-server -y --fix-missing
4) Check RabbitMQ server status

********* Start/stop/status of RabbitMQ server *******
sudo service rabbitmq-server status
sudo service rabbitmq-server start
sudo service rabbitmq-server stop

***** Install RabbitMQ Client C Library *****
Reference:
https://www.rabbitmq.com/devtools.html
   -> (C and C++) RabbitMQ C client
   -> (Latest Stable Version)
   -> https://github.com/alanxz/rabbitmq-c/releases/tag/v0.11.0

1) download C++ client library file
   rabbitmq-c-0.11.0.tar.gz
2) untar extract file
   tar -xzf rabbitmq-c-0.11.0.tar.gz
3) Go to extracted directory
   cd rabbitmq-c-0.11.0
4) compile and install C++ client library
   mkdir build
   cd build
   cmake ..
   make
   su - root
   make install
   ldconfig
5) check header file under /usr/local/include
   check library file under /usr/local/lib/x86_64-linux-gnu/
   ldd librabbitmq.so

***** Install RabbitMQ Client C++ Library *****
reference :
https://www.rabbitmq.com/devtools.html
   -> AMQP-CPP, a C++ RabbitMQ client
cd /home/hyuan/cpp/rabbitmq_ex/amqpcpp_lib
1) Edit makefile if needed
2) compile
   make lib
   su - root
   cd /home/hyuan/cpp/rabbitmq_ex/amqpcpp_lib
   make install

******************
Following is the link of example video
https://youtu.be/BlFQi7Hsn8I
