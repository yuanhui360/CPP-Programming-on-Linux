#ifndef MY_DOMLS_PARSER_H
#define MY_DOMLS_PARSER_H

#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/sax/HandlerBase.hpp>

XERCES_CPP_NAMESPACE_USE

#define toXMLCh(str) XStr(str).unicodeForm()
#define toCh(str) StrX(str).localForm()

static const XMLCh  myFeature[]= { chLatin_L, chLatin_S, chNull };

class StrX
{
public :
    StrX(const XMLCh* const toTranscode)
    {
        fLocalForm = XMLString::transcode(toTranscode);
    }

    ~StrX()
    {
        XMLString::release(&fLocalForm);
    }

    const char* localForm() const
    {
        return fLocalForm;
    }

private :
    char*   fLocalForm;
};

class XStr
{
public :
    XStr(const char* const toTranscode)
    {
        fUnicodeForm = XMLString::transcode(toTranscode);
    }

    ~XStr()
    {
        XMLString::release(&fUnicodeForm);
    }

    const XMLCh* unicodeForm() const
    {
        return fUnicodeForm;
    }

private :
    XMLCh*   fUnicodeForm;
};

#endif
