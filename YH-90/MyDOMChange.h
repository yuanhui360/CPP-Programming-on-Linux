#ifndef MY_DOM_VIEW_H
#define MY_DOM_VIEW_H

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

inline void view_node(const DOMNode* node)
{
    if ( node->hasAttributes() ) {
        std::cout << "<";
        DOMNamedNodeMap *AttrMap = node->getAttributes();
        for ( XMLSize_t j=0; j<AttrMap->getLength(); j++) {
            std::cout << toCh(AttrMap->item(j)->getTextContent());
            if ( j < AttrMap->getLength() -1 )
                std::cout << ",";
        }
        std::cout << "> ";
    }

    if ( node->hasChildNodes() ) {
        DOMNodeList* childList = node->getChildNodes();
        for (XMLSize_t j = 0; j < childList->getLength(); j++ ) {
            view_node(childList->item(j));
        }
    }
    else {
        std::cout << toCh(node->getTextContent());
    }
}

inline void cust_view_doc(const DOMDocument* doc)
{
    DOMElement*  rootNode = doc->getDocumentElement();
    std::cout << "rootNode Name  : " << toCh(rootNode->getNodeName()) << std::endl;

    DOMNodeList* bookList = doc->getElementsByTagName(toXMLCh("book"));
    std::cout << "Total book Nodes : " << bookList->getLength() << std::endl;

    for ( XMLSize_t i = 0; i < bookList->getLength(); i++ ) {
        std::cout << "Book(" << i << ") ";
        view_node(bookList->item(i));
    }
}

inline void dom_view_doc(const DOMDocument* doc)
{
   // output doc
   DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(
                                 XMLString::transcode("Core"));
   DOMLSSerializer *theWriter = ((DOMImplementationLS*)impl)->createLSSerializer();
   DOMLSOutput     *theOutputDesc = ((DOMImplementationLS*)impl)->createLSOutput();
   DOMConfiguration* theWriterConfig = theWriter->getDomConfig();
   theWriterConfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

   XMLFormatTarget *myFormTarget = new StdOutFormatTarget();
   theOutputDesc->setByteStream(myFormTarget);
   theWriter->write(doc, theOutputDesc);
   delete myFormTarget;

   theOutputDesc->release();
   theWriter->release();
}

inline void view_elem_attribute(const DOMElement* elem)
{
    if (elem->hasAttributes() ) {
        std::cout << "<" << toCh(elem->getTagName()) << " ";
        DOMNamedNodeMap *elemAttrMap = elem->getAttributes();
        for ( XMLSize_t j=0; j<elemAttrMap->getLength(); j++) {
            std::cout << toCh(elemAttrMap->item(j)->getNodeName()) << "=";
            std::cout << toCh(elemAttrMap->item(j)->getTextContent());
            if ( j < elemAttrMap->getLength() - 1)
                std::cout << ", ";
        }
        std::cout << ">" << std::endl;;
    }
}

inline void view_node_attribute(const DOMNode* node)
{
    if ( node->hasAttributes() ) {
        std::cout << "<" << toCh(node->getNodeName()) << " ";
        DOMNamedNodeMap *AttrMap = node->getAttributes();
        for ( XMLSize_t j=0; j<AttrMap->getLength(); j++) {
            std::cout << toCh(AttrMap->item(j)->getNodeName()) << "=";
            std::cout << toCh(AttrMap->item(j)->getTextContent());
            if ( j < AttrMap->getLength() -1 )
                std::cout << ",";
        }
        std::cout << "> " << std::endl;;
    }
}

inline void dom_change_doc(const DOMDocument* doc)
{
    DOMElement*  rootElem = doc->getDocumentElement();
    std::cout << "rootNode Name  : " << toCh(rootElem->getNodeName()) << std::endl;
    DOMElement* childElem = rootElem->getFirstElementChild();
    while (childElem != nullptr ) {
        std::cout << "Tag Name : " << toCh(childElem->getTagName()) << std::endl;
        DOMNode* newNode = childElem->cloneNode(true);
        if ( newNode->hasAttributes() ) {
            view_node_attribute(newNode);
        }

        /*
         * Update Original DOMDocument add attribute on DOMElement object
         */

        childElem->setAttribute(toXMLCh("province"), toXMLCh("ON"));
        DOMNode* childNode = childElem->getFirstChild();
        while( childNode != nullptr ) {
            if ( childNode->hasAttributes() ) {
                view_node_attribute(childNode);
            }
            if ( childNode->getNodeType() == DOMNode::ELEMENT_NODE ) {
                std::cout << "--> Node Name : " << toCh(childNode->getNodeName()) << " : ";
                std::cout << toCh(childNode->getTextContent()) << std::endl;
            }
            childNode = childNode->getNextSibling();
        }
        childElem = childElem->getNextElementSibling();
    }

    std::cout << "----------------------" << std::endl;
    dom_view_doc(doc);

}

#endif

