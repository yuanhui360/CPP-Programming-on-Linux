In this example, it's going to go back and discuss MongoDB, there are few videos early have discussed how to install MongoDB on Linux and using C++ to connect, insert and query database collections.
recently upgrade MongoCCC, compile got error that get_utf8() is deprecated and solution use document::element::get_string() instead it because new version r3.10.1 completely removed document.element.get_utf8() wihch exist in mongo-cxx-driver-r3.4.2, 
The example demonstrated how to upgrade previous program to version r3.10.1 and update MongoDB connection from remote using TLS.

Video YH-144 is going to discuss MongoDB Atlas Cloud. MongoDB is a source-available, cross-platform, document-oriented database program. 
Classified as a NoSQL database that store data in JSON-like Document. MongoDB Atlas is the multi-cloud developer data platform. 
An integrated suite and The most advanced cloud database service with unmatched data distribution and mobility across AWS, Azure, and Google Cloud.
and data services to accelerate and simplify how you build with data. The advantage of Atlas that it's possible to run applications anywhere

the C++ program mongo_atlas_connect.cpp will be example of connecting to MongoDB atlas Cloud Database.

Below ae link of MongoDB example and related video links:
* https://youtu.be/agThSGo2EZE  --- MongoDB Connection From Remote Using TLS
* https://youtu.be/Nq1bSyN8XsI  --- Query MongoDB through Restful Web Service
* https://youtu.be/GKTItIcOb60  --- Connect to MongoDB
* https://youtu.be/cdHg-PFSqHQ  ---  Query data from  MongoDB and Parse BSON Data
* https://youtu.be/CTmahLPb-wQ  --- Connect to MongoDB Atlas Cloud Database
