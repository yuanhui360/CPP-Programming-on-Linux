#include <AMQPcpp.h>
#include <vector>
#include <map>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

/*
 *  Headers Exchange
 *  A headers exchange is designed for routing messages based on multiple header attributes
 *  instead of routing key
 *
 *   1. No routing key required
 *   2. The header attributes format is <key:value>
 *   3. The special argument named "x-match" can have two different values: "any" or "all",
 *      which specify if all headers must match or just one., default value is "all".
 *   4. Headers Exchange is more flexable as it could be an integer or a hash (dictionary)
 *      (it does not have to be a string as the routing key)
 *
 */

using namespace std;

int main (int argc, char** argv) {

    std::string msg_body;

    if (argc != 3 && argc != 4)
    {
        std::cout << "Usage: headers_publish <host:port> <exchange> [queue]" << std::endl;
        std::cout << "Example :" << std::endl;
        std::cout << "headers_publish 192.168.0.103:5672 hdrExchange headers_queue" << std::endl;
        std::cout << "headers_publish 192.168.0.103:5672 hdrExchange" << std::endl;
        return 1;
    }

    /*
     *  [$USERNAME[:$PASSWORD]@]$HOST[:$PORT]/[$VHOST]
     *  "test:test@192.168.0.106:5672/my_vhost"
     */

    std::string conn_str = "test:test@";
    conn_str += std::string(argv[1]);
    conn_str += std::string("/my_vhost");

    std::string ex_name = argv[2];
    std::string rt_key  = "";
    std::string qu_name = "";
    if ( argc == 4 )
        qu_name = argv[3];

    try {

        AMQP amqp(conn_str);

        AMQPExchange * ex = amqp.createExchange(ex_name);
        ex->Declare(ex_name, "headers");

        short my_param = AMQP_AUTODELETE | AMQP_DURABLE;
        ex->setParam(my_param);

        ex->setHeader("Delivery-mode", AMQP_DELIVERY_PERSISTENT);
        ex->setHeader("Content-type", "text/text");
        ex->setHeader("Content-encoding", "UTF-8");

      // loop input Header Key:Value entries
        std::map<std::string, std::string> hdrMap;
        std::vector<std::string>   hdrVec;
        std::string                hdrKeyValStr;

        std::cout << "Header (Key:Value) Start Loop: x-match:any or x-match:all, Finish Loop: end:end" << std::endl;
        hdrMap.clear();
        while(1) {
            hdrKeyValStr.clear();
            hdrVec.clear();
            std::cout << "Please input header <key:Value> : ";
            std::getline(std::cin, hdrKeyValStr);
            split(hdrVec, hdrKeyValStr, boost::is_any_of(":"));
            if (hdrVec.size() != 2) { continue; }
            if (hdrVec[0] == "end" && hdrVec[1] == "end") { break; }
            hdrMap[hdrVec[0]] = hdrVec[1];
        }

        for ( auto it = hdrMap.begin(); it != hdrMap.end(); it++ ) {
            ex->setHeader(it->first, it->second, 1);
        }

        if ( qu_name.length() > 0 ) {
            short qu_param = 0;
            AMQPQueue * qu2 = amqp.createQueue(qu_name);
            qu2->Declare(qu_name, qu_param, hdrMap);
            qu2->Bind( ex_name, rt_key, hdrMap);
        }

        for (;;) {
            std::cout << "Please input Message : ";
            std::getline(std::cin, msg_body);
            if ( msg_body == "end" ) {
                break;
            }
            ex->Publish( msg_body , rt_key);
            msg_body.clear();
        }


    } catch (AMQPException &ec) {
        std::cout << ec.getMessage() << std::endl;
    }

    return 0;

}

   
