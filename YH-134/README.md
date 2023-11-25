This example is going to discuss a new library, libfuse - Filesystem in userspace. 

There are many academy terminology to define, explain and specify filesystem, in short words, Filesystem is Operating System manage how to store and retrieve your data using storage device.
There are many type of storages devices, like disk drive, optical devices (CD ROM), Tape, USB, SSD and RAM devices.
it is one of very importantly part of operating system kernel.
* https://en.wikipedia.org/wiki/File_system

Filesystem in Userspace (FUSE) is a software interface for Unix and Unix-like computer operating systems that lets non-privileged users create 
their own file systems without editing kernel code.
* https://en.wikipedia.org/wiki/Filesystem_in_Userspace

Filesystem operation, usually it requires super user privilege, 
Example: to mount/unmount disk. partition disk, handle logic volume, volume group you have to log on as super user like root.
libfuse is able to let non-privileged users to handle such filesystem operation. it is bridge between API level and kernel level.

FUSE is particularly useful for writing virtual file systems API. 
VFS - Virtual File system, is an abstract layer on top of a more concrete file system. 
The purpose of a VFS is to allow client applications to access different types of concrete file systems in a uniform way
For example, be used to access local and network storage devices transparently without the client application noticing the difference.
* https://en.wikipedia.org/wiki/Virtual_file_system

Linux filesystem concept.
A filesystem is a method for storing and organizing data on a computer. In Linux, the file system is a crucial component of the operating system, responsible for managing data stored on disk and other storage devices.

Linux File System : 
* https://opensource.com/life/16/10/introduction-linux-filesystems
* https://www.scaler.com/topics/linux-tutorial/file-system-of-linux/
* https://opensource.com/life/16/10/introduction-linux-filesystems 
* https://www.geeksforgeeks.org/linux-file-system/
* https://tekrants.files.wordpress.com/2012/05/490px-fuse_structure-svg.png

The vodei of this example link :  https://youtu.be/aMlX2x5N9Ak
