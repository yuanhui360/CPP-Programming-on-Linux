// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <iostream>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>

#include "MyDOMDCreat.h"

XERCES_CPP_NAMESPACE_USE

/*
 *  For coding DOM, SAX, or SAX2, your application must initialize
 *  the Xerces system before using the API, and terminate it after you are done
 *
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

int main(int argc, char* argv[])
{
    int errorCode = 0;

    if (argc != 2)
    {
        std::cout << "Usage: MyDOMDCreat <XML Out File> " << std::endl;
        return 1;
    }

    // Initialize the XML4C2 system.
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch(const XMLException& ec)
    {
        std::cout << "  Exception message:" << toCh(ec.getMessage()) << std::endl;
        return 1;
    }

   {
       errorCode = create_dom_document(argv[1]);
   }

   XMLPlatformUtils::Terminate();
   return errorCode;
}
