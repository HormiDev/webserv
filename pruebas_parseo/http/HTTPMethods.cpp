#include "HTTPMethods.hpp"

/**
 * Converts a string to an HTTPMethod.
 *
 * method: The string to convert.
 * Returns: The corresponding HTTPMethod.
 */
HTTPMethod stringToHTTPMethod(const std::string &method)
{
    if (method == "GET")
        return GET;
    if (method == "POST")
        return POST;
    if (method == "DELETE")
        return DELETE;
    throw std::runtime_error("Invalid HTTP method: " + method);
}