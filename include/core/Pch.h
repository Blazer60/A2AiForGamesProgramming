/**
 * @brief CMake 3.16+ Allows for precompiled header which
 * saves on compile time for common, untouched header files.
 * If you don't have CMake 3.16 or greater, this will be used
 * instead.
 * @file Pch.h
 * @author Ryan Purse
 * @date 05/01/2022
 */

#pragma once

#ifdef NO_PCH

// Project Includes
#include "DebugLogger.h"

// STL Includes
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <algorithm>
#include <memory>
#include <numeric>
#include <cstdint>

// Vendors Includes
#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <glew.h>

#endif  // NO_PCH
