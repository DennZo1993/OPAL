/**
 * @file
 *
 * @author Denis Zobnin
 *
 * @brief Header file declaring interface for various operations with JSON.
 */


#pragma once

#include "json11/json11.hpp"

#include <string>
#include <stdexcept>
#include <fstream>


namespace util {

/// Type of JSON object.
using Json = json11::Json;


/**
 * @brief Simple exception class representing a failure occurred during
 * parsing a JSON file (runtime error).
 */
class JsonParseException : public std::runtime_error {
public:
  /// Inherit constructors.
  using std::runtime_error::runtime_error;
};


/**
 * @brief Read JSON from file, specified by name.
 *
 * @param [in] fileName Name of file to read.
 *
 * @returns Parsed Json object.
 *
 * @throws JsonParseException.
 */
Json JsonFromFile(const std::string &fileName);

/**
 * @brief Read JSON from file, specified by name.
 *
 * See JsonFromFile(std::string).
 */
Json JsonFromFile(const char *fileName);

/**
 * @brief Read JSON from file stream.
 *
 * See JsonFromFile(std::string).
 */
Json JsonFromFile(std::ifstream &inputStream);

/**
 * @brief Read JSON from string (for ex. file contents).
 *
 * @param [in] inputStr String from which data is read.
 *
 * @returns Parsed Json object.
 *
 * @throws JsonParseException.
 */
Json JsonFromString(const std::string &inputStr);

} // namespace util

