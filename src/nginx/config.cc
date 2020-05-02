#include "nginx/config.h"
#include "nginx/config_statement.h"

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

std::vector<server> config::get_servers()
{
  
  std::vector<server> servers;

  for (int i = 0; i < statements_.size(); i++)
  {
  
    if (statements_[i].get()->tokens_[0].compare("server") == 0)
    {
      
      server server;

      std::vector<std::shared_ptr<config_statement>> server_statements 
        = statements_[i].get()->child_block_.get()->statements_;

      for (int j = 0; j < server_statements.size(); j++)
      {
        
        if (server_statements[j].get()->tokens_[0].compare("handler") == 0)
        {
          server.handler = server_statements[j].get()->tokens_[1];
        }
        else if (server_statements[j].get()->tokens_[0].compare("location") == 0)
        {
          location location;
          location.value = server_statements[j].get()->tokens_[1];

          std::vector<std::shared_ptr<config_statement>> location_statements 
            = server_statements[j].get()->child_block_.get()->statements_;

          for (int k = 0; k < location_statements.size(); k++)
          {
            if (location_statements[k].get()->tokens_[0].compare("root") == 0)
            {
              location.root = location_statements[k].get()->tokens_[1];
            }
          }

          server.locations.push_back(location);
        }
      }

      servers.push_back(server);

    }
    
  }

  return servers;
}

} // namspace nginx