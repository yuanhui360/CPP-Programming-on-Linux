RabbitMQ version, v4.3.0, on Ubuntu 24.04. By default, it operates over unencrypted on TCP port 5672, and these transmissions are sent in plain
text, they aren't secure enough for modern production environments. In this exaample, we’re going to walk through how to enable TLS/SSL encryption to 
secure all communication with RabbitMQ broker.

There are a few different ways to configure RabbitMQ: using the configuration file, the RabbitMQ CLI tools, or by setting environment variables. 
Here we specifically focus on how to modify the configuration 
file to enable TLS/SSL.

Some command used for Testing our example.

*) sudo rabbitmq-diagnostics listeners
*) sudo rabbitmq-diagnostics list_users
*) sudo rabbitmq-diagnostics list_user_permissions test
*) sudo rabbitmq-diagnostics list_exchanges
*) sudo rabbitmqctl list_queues -p my_vhost

video link of this example : https://youtu.be/IYc7bOo0Q94
