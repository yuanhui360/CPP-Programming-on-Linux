#!/usr/bin/python

import MyPyLib
print ("MyPyLib.myMessage() ->", MyPyLib.myMessage())
print ("MyPyLib.myAdd ->", MyPyLib.myAdd( 3, 4))

m1 = MyPyLib.MyClass("This is Hui Test Class constructor")
print ("name =",m1.name)
m1.name = "RE-Assign Message"
print ("name =",m1.name)

m1.Set(17.3)
print("Get() 17.3 ->", m1.Get())
