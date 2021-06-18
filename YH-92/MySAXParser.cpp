#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>

#include <iostream>
#include "MySAXParser.h"
#include "MySAXHandler.hpp"

using namespace std;
using namespace xercesc;

int main (int argc, char* argv[]) {

    if ( argc != 2)
    {
        std::cout << " Usage : MySAXParser <XML File> " << std::endl;
        return 1;
    }

    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Error during initialization! :\n"
             << message << "\n";
        XMLString::release(&message);
        return 1;
    }

    SAXParser* parser = new SAXParser();
    parser->setValidationScheme(SAXParser::Val_Always);
    parser->setDoNamespaces(true);    // optional

    // Customize Docment Handler during parseing
    MySAXHandler* docHandler = new MySAXHandler();
    ErrorHandler* errHandler = (ErrorHandler*) docHandler;

    parser->setDocumentHandler(docHandler);
    parser->setErrorHandler(errHandler);

    try {

        // SAX 1.0 Parser interface's
        // Return is void
        // void parse( const InputSource &source )
        parser->parse(argv[1]);
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
        return -1;
    }
    catch (const SAXParseException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
        return -1;
    }
    catch (...) {
        cout << "Unexpected Exception \n" ;
        return -1;
    }

    delete parser;
    delete docHandler;

    // And call the termination method
    XMLPlatformUtils::Terminate();
    return 0;
}
