#include "nginx/config.h"
#include "nginx/config_statement.h"

namespace nginx {

std::string config::to_string(int depth) 
{
  std::string serialized_config;
  for (const auto& statement : config::statements_) 
  {
    serialized_config.append(statement->to_string(depth));
  }
  return serialized_config;
}

int config::get_port(const config* config) 
{

  int port = err_not_found;

  if (config != nullptr) {  
    for (auto statement : config->statements_) 
    {
      if (statement->child_block_.get() == nullptr) 
      {
        if (statement->tokens_.size() == 2 && statement->tokens_[0] == nginx::config_port) 
        {
          port = atoi(statement->tokens_[1].c_str());
          break;
        }
      } 
      else 
      {
        int ret = get_port(statement->child_block_.get());
        if (ret != nginx::err_not_found) 
        {
          port = ret;
          break;
        }
      }
    }
  }

  return (port >= 0 && port <= 0xffff) ? port : err_not_found;
}

} // namspace nginx