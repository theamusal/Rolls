#include <iostream>
#include <random>
#include <iostream>
#include <fstream>
#include <cmath>
#include <thread>

int dice;
int rolls;
int min;
int max;
int plus;
double mean;
double stddev;

class Generator {
    std::random_device rd;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;
    double min;
    double max;
public:
    Generator(double mean, double stddev, double min, double max):
        generator(rd()), distribution(mean, stddev), min(min), max(max){}

    double operator ()() {
        while (true) {
            double number = this->distribution(generator);
            if (number >= this->min && number <= this->max)
                return std::round(number);
        }
    }
};

extern "C" {
    int* getrolls(int numberofrolls) {
        int* outputarray = new int[numberofrolls];

        Generator g(mean, stddev, min, max);
        for (int i = 0; i < numberofrolls; i++)
        {
            outputarray[i] = (int)g();
        }
        

        return outputarray;
    }
}

extern "C" {
    void setdice(int argdice, int argrolls, int argplus) {
        dice = argdice;
        rolls = argrolls;
        plus = argplus;
        min = rolls + plus;
        max = dice * rolls + plus;
        mean = (((double)dice + 1) / 2) * rolls + plus;
        stddev = std::sqrt(rolls * ((std::pow(dice, 2) - 1) / 12));
    }
}

extern "C" {
    const char* stats() {
        std::string str =  "Dice: " + std::to_string(dice) + ", Rolls: " + std::to_string(rolls) + ", Plus: " + std::to_string(plus) + ", Mean: " + std::to_string(mean) + ", stddev: " + std::to_string(stddev) + ", min: " + std::to_string(min) + ", max: " + std::to_string(max);
        return str.c_str(); 
    }
}

extern "C" {
    bool writerolls(char* filename, int numberofrolls) {

        std::ofstream results;
        results.open (filename);

        Generator g(mean, stddev, min, max);
        for (int i = 0; i < numberofrolls; i++)
        {
            results << (int)g() + plus << std::endl;
        }
        results.close();

        return true;
    }
}

int main() {
    setdice(10,10, 0);
    int* res = getrolls(10);
    std::cout << stats() << std::endl; 
    for (int i = 0; i < 10; i++)
        std::cout << res[i] << std::endl;
}