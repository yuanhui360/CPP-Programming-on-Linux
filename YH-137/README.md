Libfuse is very useful library, using libfuse, it's possible to crate a new layer of file system and customize special logic on this file system.
Especially in today, more and more VM and cloud storage been inplemented, libfuse API will be more and more useful.
Our example in this video is concentrated for concept only, usually in real libfuse related API, it's pretty complex.
The example of this video, is demon a libfuse low level API.
As we know libfuse offers 2 types API, high-level API and Low-level API.
1) "high-level", synchronous API, 
   work with file names and paths
2) "low-level" asynchronous API. 
   work with inodes ( Index Node)

This is one of very useful libfuse documentation page : https://libfuse.github.io/doxygen/index.html
here is Index Node specification : https://docs.rackspace.com/docs/what-are-inodes-in-linux
The video reference of this example can be found on my YouTube Channel : https://youtu.be/1hWWpttIGIQ

