#include "http/server/request_handler/request_handler_factory.h"
#include "http/server/request_handler/request_handler.h"
#include "nginx/config.h"
#include "nginx/config_statement.h"
#include "http/server/request_handler/echo_request_handler.h"
#include "http/server/request_handler/static_request_handler.h"

namespace http {
namespace server {
namespace request_handler {

std::vector<std::shared_ptr<request_handler>>
  request_handler_factory::create_request_handlers(nginx::config& config)
{
  std::vector<std::shared_ptr<request_handler>> res;

  std::vector<nginx::server> servers = config.get_servers();

  for (int i = 0; i < servers.size(); i++)
  {
    for (int j = 0; j < servers[i].locations.size(); j++)
    {
      if (servers[i].handler.compare("static") == 0)
      {
        auto handler = std::make_shared<static_request_handler>(servers[i].locations[j].value, servers[i].locations[j].root);
        res.push_back(handler);
      }
      else if (servers[i].handler.compare("echo") == 0)
      {
        auto handler = std::make_shared<echo_request_handler>(servers[i].locations[j].value);
        res.push_back(handler);
      }
    }
  }
  
  return res;
}

} // namespace request_handler
} // namespace server
} // namespace http