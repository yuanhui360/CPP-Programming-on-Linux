#ifndef DOMPrintFilter_HEADER_GUARD_
#define DOMPrintFilter_HEADER_GUARD_

#include <xercesc/dom/DOMLSSerializerFilter.hpp>

XERCES_CPP_NAMESPACE_USE

class DOMPrintFilter : public DOMLSSerializerFilter {
public:

    DOMPrintFilter(ShowType whatToShow = DOMNodeFilter::SHOW_ALL);
    ~DOMPrintFilter(){};

    virtual FilterAction acceptNode(const DOMNode*) const;
    virtual ShowType getWhatToShow() const {return fWhatToShow;};

private:
    // unimplemented copy ctor and assignement operator
    DOMPrintFilter(const DOMPrintFilter&);
    DOMPrintFilter & operator = (const DOMPrintFilter&);

    ShowType fWhatToShow;
};

#endif
