#pragma once

/**
 * @file utils.hpp
 * @author Mathias Tiberghien (mathias.tiberghien@thalesgroup.com)
 * @brief Utils or common functions
 * @version 0.1
 * @date 2023-04-24
 * 
 * @copyright © Thales Services Numériques 2023
 * 
 */

#include <string>

using namespace std;

/// @brief Check if the text is whitespace only or empty string
/// @param text The text to check
/// @return true if the text is whitespace or empty, false otherwise
bool is_whitespace_or_empty(const string& text);