RabbitMQ is a messaging broker. It accepts messages from publishers, routes them and, if there were queues to route to, 
stores them for consumption or immediately delivers to consumers,the consumer is an application (or application instance) 
that consumes messages from specified queue. 

Message Header Flag could be

************
#define AMQP_BASIC_CLASS (0x003C) // basic class id @internal 60
#define AMQP_BASIC_CONTENT_TYPE_FLAG (1 << 15)
#define AMQP_BASIC_CONTENT_ENCODING_FLAG (1 << 14)
#define AMQP_BASIC_HEADERS_FLAG (1 << 13)
#define AMQP_BASIC_DELIVERY_MODE_FLAG (1 << 12)
#define AMQP_BASIC_PRIORITY_FLAG (1 << 11)
#define AMQP_BASIC_CORRELATION_ID_FLAG (1 << 10)
#define AMQP_BASIC_REPLY_TO_FLAG (1 << 9)
#define AMQP_BASIC_EXPIRATION_FLAG (1 << 8)
#define AMQP_BASIC_MESSAGE_ID_FLAG (1 << 7)
#define AMQP_BASIC_TIMESTAMP_FLAG (1 << 6)
#define AMQP_BASIC_TYPE_FLAG (1 << 5)
#define AMQP_BASIC_USER_ID_FLAG (1 << 4)
#define AMQP_BASIC_APP_ID_FLAG (1 << 3)
#define AMQP_BASIC_CLUSTER_ID_FLAG (1 << 2)

**** Consumer Steps *******

Using librabbitmq.so for consumer API steps

/*
 *  Step 1 : setup connection
 *     1) amqp_new_connection()
 *     2) amqp_tcp_socket_new()
 *     3) amqp_socket_open()
 *     4) amqp_login()
 *     5) amqp_channel_open()
 */

/*
 *  Step 2 : Declare Queue and register consumer tag
 *         1) amqp_queue_declare()   <- Declare queue
 *         2) amqp_basic_consume()   <- Start Consumer
 */

/*
 *  Step 3 : Loop Wait for consume a message (wait for basic delivery)
 *  LOOP:
 *     amqp_consume_message()  <-- Allocate memeory for envelope object
 */

Envelope object contain following information
/*
 * Envelope object include following information
 *   1) Channel ID
 *   2) Consumer Tag
 *   3) Delivery Tag
 *   4) Exchange Name
 *   5) Routing Key
 *   6) Redelivered (true/false)
 *   7) Message Info
 */

Message object contain following information
/*
 *   amqp_message_t object include following information
 *      1) Basic Header information
 *      2) Special Header information
 *      3) Message Body Information
 */

Example video link : https://youtu.be/JbjVZdaslgA
