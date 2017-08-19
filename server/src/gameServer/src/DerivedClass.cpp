#include "DerivedClass.h"
#include "buildVersion.h"
#include <iostream>

DerivedClass::DerivedClass(void)
{
    STACK

    std::cout << " DerivedClass::DerivedClass()" << std::endl;
}

DerivedClass::~DerivedClass(void)
{
    STACK

    std::cout << " DerivedClass::~DerivedClass()" << std::endl;
}

void DerivedClass::print(void)
{
    STACK

    MyAppLib::print();

    std::cout << " DerivedClass::print(Hello Worls!!!)" << std::endl;
}
