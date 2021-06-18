DOMLSParser
1) a new interface introduced by the W3C DOM Level 3.0 Load and Save Specification.
2) provides API for parsing XML documents and building the corresponding
   DOM document tree from various input sources.
3) use the DOMLSInput object to determine how to read XML input.
   a) If there is a character stream available, the parser will read that stream directly;
   b) if not, the parser will use a byte stream, if available;
   c) if neither a character stream nor a byte stream is available,
      the parser will attempt to open a URI connection to the
      resource identified by the system identifier.

https://www.w3schools.com/xml/cd_catalog.xml

Video of this example : https://youtu.be/zOsHns6esUA
