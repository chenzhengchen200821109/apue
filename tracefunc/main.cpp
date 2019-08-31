#include <iostream>
#include <string>

int Subroutine()
{
    //std::cout << "I am a subroutine" << std::endl;
    return 0;
}

class A
{
    public:
        A() 
        {
            //std::cout << "A:A()" << std::endl;
        }
        ~A()
        {
            //std::cout << "~A::A()" << std::endl;
        }
        int get() const
        {
            Subroutine();
            return 0;
        }
};

int main()
{
    //std::cout << "Hello World" << std::endl;

    A a;
    A* pa = new A;
    std::string str;
    Subroutine();
    //std::cout << a.get() << std::endl;

    str += "Hello, World";
    pa->get();
    delete pa;
    return 0;
}
