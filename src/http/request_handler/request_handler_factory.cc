#include "http/request_handler/request_handler_factory.h"
#include "http/request_handler/request_handler.h"
#include "nginx/config.h"
#include "nginx/config_statement.h"
#include "nginx/location.h"
#include "http/request_handler/echo_request_handler.h"
#include "http/request_handler/static_request_handler.h"
#include "http/request_handler/status_request_handler.h"
#include "http/request_handler/not_found_request_handler.h"
#include "logging/logging.h"

namespace http {
namespace request_handler {

request_handler_factory::request_handler_factory()
  : request_handlers_(),
    http_404_request_handler_(std::make_shared<not_found_request_handler>())
{
}

void request_handler_factory::init(const std::vector<nginx::location>& locations)
{

  for (int i = 0; i < locations.size(); i++)
  {    

    // reconstruct config from location
    nginx::config config;
    
    if (locations[i].root.size() > 0)
    {
      auto root_config_statement = std::make_shared<nginx::config_statement>();
      root_config_statement.get()->tokens_.push_back("root");
      root_config_statement.get()->tokens_.push_back("\"" + locations[i].root + "\"");
      config.statements_.push_back(root_config_statement);
    }
    
    // obtain ownwership of the request_handler pointer
    auto handler = std::shared_ptr<request_handler>(
      create_handler(locations[i].path, locations[i].handler, config));

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
  return http_404_request_handler_;
}

request_handler* request_handler_factory::create_handler(const std::string& location_path, 
  const std::string& location_handler, const nginx::config& config)
{

  // Additional handlers can be added here for dispatch
  if (location_handler == "echo_handler")
  {
    return echo_request_handler::init(location_path, config);
  }
  else if (location_handler == "static_handler")
  {
    return static_request_handler::init(location_path, config);
  }
  else if (location_handler == "status_handler") 
  {
    return status_request_handler::init(location_path, config);
  }

  return nullptr;
}

} // namespace request_handler
} // namespace http