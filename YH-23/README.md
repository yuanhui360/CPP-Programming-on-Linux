MongoDB is one of most popular used nonSQL database since big data and cloud application is more and more common these days. 
This example demonstrated how to connect to MongoDB through C++ program on Linux.

Following is the link of video:

https://youtu.be/GKTItIcOb60

some viewer feed back :
when make shows error: bsoncxx::v_noabi::document::element::get_utf8() is deprecated
solution: use document::element::get_string() instead

The video example using Mongo CXX version rmongo-cxx-driver-r3.4.2, 
Downloaded and checked new version, rmongo-cxx-driver-r3.10.1, bsoncxx::v_noabi::document::element::get_utf8() has been removed.

Created new program : mongo_query.cpp <-- use document::element::get_string() instead
