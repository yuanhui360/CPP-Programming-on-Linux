RabbitMQ using Exchange, Routing Key and Queue to publish and consume message.

Following is the Standard RabbitMQ message flow
1) The producer publishes a message to the exchange.
2) The exchange receives the message and is now responsible for the routing of the message.
3) Binding must be set up between the queue and the exchange. In this case, we have bindings to two different queues from the exchange. 
4) The exchange routes the message into the queues.
5) The messages stay in the queue until they are handled by a consumer.
6) The consumer handles the message.

RabbitMQ Exchange has 4 types.

Direct exchange
Fanout exchange
Topic exchange
Headers exchange

Direct Exchange
A direct exchange delivers messages to queues based on the message routing key. A direct exchange is ideal for the unicast routing of 
messages (although they can be used for multicast routing as well). Here is how it works:

1) A queue binds to the exchange with a routing key K
2) When a new message with routing key R arrives at the direct exchange, the exchange routes it to the queue if K = R

This short vide focus on how to use RabbitMQ Direct exchange publishing and consuming messages.

Video link of this example : 

https://youtu.be/S9zMmrkSA1Y

https://youtu.be/BlFQi7Hsn8I
