#ifndef MY_PARSER_HANDLER_H
#define MY_PARSER_HANDLER_H

#include <boost/json.hpp>
#include <boost/json/basic_parser_impl.hpp>

#include <iomanip>
#include <iostream>

using namespace boost::json;

class MyParserHandler
{
public:
    constexpr static std::size_t max_object_size = std::size_t(-1);
    constexpr static std::size_t max_array_size = std::size_t(-1);
    constexpr static std::size_t max_key_size = std::size_t(-1);
    constexpr static std::size_t max_string_size = std::size_t(-1);

    bool on_document_begin( error_code& );
    bool on_document_end( error_code& );
    bool on_object_begin( error_code& );
    bool on_object_end( std::size_t, error_code& );
    bool on_array_begin( error_code& );
    bool on_array_end( std::size_t, error_code& );
    bool on_key_part( string_view, std::size_t, error_code& );
    bool on_key( string_view, std::size_t, error_code& );
    bool on_string_part( string_view, std::size_t, error_code& );
    bool on_string( string_view, std::size_t, error_code& );
    bool on_number_part( string_view, error_code& );
    bool on_int64( std::int64_t, string_view, error_code& );
    bool on_uint64( std::uint64_t, string_view, error_code& );
    bool on_double( double, string_view, error_code& );
    bool on_bool( bool, error_code& );
    bool on_null( error_code& );
    bool on_comment_part(string_view, error_code&);
    bool on_comment(string_view, error_code&);

    MyParserHandler();
    ~MyParserHandler();
private:
    int numSeq;
};

#endif
    
