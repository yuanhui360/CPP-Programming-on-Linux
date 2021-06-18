	
The SAX API for XML parsers was originally developed for Java. Please be aware that there is no standard SAX API for C++, 
and that use of the Xerces-C++ SAX API does not guarantee client code compatibility with other C++ XML parsers.

The SAX API presents a callback based API to the parser. An application that uses SAX provides an instance of a handler 
class to the parser. When the parser detects XML constructs, it calls the methods of the handler class, passing them 
information about the construct that was detected. The most commonly used handler classes are DocumentHandler which is 
called when XML constructs are recognized, and ErrorHandler which is called when an error occurs. The header files for 
the various SAX handler classes are in the xercesc/sax/ directory.

Video of this example : https://youtu.be/RWE3vEP5VpM
