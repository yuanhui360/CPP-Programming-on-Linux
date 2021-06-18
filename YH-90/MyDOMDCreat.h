#ifndef MY_DOM_DOCUMENT_H
#define MY_DOM_DOCUMENT_H

#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>

XERCES_CPP_NAMESPACE_USE

#define toXMLCh(str) XStr(str).unicodeForm()
#define toCh(str) StrX(str).localForm()

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

inline void output_doc(const DOMImplementation* impl, const DOMDocument* doc, char* fileName)
{
   // output doc
   DOMLSSerializer *theWriter = ((DOMImplementationLS*)impl)->createLSSerializer();
   DOMLSOutput     *theOutputDesc = ((DOMImplementationLS*)impl)->createLSOutput();


   DOMConfiguration* theWriterConfig = theWriter->getDomConfig();
   theWriterConfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
   XMLFormatTarget *myFormTarget = new LocalFileFormatTarget(fileName);
   theOutputDesc->setByteStream(myFormTarget);

   theWriter->write(doc, theOutputDesc);
   delete myFormTarget;

   theOutputDesc->release();
   theWriter->release();
}

/*
 * public final class DOMImplementationRegistry
 * Since : DOM Level 3
 * This provides an application with an implementation-independent starting point.
 * DOM implementations may modify this class to meet new security standards or to
 * provide *additional* fallbacks for the list of DOMImplementationSources.
 */

inline int create_dom_document(char* fileName)
{
    int rtn = 0;
    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(
                                 XMLString::transcode("Core"));
    if (impl == NULL)
    {
        std::cout << "Implementation Error !" << std::endl;
        rtn = 4;
        return rtn;
    }
 
    try
    {
       // root element namespace URI.
       // root element name
       // document type object (DTD).

       DOMDocument* doc = impl->createDocument( 0, toXMLCh("bookstore"), 0);
       DOMElement* rootElem = doc->getDocumentElement();

       /*
        * <?xml version="1.0" encoding="UTF-8"?>
        * <bookstore>
        *     <book category="cooking" language="Italian">
        *         <title>Everyday Italian</title>
        *         <author>Bruno Pasquale</author>
        *         <price currency="CAD">102.83</price>
        *     </book>
        * </bookstore>
        */

        DOMElement*  bookElem = doc->createElement(toXMLCh("book"));
        bookElem->setAttribute(toXMLCh("category"), toXMLCh("cooking"));
        bookElem->setAttribute(toXMLCh("language"), toXMLCh("Italian"));
        rootElem->appendChild(bookElem);

        DOMElement*  titleElem = doc->createElement(toXMLCh("title"));
        DOMText*  titleText = doc->createTextNode(toXMLCh("Everyday Italian"));
        titleElem->appendChild(titleText);
        bookElem->appendChild(titleElem);


        DOMElement*  authElem = doc->createElement(toXMLCh("author"));
        DOMText* authText = doc->createTextNode(toXMLCh("Bruno Pasquale"));
        authElem->appendChild(authText);
        bookElem->appendChild(authElem);

        DOMElement*  priceElem = doc->createElement(toXMLCh("price"));
        priceElem->setAttribute(toXMLCh("currency"), toXMLCh("CAD"));
        DOMText* priceText = doc->createTextNode(toXMLCh("102.83"));
        priceElem->appendChild(priceText);
        bookElem->appendChild(priceElem);

        /*
         * Document Object Model Load and Save (DOMLSxxx)
         * DOM Level 3 will provide an API for loading XML source documents into a DOM
         * representation and for saving a DOM representation as a XML document
         *
         * DOMLSParser, DOMLSSerializer, DOMLSInput, DOMLSOutput
         */

        output_doc(impl, doc, fileName);
        doc->release();
    }
    catch (...)
    {
       std::cout << "An error occurred creating the document" << std::endl;
       rtn = 3;
    }

    return rtn;
}

#endif
