#include "MySAXHandler.hpp"
#include "MySAXParser.h"
#include <iostream>

using namespace std;
using namespace xercesc;

MySAXHandler::MySAXHandler() : numSeq(0) { }
MySAXHandler::~MySAXHandler() {}

void MySAXHandler::startElement(const XMLCh* const name, AttributeList& attributes)
{
    numSeq++;
    cout << numSeq << "\t-> Start : " << toCh(name) << endl;

    XMLSize_t len = attributes.getLength();
    for (XMLSize_t index = 0; index < len; index++) {
        cout << "\t\t-> " << toCh(attributes.getName(index))
             << "="
             << toCh(attributes.getValue(index)) << endl;
    }
}

void MySAXHandler::endElement(const XMLCh* const name)
{
    numSeq++;
    cout << numSeq <<  "\t-> End   : " << toCh(name) << endl;
}

void MySAXHandler::characters(const XMLCh* const data, const XMLSize_t length)
{
    char* myText = XMLString::transcode(data);
    XMLString::trim(myText);
    XMLSize_t myLen = XMLString::stringLen(myText);
    if ( myLen > 0 ) {
        numSeq++;
        std::cout << numSeq << "\t-> Data  : " <<  myText << std::endl;;
    }
    XMLString::release(&myText);
}

/*
    numSeq++;
    std::cout << numSeq << "\t-> Data  : " <<  toCh(data) << std::endl;;

 */

void MySAXHandler::ignorableWhitespace(const XMLCh* const data, const XMLSize_t length)
{
    /*
    numSeq++;
    cout << numSeq <<  "\t-> Space   : " << toCh(data) << endl;
     */
}

void MySAXHandler::fatalError(const SAXParseException& exception)
{
    cout << "Fatal Error: " << toCh(exception.getMessage())
         << " at line: " << exception.getLineNumber()
         << endl;
}
