/**
 * @file Common.h
 * @author Ryan Purse
 * @date 31/12/2021
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include <chrono>
#include <cstdint>
#include <functional>

/**
 * @brief Returns a random float between 0 and 1.
 * @return [0, 1]
 */
[[nodiscard]] float randomFloat();

/**
 * @brief Returns a random unsigned int between the specified range.
 * @param min - The minimum number that can be returned.
 * @param max - The maximum number that can be returned.
 * @return [min, max]
 */
[[nodiscard]] uint32_t randomInt(uint32_t min=0, uint32_t max=1);

/**
 * @returns A unique string based on the time and date.
 */
[[nodiscard]] std::string getUniqueString();


/**
 * @brief Counts the time in micro seconds to complete a function.
 * @tparam ReturnType - The type that is returned by Function func.
 * @tparam Function - The type of func (Can be auto deduced).
 * @tparam ...Args - The type of each args for func.
 * @param func - The function that you want to time.
 * @param args - The arguments that you want to pass to func.
 * @returns [time in microseconds, ReturnType]
 */
template<typename ReturnType, typename Function, typename ...Args>
[[nodiscard]] std::pair<long long, ReturnType> timeIt(const Function &func, const Args &...args)
{
    const auto startTime= std::chrono::high_resolution_clock::now();
    const auto returnValue = func(args...);
    const auto endTime = std::chrono::high_resolution_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    return { time.count(), returnValue };
}