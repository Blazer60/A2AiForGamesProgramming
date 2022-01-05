/**
 * @file Common.cpp
 * @author Ryan Purse
 * @date 31/12/2021
 */


#include "Common.h"

#include <chrono>
#include <random>


std::default_random_engine &getGenerator()
{
    static std::random_device device;
    static std::default_random_engine generator(device());
    return generator;
}

float randomFloat()
{
    static std::uniform_real_distribution<float> distribution(0, 1);
    return distribution(getGenerator());
}

uint32_t randomInt(uint32_t min, uint32_t max)
{
    std::uniform_int_distribution<uint32_t> distribution(min, max);
    return distribution(getGenerator());
}

std::string getUniqueString()
{
    const auto now = std::chrono::system_clock::now();
    const long long time = std::chrono::system_clock::to_time_t(now);
    std::string fileName = std::string(std::ctime(&time));  // ctime() is deprecated but there is no equivalent.
    fileName = fileName.substr(0, fileName.size() - 1);  // Remove the \n
    
    // Remove unwanted characters that can't be used in file names (windows).
    fileName.erase(std::remove(fileName.begin(), fileName.end(), ':'), fileName.end());
    fileName.erase(std::remove(fileName.begin(), fileName.end(), ' '), fileName.end());
    
    // Add a timestamp too in the event that multiple items are saved at once.
    return fileName + std::to_string(now.time_since_epoch().count());
}
