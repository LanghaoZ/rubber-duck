#ifndef NGINX_LOCATION_H
#define NGINX_LOCATION_H

#include <iostream>
#include "config.h"
#include "config_statement.h"

namespace nginx {

/**
 * 
 * struct representation of location directive
 * 
 * location <path> <handler> {
 *   root <root>;
 * }
 * 
 */
struct location {
  std::string path;
  std::string handler;
  std::string root;
};

} // namespace nginx

#endif // NGINX_LOCATION_H