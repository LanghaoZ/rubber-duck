#include <algorithm>
#include <iostream>
#include "nginx/config.h"
#include "nginx/config_statement.h"
#include "nginx/location.h"

namespace nginx {

config::~config()
{
}

std::string config::to_string(int depth) 
{
  std::string serialized_config;
  for (const auto& statement : config::statements_) 
  {
    serialized_config.append(statement->to_string(depth));
  }
  return serialized_config;
}

int config::get_port() 
{

  int port = err_not_found;

  for (auto statement : statements_) 
  {
    if (statement->child_block_.get() == nullptr) 
    {
      if (statement->tokens_.size() == 2 && statement->tokens_[0] == config_port) 
      {
        port = atoi(statement->tokens_[1].c_str());
        break;
      }
    } 
    else 
    {
      // recursively find port
      int ret = statement->child_block_.get()->get_port();
      if (ret != nginx::err_not_found) 
      {
        port = ret;
        break;
      }
    }
  }

  return (port >= 0 && port <= 0xffff) ? port : err_not_found;
}

std::vector<location> config::get_locations() const
{
  
  std::vector<location> locations;

  for (int i = 0; i < statements_.size(); i++)
  {
  
    if (statements_[i].get()->tokens_[0] == "location")
    {
      location location;
      location.path = statements_[i].get()->tokens_[1];
      location.path.erase(std::remove(location.path.begin(), location.path.end(),'\"'), location.path.end());
      location.handler = statements_[i].get()->tokens_[2];
      
      std::vector<std::shared_ptr<config_statement>> location_statements 
        = statements_[i].get()->child_block_.get()->statements_;

      for (int j = 0; j < location_statements.size(); j++)
      {
        if (location_statements[j].get()->tokens_[0] == "root")
        {
          location.root = location_statements[j].get()->tokens_[1];
          location.root.erase(std::remove(location.root.begin(), location.root.end(),'\"'), location.root.end());
        }
      }

      locations.push_back(location);

    }
    
  }

  return locations;
}

} // namspace nginx