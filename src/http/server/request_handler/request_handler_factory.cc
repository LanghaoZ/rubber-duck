#include "http/server/request_handler/request_handler_factory.h"
#include "http/server/request_handler/request_handler.h"
#include "nginx/config.h"
#include "nginx/config_statement.h"
#include "nginx/location.h"
#include "http/server/request_handler/echo_request_handler.h"
#include "http/server/request_handler/static_request_handler.h"
#include "logging/logging.h"

namespace http {
namespace server {
namespace request_handler {

void request_handler_factory::init(const std::vector<nginx::location>& locations)
{

  logging::logging::log_debug("initializing factory");

  for (int i = 0; i < locations.size(); i++)
  {

    logging::logging::log_debug(locations[i].path);
    
    // reconstruct config from location
    nginx::config config;
    auto location_config_statement = std::make_shared<nginx::config_statement>();
    location_config_statement.get()->tokens_.push_back("location");
    location_config_statement.get()->tokens_.push_back("\"" + locations[i].path + "\"");
    location_config_statement.get()->tokens_.push_back(locations[i].handler);
    
    auto location_child_config = std::make_unique<nginx::config>();

    if (locations[i].root.size() > 0)
    {
      auto root_config_statement = std::make_shared<nginx::config_statement>();
      root_config_statement.get()->tokens_.push_back("root");
      root_config_statement.get()->tokens_.push_back("\"" + locations[i].root + "\"");
      location_child_config.get()->statements_.push_back(root_config_statement);
    }
    
    location_config_statement.get()->child_block_ = std::move(location_child_config);

    config.statements_.push_back(location_config_statement);

    std::shared_ptr<request_handler> handler = create_handler(config);

    if (handler != nullptr)
    {
      request_handlers_[locations[i].path] = handler;
    }

  }
  
}

std::shared_ptr<request_handler> request_handler_factory::dispatch(const std::string& uri)
{
  std::string path_to_check_matching = uri;

  while (path_to_check_matching != "")
  {

    auto matched_request_handler_iterator = request_handlers_.find(path_to_check_matching);
    if (matched_request_handler_iterator != request_handlers_.end())
    {
      logging::logging::log_debug("Use a request handler for path " + path_to_check_matching);
      return matched_request_handler_iterator->second;
    }

    // match longest path first
    path_to_check_matching = path_to_check_matching.substr(0, path_to_check_matching.find_last_of("/"));

  }

  logging::logging::log_warning("Cannot find the corresponding request handler");
  return nullptr;
}

std::shared_ptr<request_handler> request_handler_factory::create_handler(const nginx::config& config)
{

  std::vector<nginx::location> locations = config.get_locations();
  nginx::location location = locations[0];
  
  if (location.handler == "echo_handler")
  {
    return echo_request_handler::init(config);
  }
  else if (location.handler == "static_handler")
  {
    return static_request_handler::init(config);
  }

  return nullptr;
}

} // namespace request_handler
} // namespace server
} // namespace http