In this example, it's going to discuss how to gzip/gunzip using crypto++ library. 

https://en.wikipedia.org/wiki/Gzip
gzip is a file format and a software application used for file compression and decompression. 
it is one of common used data compression format standardized in RFC 1952 specification, and 
it is based on the DEFLATE algorithm, and a lossless compression format

Gzip is widely used for saving data space and speed up network stream data transmission application.
Using Gzip, data can be fully compressed in whole file (.gz), partially embedded into the file (.pdf)
 or combined with other type of file (.tar.gz).
for instance:
1) A GZ file with .gz extension. is whole gzip compressed file 
2) The .tar.gz file. is Unix and Linux's tar utility combine .tar and .gz format file.
   it is very common used for download and install Unix/Linux API and library
   ls -l $HOME/Down*/*.tar.gz
3) the popular used PDF file, is exampl of gzip embedered file format.
   it's page image object data is using gzip format that embeded into the file. 
   you can reference one of eraly video in this channel, Decording PDF File page and Read Decooding Gzip File.
4) The gzip format is used in HTTP compression, a technique used to speed up the sending of HTML and other 
   content on the internet, It is one of the three standard formats for HTTP compression specified in RFC 2616. 

( https://zlib.net/ )
The example of early videos are using zlib library to processing gzip data format. 
zlib is standard library in most Linux distribution, 

In general zlib library is a classic C library, and
Crypto++ is Moden C++ library. You can compare both examples and have your own oponion and make your 
own decision for your project.

YH-50 Read Decooding Gzip File
YH-49 Decording PDF File page

************************
Here On linux we created 2 program :
gzip_compress.cpp <- compress original file to gzip file
gzip_extract.cpp  <- extract gzip file to it's original file

In crypto++ libeary, Gzip object is similar to Encoder object but different.
The similar is they both are considered as Stream Transformation Filter. 
The difference is. 
    Encoder is designed for making not authorized party harder to decoder the original data.
    Gzip is designed for compressing data stream and make it smaller compare to their original data

Below is the link of this example's video on YouTube Channel:
https://youtu.be/fnbOLAuj7yc

