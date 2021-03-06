#ifndef NGINX_CONFIG_H
#define NGINX_CONFIG_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map> 

namespace nginx {

class config_statement;
struct location;

const int err_not_found = -1;
const std::string config_port = "port";

// The parsed representation of the entire config.
class config {
public:
  virtual ~config();
  virtual std::string to_string(int depth = 0);
  
  std::vector<std::shared_ptr<nginx::config_statement>> statements_;
  int get_port();
  std::vector<location> get_locations() const;
};

} // namespace nginx

#endif // NGINX_CONFIG_H