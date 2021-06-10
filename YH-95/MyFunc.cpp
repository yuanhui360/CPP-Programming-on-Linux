#include <iostream>

char const* myMessage()
{
   return "Message from Function";
}

int const myAdd(int a, int b)
{
   return a+b;
}

class MyClass
{
public:
    MyClass(std::string str) : name(str), mNumber(0.0) {}

    std::string name;

    double getNumber() const { return mNumber; }
    void setNumber(double n) { mNumber = n; }

private:
    double mNumber;
};

#include <boost/python.hpp>

using namespace boost::python;
BOOST_PYTHON_MODULE(MyPyLib)
{
    def("myMessage", myMessage);
    def("myAdd", myAdd);
    class_<MyClass>("MyClass", init<std::string>())
        .def_readwrite("name", &MyClass::name)
        .def("Get", &MyClass::getNumber)
        .def("Set", &MyClass::setNumber)
    ;

}
