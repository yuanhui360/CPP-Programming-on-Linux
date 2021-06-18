#ifndef MY_DOMLS_PARSER_H
#define MY_DOMLS_PARSER_H

#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "DOMPrintFilter.hpp"

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

/*
 * DOMLSSerializer
 * 1) A DOMLSParser instance is obtained from the DOMImplementationLS interface by invoking
 *    its createLSParser method
 * 2) DOMImplementationLSMode :
 *     MODE_SYNCHRONOUS  = 1
 *     MODE_ASYNCHRONOUS = 2
 * 3) optionally can set some features on this serializer
 * 4) optionally can implement filter DOMLSSerializerFilter
 *
 * DOMLSInput
 * 1) This interface represents a single input source for an XML entity.
 * 2) This interface allows an application to encapsulate information about an input source
 * 3) in a single object, which may include:
 *       a public identifier,
 *       a system identifier,
 *       a byte stream (possibly with a specified encoding), and/or a character stream.
 *
 * DOMLSOutput
 *
 */

inline void dom_view_doc(const DOMDocument* doc)
{
    // output doc
    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(myFeature);
    DOMLSSerializer *theWriter = ((DOMImplementationLS*)impl)->createLSSerializer();
    DOMLSOutput     *theOutputDesc = ((DOMImplementationLS*)impl)->createLSOutput();

    DOMConfiguration* theWriterConfig = theWriter->getDomConfig();
    theWriterConfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    XMLFormatTarget *myFormTarget = new StdOutFormatTarget();
    theOutputDesc->setByteStream(myFormTarget);

    /*
     *   optionally you can set some filter
     */

    DOMPrintFilter* myFilter = new DOMPrintFilter(DOMNodeFilter::SHOW_ELEMENT   |
                                               DOMNodeFilter::SHOW_ATTRIBUTE |
                                               DOMNodeFilter::SHOW_DOCUMENT_TYPE);
    theWriter->setFilter(myFilter);

    theWriter->write(doc, theOutputDesc);

    delete myFormTarget;
    theOutputDesc->release();
    theWriter->release();
}


/*
 * public final class DOMImplementationRegistry
 *
 * Since : DOM Level 3
 * This provides an application with an implementation-independent starting point.
 * DOM implementations may modify this class to meet new security standards or to
 * provide *additional* fallbacks for the list of DOMImplementationSources.
 *
 * DOMLSParser:
 * DOMImplementationLSMode :
 *     MODE_SYNCHRONOUS  = 1
 *     MODE_ASYNCHRONOUS = 2
 *
 * ACTIONS:
 *     ACTION_APPEND_AS_CHILDREN
 *     ACTION_REPLACE_CHILDREN
 *     ACTION_INSERT_BEFORE
 *     ACTION_INSERT_AFTER
 *     ACTION_REPLACE
 *
 */

inline int func_domls_parse(char* myURI)
{
    int rtn = 0;

    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(toXMLCh("LS"));
    DOMLSParser* parser = ((DOMImplementationLS*)impl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

    // optionally you can set some features on this builder
    if (parser->getDomConfig()->canSetParameter(XMLUni::fgDOMValidate, true))
        parser->getDomConfig()->setParameter(XMLUni::fgDOMValidate, true);
    if (parser->getDomConfig()->canSetParameter(XMLUni::fgDOMNamespaces, true))
        parser->getDomConfig()->setParameter(XMLUni::fgDOMNamespaces, true);
    if (parser->getDomConfig()->canSetParameter(XMLUni::fgDOMDatatypeNormalization, true))
        parser->getDomConfig()->setParameter(XMLUni::fgDOMDatatypeNormalization, true);

    // optionally you can implement your DOMErrorHandler (e.g. MyDOMErrorHandler)
    // and set it to the builder
    ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    parser->getDomConfig()->setParameter(XMLUni::fgDOMErrorHandler, errHandler);

    DOMDocument *doc = nullptr;

    try {
        doc = parser->parseURI(myURI);
        dom_view_doc(doc);
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        std::cout << "Exception message is: \n" << message << "\n";
        XMLString::release(&message);
        return -1;
    }
    catch (const DOMException& toCatch) {
        char* message = XMLString::transcode(toCatch.msg);
        std::cout << "Exception message is: \n" << message << "\n";
        XMLString::release(&message);
        return -1;
    }
    catch (...) {
        std::cout << "Unexpected Exception \n" ;
        return -1;
    }

    parser->release();
    delete errHandler;

    return rtn;
}

#endif                     
