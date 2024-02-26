#include <iostream>
#include <map>
#include <string>

class MyClass
{
  public:
    static void staticMethod1()
    {
        std::cout << "Static Method 1 called" << std::endl;
    }

    static void staticMethod2()
    {
        std::cout << "Static Method 2 called" << std::endl;
    }

    static void staticMethod3()
    {
        std::cout << "Static Method 3 called" << std::endl;
    }
};

int main()
{
    // Create a map of string (method name) to member function pointer
    std::map<std::string, void (MyClass::*)()> methodMap;

    // Populate the map with static method names and their corresponding pointers
    methodMap["method1"] = &MyClass::staticMethod1;
    methodMap["method2"] = &MyClass::staticMethod2;
    methodMap["method3"] = &MyClass::staticMethod3;

    // Call a method using the map
    std::string methodName = "method2";
    if (methodMap.find(methodName) != methodMap.end())
    {
        // Call the method through the member function pointer
        (MyClass().*methodMap[methodName])();
    }
    else
    {
        std::cout << "Method not found" << std::endl;
    }

    return 0;
}