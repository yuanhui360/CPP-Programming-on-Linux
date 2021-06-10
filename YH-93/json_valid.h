#ifndef JSON_PRINT_H
#define JSON_PRINT_H

#include <boost/json.hpp>
#include <iomanip>
#include <iostream>
#include <fstream>

#include <boost/json.hpp>

namespace json = boost::json;

inline void pretty_print( json::value const& jv, std::string* indent = nullptr )
{
    std::string indent_;
    if(! indent)
        indent = &indent_;
    switch(jv.kind())
    {
    case json::kind::object:
    {
        std::cout << "{" << std::endl;
        indent->append(4, ' ');
        auto const& obj = jv.get_object();
        if(! obj.empty())
        {
            size_t objLen = obj.size();
            size_t i = 0;
            for (auto it = obj.begin(); it != obj.end(); it++ )
            {
                i++;
                std::cout << *indent << json::serialize(it->key()) << " : ";
                pretty_print(it->value(), indent);
                if ( i < objLen )
                    std::cout << "," << std::endl;;
            }
        }
        std::cout << std::endl;
        indent->resize(indent->size() - 4);
        std::cout << *indent << "}";
        break;
    }

    case json::kind::array:
    {
        std::cout << "[" << std::endl;;
        indent->append(4, ' ');
        auto const& arr = jv.get_array();
        if(! arr.empty())
        {
            for(auto it = arr.begin(); it != arr.end(); it++)
            {
                std::cout << *indent;
                pretty_print( *it, indent);
                if(*it != arr.back())
                    std::cout << "," << std::endl;
            }
        }
        std::cout << std::endl;
        indent->resize(indent->size() - 4);
        std::cout << *indent << "]";
        break;
    }

    case json::kind::string:
    {
        std::cout << json::serialize(jv.get_string());
        break;
    }

    case json::kind::uint64:
        std::cout << jv.get_uint64();
        break;

    case json::kind::int64:
        std::cout << jv.get_int64();
        break;

    case json::kind::double_:
        std::cout << jv.get_double();
        break;

    case json::kind::bool_:
        if(jv.get_bool())
            std::cout << "true";
        else
            std::cout << "false";
        break;

    case json::kind::null:
        std::cout << "null";
        break;
    }
  
    if(indent->empty())
        std::cout << std::endl;
}

inline bool json_stream_parse(const char* filename)
{
    std::string line;
    int lineNum;
    bool rc = true;

    json::parse_options myOpt;
    myOpt.allow_comments = true;
    json::error_code ec;
    json::stream_parser myParser(json::storage_ptr(), myOpt);

    try
    {
        std::ifstream myFile(filename);
        if (myFile.is_open())
        {
            lineNum = 0;
            while( std::getline( myFile, line ) ) {

                lineNum++;
                myParser.write( line, ec );
                if( ec ) {
                    rc = false;
                    std::cerr << "Parse Error at line : " << lineNum
                              << " <- " << ec.message() << std::endl;
                    break;
                }
                line.clear();
            }
            myFile.close();
            if ( rc ) {
                myParser.finish(ec);
                if ( ec ) {
                    rc = false;
                    std::cerr << "Parse Finish Error : " << " <- " << ec.message() << std::endl;
                }
            }
        }
    }
    catch(std::exception const& e)
    {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    if ( rc ) {
        pretty_print(myParser.release());
    }

    /*
        std::cout << myParser.release() << std::endl;
    */

    return rc;
}


#endif
