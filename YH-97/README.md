Protocol Buffers Message

The Message interface (structured data) defines methods that let you check,
manipulate, read, or write the entire message, including parsing from and
serializing to binary strings.

*******  Build and compile Steps *****
 1) Create message (structured data) .proto file (ex. message.proto)
 2) proto compile :
    Because you want C++ classes, you use the --cpp_out option
        protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/message.proto
    This generates the following files in your specified destination directory:
         message.pb.h, the header which declares your generated classes.
         message.pb.cc, which contains the implementation of your classes.
 3) Write API using message (object)
 4) Edit Makefile and compile

Example video link :  https://youtu.be/7wWjJ2eYixk
