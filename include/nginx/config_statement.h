#ifndef NGINX_CONFIG_STATEMENT_H
#define NGINX_CONFIG_STATEMENT_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace nginx {

class config;

// The parsed representation of a single config statement.
class config_statement {
 public:
  virtual std::string to_string(int depth);
  std::vector<std::string> tokens_;
  std::unique_ptr<nginx::config> child_block_;
};

}

#endif // NGINX_CONFIG_STATEMENT_H