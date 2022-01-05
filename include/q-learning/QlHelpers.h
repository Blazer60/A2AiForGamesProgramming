/**
 * @file QLearning.h
 * @author Ryan Purse
 * @date 31/12/2021
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include <array>
#include <map>

enum class action
        : unsigned int {
    North, NorthEast, East, SouthEast, South, SouthWest, West, NorthWest, Count
};

/**
 * @brief Defines the direction to the goal.
 */
enum class direction
        : char {
    NoDirection = '0', Left = '1', Up = '1', Right = '2', Down = '2'
};

typedef std::string                                           State;
typedef std::array<float, static_cast<size_t>(action::Count)> ActionValues;
typedef std::map<State, ActionValues>                         QTable;
