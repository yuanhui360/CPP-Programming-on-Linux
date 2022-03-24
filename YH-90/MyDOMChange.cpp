#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <iostream>
#include <string>
#include "MyDOMChange.h"

using namespace std;
using namespace xercesc;

/*
 *  XMLPlatformUtils::Initialize();
 *
 *  {
 *      do something
 *  }
 *
 *  XMLPlatformUtils::Terminate();
 *
 ****************************************
 *  Conversion between char* (UTF-8) and XMLCh* (UTF-16)
 *
 *  char*  XMLString::transcode( const XMLCh *const )
 *  XMLCh* XMLString::transcode( const char  *const )
 *
 *  XMLString::release(char**)
 *  XMLString::release(XMLCh**)
 *
 */

int main (int argc, char* argv[]) {

    int rc = 0;
    if ( argc != 2)
    {
        std::cout << " Usage : MyDOMChange <XML File>" << std::endl;
        return 1;
    }

    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& ec) {
        cout << "Error during initialization! :\n" << toCh(ec.getMessage()) << std::endl;
        return 1;
    }

    XercesDOMParser* parser = new XercesDOMParser();
    parser->setValidationScheme(XercesDOMParser::Val_Always);
    parser->setDoNamespaces(true);    // optional

    ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    parser->setErrorHandler(errHandler);

    try {

        parser->parse(argv[1]);

        DOMDocument* doc = parser->adoptDocument();

        dom_change_doc(doc);
         doc->release();
    }
    catch (const XMLException& ec) {
        std::cout << "XMLException : " << std::endl;
        std::cout << "File : " << ec.getSrcFile()
                  << " -> Line : " << ec.getSrcLine()
                  << " Error Message " << toCh(ec.getMessage()) << std::endl;
        rc = -1;
    }
    catch (const DOMException& ec) {
        std::cout << "DOMException : " << std::endl;
        std::cout << "File : " << argv[1]
                  << " Error : " << toCh(ec.msg)
                  << "--\n" << toCh(ec.getMessage())
                  << std::endl;
        rc = -1;
    }
    catch ( const SAXParseException &ec ) {
        std::cout << "SAXParseException during Parsing\n" ;
        std::cout << "File : " << argv[1]
                  << " -> Line : " << ec.getLineNumber()
                  << " Column : " << ec.getColumnNumber() << " Error Message : "
                  << toCh(ec.getMessage()) << std::endl;
        rc = -1;
    }
    catch (...) {
        std::cout << "Unexpected Exception During Parsing\n" ;
        rc = -1;
    }

    if ( rc == 0 ) {
         std::cout << "---------------------" << std::endl;;
        std::cout << "XML File : " << argv[1] << " <- parse OK " << std::endl;
    }

    delete parser;
    delete errHandler;

    XMLPlatformUtils::Terminate();
    return 0;
}
