#include "utility.hpp"

loading_bar::loading_bar(double steps, std::string name){
    this -> steps = steps;

    this -> size = 1/steps;

    this -> name = name;


}

void loading_bar::step_bar(){
    
    
   

    std::cout << name << ": [";
    percent += size; 
    int pos = barWidth * percent;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos){
            std::cout << "=";
        }

        else if(i == pos) std::cout <<">";

        else std::cout << " ";
    }

    if(percent > 1.00-size){
        std::cout << "] " << 100 << " %\r";
        std::cout << std::endl;
    }

    else{
        std::cout << "] " << int(percent * 100.0) << " %\r";
    }
    
    
    std::cout.flush();
    
};