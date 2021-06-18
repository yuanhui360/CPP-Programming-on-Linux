#include "DOMPrintFilter.hpp"
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLString.hpp>
#include "MyDOMLSParser.h"

static const XMLCh  element_head[]= { chLatin_h, chLatin_e, chLatin_a, chLatin_d, chNull };
static const XMLCh  element_html[]= { chLatin_h, chLatin_t, chLatin_m, chLatin_l, chNull };
static const XMLCh  element_body[]= { chLatin_b, chLatin_o, chLatin_d, chLatin_y, chNull };
static const XMLCh  element_link[]= { chLatin_l, chLatin_i, chLatin_n, chLatin_k, chNull };

DOMPrintFilter::DOMPrintFilter(ShowType whatToShow)
:fWhatToShow(whatToShow)
{}

DOMNodeFilter::FilterAction DOMPrintFilter::
acceptNode(const DOMNode* node) const
{
    if ((getWhatToShow() & (1 << (node->getNodeType() - 1))) == 0)
        return DOMNodeFilter::FILTER_ACCEPT;

    switch (node->getNodeType())
    {
    case DOMNode::ELEMENT_NODE:
        {
            /*
             *  FILTER_ACCEPT: Accept the node.
             *  FILTER_REJECT: Reject the node. The children of this node will also be rejected.
             *  FILTER_SKIP:   Skip this single node. the children of this node will still be considered.
             */

            // for element whose name is "head", skip it
            if (XMLString::compareString(node->getNodeName(), element_html)==0)
                return DOMNodeFilter::FILTER_SKIP;
            if (XMLString::compareString(node->getNodeName(), element_head)==0)
                return DOMNodeFilter::FILTER_REJECT;
            if (XMLString::compareString(node->getNodeName(), element_body)==0)
                return DOMNodeFilter::FILTER_REJECT;
            if (XMLString::compareString(node->getNodeName(), toXMLCh("br"))==0)
                return DOMNodeFilter::FILTER_SKIP;

            if (XMLString::compareString(node->getNodeName(), element_link)==0)
                return DOMNodeFilter::FILTER_REJECT;

            // for rest, accept it
            return DOMNodeFilter::FILTER_ACCEPT;
            break;
        }
    case DOMNode::COMMENT_NODE:
        {
            return DOMNodeFilter::FILTER_REJECT;
            break;
        }
    case DOMNode::TEXT_NODE:
        {
            return DOMNodeFilter::FILTER_REJECT;
            break;
        }
    case DOMNode::DOCUMENT_TYPE_NODE:
        {
            return DOMNodeFilter::FILTER_REJECT;  // no effect
            break;
        }
    case DOMNode::DOCUMENT_NODE:
        {
            return DOMNodeFilter::FILTER_REJECT;  // no effect
            break;
        }
    default :
        {
            return DOMNodeFilter::FILTER_ACCEPT;
            break;
        }
    }

    return DOMNodeFilter::FILTER_ACCEPT;
}
