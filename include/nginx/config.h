#ifndef NGINX_CONFIG_H
#define NGINX_CONFIG_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map> 

namespace nginx {

class config_statement;

const int err_not_found = -1;
const std::string config_port = "port";

struct location {
  std::string value;
  std::string root;
};

struct server {
  std::string handler;
  std::vector<location> locations;
};

// The parsed representation of the entire config.
class config {
  public:
    virtual std::string to_string(int depth = 0);
    std::vector<std::shared_ptr<nginx::config_statement>> statements_;
    int get_port();
    std::vector<server> get_servers();
};



}

#endif // NGINX_CONFIG_H