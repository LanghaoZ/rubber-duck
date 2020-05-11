#ifndef HTTP_STATUS_REQUEST_HANDLER
#define HTTP_STATUS_REQUEST_HANDLER

#include "request_handler.h"
#include "nginx/config.h"

namespace http {
namespace server {
namespace request_handler {

class status_request_handler : public request_handler {
    public:
        static std::shared_ptr<status_request_handler> init(const nginx::config& config);
        status_request_handler(const std::string& path);
        virtual response handle_request(const request& req);
    
};


} // namespace http
} // namespace server
} // namespace request_handler

#endif // HTTP_STATUS_REQUEST_HANDLER