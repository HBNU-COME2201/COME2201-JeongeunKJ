#pragma once
#include "base_decorator.hpp"

class SimpleDecorator2: public BaseDecorator
{
public:
    SimpleDecorator2(CAgent* agent):BaseDecorator(agent){};
    
public:
    virtual bool detect(CAgent* pTarget){
        std::cout << "Simple Decorator2" << std::endl;
        return m_wrappee->detect(pTarget);
    };
};