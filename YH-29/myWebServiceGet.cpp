#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <restbed>

using namespace std;
using namespace restbed;

void get_method_handler( const shared_ptr< Session > session )
{
    //perform awesome solutions logic...
    // const auto request = session->get_request();
    string response_body;
    std::string line;
    std::stringstream myStream;
    std::ifstream myFile("player.json");
    if (myFile.is_open())
    {
        while ( getline(myFile, line))
        {
            myStream << line << "\r\n";
        }
        myFile.close();
    }

    response_body = myStream.str();
    session->close( OK, response_body, { { "Content-Length", ::to_string(response_body.length()) }, { "Content-Type", "app
}

void service_ready_handler( Service& )
{
    fprintf( stderr, "The service port 1234 /player is up and running." );
}

int main( const int, const char** )
{

    auto resource = make_shared< Resource >( );
    resource->set_path( "/player" );
    resource->set_method_handler( "GET", get_method_handler );

    auto settings = make_shared< Settings >( );
    settings->set_port( 1234 );
    settings->set_default_header( "Connection", "close" );

    // Service service;
    auto service = make_shared< Service >( );
    service->publish( resource );
    service->set_ready_handler( service_ready_handler );
    service->start( settings );

    return EXIT_SUCCESS;
}
 
