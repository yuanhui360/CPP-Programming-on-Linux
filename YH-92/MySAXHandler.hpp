#ifndef MY_SAX_PARSER_H
#define MY_SAX_PARSER_H

#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/HandlerBase.hpp>

using namespace std;
using namespace xercesc;

class MySAXHandler : public HandlerBase {
public:
    MySAXHandler();
    ~MySAXHandler();

    void startElement(const XMLCh* const name, AttributeList& attributes); // start of an element.
    void endElement(const XMLCh* const name); //  end of an element.
    void characters(const XMLCh* const chars, const XMLSize_t  length); //  character data between element.
    void ignorableWhitespace(const XMLCh* const chars, const XMLSize_t length);

    /*
     * virtual void error(const SAXParseException& exc);   <- recoverable parser error.
     * virtual void warning(const SAXParseException& exc); <- parser warning.
     */

    void fatalError(const SAXParseException&); // Report a fatal XML parsing error.
private:
    int numSeq;
};

#endif
