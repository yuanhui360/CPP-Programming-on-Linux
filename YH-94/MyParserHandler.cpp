#include "MyParserHandler.h"

using namespace boost::json;

MyParserHandler::MyParserHandler() : numSeq(0) {}
MyParserHandler::~MyParserHandler() {}

bool MyParserHandler::on_document_begin(error_code& ec)
{
    numSeq++;
    std::cout << numSeq << "\t -> on_document_begin()" << std::endl;
    return true;
}

bool MyParserHandler::on_document_end( error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_document_end()" << std::endl;
    return true;
}

bool MyParserHandler::on_object_begin( error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_object_begin()" << std::endl;
    return true;
}

bool MyParserHandler::on_object_end( std::size_t size, error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_object_end() \t: size : " << size << std::endl;
    return true;
}

bool MyParserHandler::on_array_begin( error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_array_begin()" << std::endl;
    return true;
}

bool MyParserHandler::on_array_end( std::size_t size, error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_array_end() \t: size : " << size << std::endl;
    return true;
}

bool MyParserHandler::on_key_part( string_view str, std::size_t size, error_code& ) {
    numSeq++;
    std::cout << numSeq << "\t -> on_key_part() \t: "
              << str << " -> size : " << size << std::endl;
    return true;
}

bool MyParserHandler::on_key( string_view str, std::size_t size, error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_key() \t\t: " << str << " -> size : " << size << std::endl;
    return true;
}

bool MyParserHandler::on_string_part( string_view str, std::size_t size, error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_string_part() \t: "
              << str << " -> size : " << size << std::endl;
    return true;
}

bool MyParserHandler::on_string( string_view str, std::size_t size, error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_string() \t: "
              << str << " -> size : " << size << std::endl;
    return true;
}

bool MyParserHandler::on_number_part( string_view str, error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_string() \t: " << str << std::endl;
    return true;
}

bool MyParserHandler::on_int64( std::int64_t value, string_view str, error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_int64() \t\t: " << str << std::endl;
    return true;
}

bool MyParserHandler::on_uint64( std::uint64_t value, string_view str, error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_uint64() \t\t: " << str << std::endl;
    return true;
}

bool MyParserHandler::on_double( double value, string_view str, error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_double() \t: " << str << std::endl;
    return true;
}

bool MyParserHandler::on_bool( bool val, error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_bool() \t\t: ";
    if ( val )
       std::cout  << "True" << std::endl;
    else
       std::cout  << "False" << std::endl;
    return true;
}

bool MyParserHandler::on_null( error_code& ) {
    numSeq++;
    std::cout << numSeq << "\t -> on_null() \t\t: Null" << std::endl;
    return true;
}

bool MyParserHandler::on_comment_part(string_view str, error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_comment_part() \t: " << str << std::endl;
    return true;
}

bool MyParserHandler::on_comment(string_view str, error_code& ec) {
    numSeq++;
    std::cout << numSeq << "\t -> on_comment() \t: " << str << std::endl;
    return true;
}
