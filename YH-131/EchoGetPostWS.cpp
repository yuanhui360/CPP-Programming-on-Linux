#include <iostream>
#include "EchoGetPostWS.h"

using namespace std;
using namespace restbed;

int ws_port;
std::string ws_path;

void post_method_handler( const shared_ptr< Session > session )
{
    const auto request = session->get_request();
    int content_length = 0;
    request->get_header( "Content-Length", content_length, 0 );
    std::cout << "HTTP POST Request received  content_length : " << content_length << std::endl;
    session->fetch( content_length,
                    []( const shared_ptr< Session > session, const Bytes & body )
    {
        const auto request = session->get_request();
        int content_length = 0;
        request->get_header( "Content-Length", content_length, 0 );

        std::stringstream myStream;
        myStream << "{\r\n";
        myStream << "    'WS port' : '" << ws_port << "',\r\n    'WS path' : '" << ws_path << "',\r\n";
        myStream << "    'http_request' : 'POST',\r\n";
        myStream << "    'Content-Length' : '" << content_length << "',\r\n";
        myStream << "    " << std::string( body.begin( ), body.end( ) ) << "\r\n}";
        std::cout << std::string( body.begin( ), body.end( ) ) << std::endl;

        // parseBodyToBsonDoc(bodyStr);
        std::string bodyStr = myStream.str();
        std::string response_body = "Body-Length is : " + std::to_string(bodyStr.length());
        session->close( OK, bodyStr, { { "Content-Length", std::to_string(bodyStr.length()) } } );
    } );

}

void get_method_handler( const shared_ptr< Session > session )
{
    //perform get solutions logic...
    const auto request = session->get_request();
    int content_length = 0;
    request->get_header( "Content-Length", content_length, 0 );
    std::cout << "HTTP Get Request received  content_length : " << content_length << std::endl;

    std::string response_body;
    std::stringstream myStream;
    std::string paramName = request->get_query_parameter("Name");
    std::string paramGender = request->get_query_parameter("Gender");

    myStream << "{\r\n";
    myStream << "    'WS port' : '" << ws_port << "',\r\n    'WS path' : '" << ws_path << "',\r\n";
    myStream << "    'http_request' : 'GET',\r\n    'Content_Length' : '" << content_length << "',\r\n";
    if ( paramName.length() > 0 ) {
        myStream << "    'Name' : '" << paramName;
        std::cout << "{ Name : " << paramName;
    }
    if ( paramGender.length() > 0 ) {
        if ( paramName.length() > 0 ) {
            std::cout << ", ";
            myStream << "',\r\n";
        }
        std::cout << "Gender : " << paramGender << " }" << std::endl;
        myStream << "    'Gender' : '" << paramGender;
    }
    myStream << "'\r\n}";

    response_body = myStream.str();
    session->close( OK, response_body, { { "Content-Length", ::to_string(response_body.length()) }, { "Content-Type", "application/json
}


void put_method_handler( const shared_ptr< Session > session )
{
    //perform get solutions logic...
    std::string response_body;
    std::stringstream myStream;
    int content_length = 0;
    const auto request = session->get_request();
    request->get_header( "Content-Length", content_length, 0 );
    std::cout << "HTTP PUT Request received  Content_Length : " << content_length << std::endl;
    myStream << "{\r\n";
    myStream << "    'WS port' : '" << ws_port << "','\r\n WS path : '" << ws_path << "','\r\n";
    myStream << "    'http_request' : 'PUT',\r\n   'Content_Length' : '" << content_length << "'}\r\n";
    response_body = myStream.str();
    session->close( OK, response_body, { { "Content-Length", ::to_string(response_body.length()) }, { "Content-Type", "application/json
}

void service_ready_handler( Service& )
{
    // std::unique_ptr<mongocxx::instance>myInst = make_unique<mongocxx::instance>();
    std::cout << "The service port " << ws_port << " " << ws_path << " is up and running." << std::endl;
}

/*
 * myResource->set_method_handler("POST", post_method_handler);
 */

int main(int argc, char* argv[])
{

    if (argc != 3 )
    {
        std::cout << "Usage: " << argv[0] << " <port number> <WS path>" << std::endl;
        std::cout << "Example : " << argv[0] << " 1234 /MyEchoWS" << std::endl;
        return (EXIT_FAILURE);
    }

    ws_port = atoi(argv[1]);
    ws_path = std::string(argv[2]);

    std::shared_ptr<restbed::Resource> myResource = make_shared< restbed::Resource >( );
    myResource->set_path(ws_path);
    myResource->set_method_handler("GET", get_method_handler);
    myResource->set_method_handler("POST", post_method_handler);
    myResource->set_method_handler("PUT", put_method_handler);

    std::shared_ptr<restbed::Settings> mySettings = make_shared< restbed::Settings >( );
    mySettings->set_port(ws_port);
    mySettings->set_default_header( "Connection", "close" );

    std::shared_ptr<restbed::Service> myService = make_shared< restbed::Service >();
    myService->publish(myResource);
    myService->set_ready_handler( service_ready_handler );
    myService->start(mySettings);

    return EXIT_SUCCESS;
}
