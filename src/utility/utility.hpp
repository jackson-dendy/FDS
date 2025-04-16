#pragma once 
#include <iostream>
#include <string>

class loading_bar{
    public:

    loading_bar(double steps, std::string name);

    void step_bar();

    int barWidth = 60;

   

    double steps;
    int current = 0;
    double size;
    double percent = 0;
    std::string name;
};