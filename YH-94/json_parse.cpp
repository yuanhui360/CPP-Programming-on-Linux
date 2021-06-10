#include <iostream>
#include <sstream>
#include <fstream>
#include "MyParserHandler.h"

using namespace boost::json;

/*
 *  BASIC_PARSER
 *  A SAX style parser implementation which converts a serialized JSON into a
 *  series of member function calls to a user provided handler. This allows
 *  custom behaviors to be implemented for representing the document in memory.
 *
 *  std::size_t write_some( bool more, char const* data, std::size_t size, error_code& ec);
 *
 */

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: json_parse <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    parse_options opt;     // A structure for select which extensions to enable during parsing.
    opt.allow_comments = true;
    error_code ec;
      /*
     *  <MyParserHandler>
     *  customized parser handler with our of member function
     */
    basic_parser<MyParserHandler> myParser(opt);

    bool valid = true;
    try
    {
        std::ifstream myFile(argv[1]);
        if (myFile.is_open())
        {
            while ( std::getline(myFile, line) ) {
                myParser.basic_parser::write_some(true, line.c_str(), line.length(), ec);
                if ( ec ) {
                    valid = false;
                    std::cerr << ec.message() << std::endl;
                }
                line.clear();
            }
            myFile.close();
            myParser.done();
        }

    }
    catch(std::exception const& e)
    {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

      if( valid )
        std::cerr << "File : " << argv[1] << " <- Validation Success\n";
    else
        std::cerr << "File : " << argv[1] << " <- Validateion Not Success\n";

    return EXIT_SUCCESS;
}
