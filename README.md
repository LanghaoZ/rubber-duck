# RubberDuck

RubberDuck is a simple HTTP server that can echo client requests and serve static files.

## Overview

We provide a berief overview of the application here to make it easy for you to follow what's going on when you read the source code.

```           
<server_main> ---> <config_parser>
               |          |
               |          |   parse config and give it to factory
               |         \|/
               |---> <request_handler_factory>
               |                       |      
               |                       |   dispatch request handler     
               |                      \|/
               |---> <server> ---> <session>
                               |
                               |-> <session> -> request_handler
                               .                -> echo_request_handler
                               .                -> static_request_handler
                               .                -> not_found_request_handler
                               .                -> status_request_handler
                               .
                               .
                               .                             
```

The primary function of our server is to echo client requests, serve static files, show server status, and return error/not_found page, based on C++ and Boost library.

The above graph shows the layout of our source code. First, the config_parser parses the server config. Then it initializes the request_handler_factory and create all the necessary request handlers, and finally starts up the server.

For each request, the server establishes a session. The session uses request_parser to parse the incoming message, and if it is valid, it asks the request_handler_factory to dispatch a request handler to handle the request. 


## Development

Here's commands you should be aware of when developing this project.

To build the program, run

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

To start the server after the build, run

```
$ ./bin/server ../configs/developent.conf
```

To unit test, run

```
$ make test
```

Note ```make test``` does not give you the output of tests. If you want to see the output, you can run the test individually as follows.

```
$ bin/<unit_test_executable>
```

```<unit_test_executable>``` can be replaced with ```server_test```, ```session_test```, ```config_parser_test```, ```request_parser_test```, ```response_test```, ```header_test```, ```config_statement_test```, ```config_test```, ```echo_request_handler_test```, ```static_request_handler_test```, ```request_handler_factory_test```, ```mime_types_test```, ```request_test```, ```logging_test```, and each of them tests a class respectively.

To run integration test by itself, you can do this.

```
$ ./integartion_test/integration_test.sh
```

To perform test coverage test, run

```
$ mkdir build_coverage
$ cd build_coverage
$ cmake -DCMAKE_BUILD_TYPE=Coverage ..
$ make coverage
```

More information about CMake can be found: ```https://www.cs130.org/guides/cmake/```

## Contribution

Here's some guidelines for contributing to this project.

### Adding a New Request Handler

You can follow these steps to create a new request handler to the server. In this part, we use ```echo_request_handler``` as an example to illustrate, while we also provide a template to work with.

1. Decide what the handler looks like in the config file. Specify any attributes it may need. All the config files are located at ```configs``` folder.

Here is the example we provide,  ```echo_request_handler```:
```
location "/echo" echo_handler {
  # no arguments required for echo_handler, but additional arguments can be added for new handler if needed
}
```

Here is the template for the new handler:

```
location "/new_path" new_handler {

  # here is for where arguments can be added for new a handler
  attribute_name attribute_value
  
  # attribute can have multiple values
  attribute_name attribute_value_1 attribute_value_2
  
}
```
	
2. Define a class for the new request handler. It should be placed under ```http/request/request_handler``` folder. We here use echo_request_handler as an example. When in doubt, you can also check out how other requst handlers are implemented. 

Here is the example we provide,  ```echo_request_handler```:

echo_request_handler.h:
```
#ifndef HTTP_ECHO_REQUEST_HANDLER_H
#define HTTP_ECHO_REQUEST_HANDLER_H

#include "http/request_handler/request_handler.h"
#include "nginx/config.h"

namespace http {
namespace request_handler {

// A request handler must inherit from request_handler class
class echo_request_handler : public request_handler
{
public:
  // Take config to construct a echo request handler.
  static echo_request_handler* init(const std::string& location_path, const nginx::config& config);
  
  virtual response handle_request(const request::request& req);
};

} // namespace request_handler
} // namespace http

#endif // HTTP_ECHO_REQUEST_HANDLER_H
```

echo_request_handler.cc:
```
#include "http/request_handler/echo_request_handler.h"
#include "http/request/request.h"
#include "http/response.h"
#include "http/server/server.h"
#include "nginx/config.h"
#include "nginx/location.h"
#include "http/status_code.h"

namespace http {
namespace request_handler {

echo_request_handler* echo_request_handler::init(const std::string& location_path, const nginx::config& config)
{
  return new echo_request_handler(); // here implements the initialization
}

response echo_request_handler::handle_request(const request::request& req)
{

  response res;

  // set response status
  res.code = status_code::ok;

  server::server::update_request_history(req.uri, status_code::ok);

  // set response content
  res.body = request_to_string(req);
  
  // set response header
  res.headers[header::field_name_type_to_string(header::content_type)] = header::field_value_type_to_string(header::text_plain);
  res.headers[header::field_name_type_to_string(header::content_length)] = std::to_string(res.body.size());
  
  return res;
}

} // namespace request_handler
} // namespace http
```

Here we provide a template for new handler class with more detailed comments about how to create the header and cc files. The namThe namThe nam

new_request_handler.h (under ```include/http/request/request_handler``` folder):
			
```
#ifndef HTTP_NEW_REQUEST_HANDLER_H
#define HTTP_NEW_REQUEST_HANDLER_H

#include "request_handler.h"
#include "nginx/config.h"

namespace http {
namespace server {
namespace request_handler {

// A request handler must inherit from request_handler class
class new_request_handler : public request_handler
{
public:

  // A request handler must have static init method
  static std::shared_ptr<new_request_handler> init(const nginx::config& config);

  // A request handler must have a constructor to initialize
  // the base class. You can add more arguments as you wish,
  // but it needs a path parameter at least.
  new_request_handler(const std::string& path);

  // A request handler must implement handle_request method
  // to produce a response given a request.
  virtual response handle_request(const request& req);
  
private:
  // Add private functions or variables here as you wish
};

} // namespace request_handler
} // namespace server
} // namespace http

#endif // HTTP_NEW_REQUEST_HANDLER_H
```
			
new_request_handler.cc (under ```src/http/request/request_handler``` folder):

```
#include "http/server/request_handler/new_request_handler.h"
#include "http/server/request.h"
#include "http/server/response.h"
#include "nginx/config.h"
#include "nginx/location.h"

namespace http {
namespace server {
namespace request_handler {
			
std::shared_ptr<new_request_handler> new_request_handler::init(const nginx::config& config)
{
  std::vector<nginx::location> locations = config.get_locations();
  nginx::location location = locations[0];
  return std::make_shared<new_request_handler>(location.path);
}
			
new_request_handler::new_request_handler(const std::string& path)
: request_handler(path)
{
  // Do any additional initialization you need
}

response new_request_handler::handle_request(const request& req)
{

  response res;

  // set response status
  res.code = http::server::response::ok;

  // set response content
  res.body = "I am new request handler";

  // set response header
  res.headers[header::field_name_type_as_string(header::content_type)] 
    = header::field_value_type_as_string(header::text_plain);
  res.headers[header::field_name_type_as_string(header::content_length)] 
    = std::to_string(res.body.size());
			
  return res;
}
```

3. Now that you created a new request handler, you need to tell the factory when to how to instantiate it. Go to ```request_handler_factory.cc```. Under ```create_handler``` method, insert an addtional```else if``` clause so the new handler can be created and dispatched. Note the name of the handler must match the one in the config file.

```
// Here is checking for echo_request_handler
if (location.handler == "echo_handler")
{
  return echo_request_handler::init(config);
}
else if (location.handler == "static_handler")
{
  return static_request_handler::init(config);
}

...

// Add a new else if clause here for new_request_handler
else if (location.handler == "new_handler")
{
  return new_request_handler::init(config);
}
```

4. Register the new request handler source file to ```CMakeLists.txt```.

```
add_library(
  server_lib 
  
  ...
  
  src/http/server/request_handler/request_handler.cc 
  src/http/server/request_handler/request_handler_factory.cc 
  src/http/server/request_handler/static_request_handler.cc 
  src/http/server/request_handler/echo_request_handler.cc 
  src/http/request_handler/status_request_handler.cc
  src/http/request_handler/not_found_request_handler.cc
  
  # Add the source file path to new request handler here
  src/http/server/request_handler/new_request_handler.cc
  
  ...
)
```

You are all set! Run the server and check whether your handler is doing what you expect it to do.

More information about echo_request_handler can be found: ```https://www.cs130.org/assignments/2/#write-an-echoing-web-server```

More information about other handlers can be found: ```https://www.cs130.org/assignments/4/#serve-static-files-from-your-server```, ```https://www.cs130.org/assignments/6/#write-a-404-handler```, ```and https://www.cs130.org/assignments/6/#write-a-status-handler```

### Testing a New Request Handler

You can follow these steps to unit test the new request handler you created in previous section.

1. Add the test file under ```tests/http/request/request_handler``` folder. Here's a template for ```new_request_handler_test.cc```.

```
#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "http/server/request_handler/new_request_handler.h"
#include "http/server/request.h"
#include "http/server/response.h"
#include "http/server/header.h"

namespace http {
namespace server {
namespace request_handler {

class NewRequestHandlerTest : public ::testing::Test 
{
protected:
  NewRequestHandlerTest()
    : handler_("/new_path")
  {
  }
  
  new_request_handler handler_;
  response res_;
};

TEST_F(NewRequestHandlerTest, EverythingIsWorking) 
{
  // Write what you want to test
}

// you can add more tests as you wish

} // namespace request_handler
} // namespace server
} // namespace http
```

2. Register the unit test file to the ```CMakeLists.txt```.

```
...

# Add test executable

# This is for the echo_reqest_handler test
add_executable(echo_request_handler_test tests/http/request_handler/echo_request_handler_test.cc)
target_link_libraries(echo_request_handler_test server_lib config_parser_lib logging_lib Boost::system Boost::regex Boost::log_setup Boost::log gtest_main)

...

# Here is the temeplate to add the new_reqest_handler test
add_executable(new_request_handler_test tests/http/server/request_handler/new_request_handler_test.cc)
target_link_libraries(new_request_handler_test server_lib config_parser_lib logging_lib Boost::system Boost::regex Boost::log_setup Boost::log gtest_main)

...

# Discover Google test instances

# This is for the echo_reqest_handler test
gtest_discover_tests(echo_request_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

...

# Here is the temeplate to add the new_reqest_handler test
gtest_discover_tests(new_request_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
		
...

# Register the new unit test to coverage report summary
generate_coverage_report(
  ...
  
  TESTS 
  
    ...
    
    # This is for the echo_reqest_handler test
    echo_request_handler_test
    
    ...
    
    # Here is the temeplate to add the new_reqest_handler test
    new_request_handler_test
    
    ...

)
```
		
### Coding Styles

Here's some coding styles we ask you to follow for consistency.

#### Naming Conventions

We use lowercase and underscore for everything except tests.

```
class my_class 
{
  ...
}
```

For tests, we use CamelCase.
```
TEST_F(MyClassTest, EverythingIsWorking)
{
  ...
}
```

Class member variables end with underscore but struct member don't.
```
class my_class
{
  static my_static_variable; // static variables don't end with underscore
  int my_member_variable_;
}

struct my_struct
{
  int my_member_variable;
}
```



#### Format

Any open bracket goes to next line except for namespaces.

```
my_class
{

}

my_method()
{

}
```

Namespces can have open bracket on the same line. Closing bracket
is followed by a comment that indicates the end of namespace.

```
namespace foo {

} // namespace foo
```

Any content inside the namespace doesn't need an indentation, including nested namespaces.

```
namespace foo {
namespace bar {
namespace foobar {

class my_class
{

}

} // namespace foobar
} // namespace bar
} // namespace foo
```

No indentation is needed for access level identifiers.
```
my_class
{
public:
  ...
   
protected:
  ...
 
private:
  ...
}
```

#### Pointers

Use smart pointers when possible.

#### Logging

Log the activities when appropriate.

```
#include "logging/logging.h"

// Use this only for debugging. When you 
// make a pull request, this should not 
// appear in the source code.
logging::logging::log_trace("my function is called");

// Use this if you think the activity might 
// be useful for future developers to know
logging::logging::log_debug("echo_handler is handling this request");

// Use this if you think the activity might
// be useful for server administrator 
logging::logging::log_info("server has started at port 8080");

// Use this when something unexpected may happen
logging::logging::log_warning("closing a session");

// Use this when there is an error
// but the server can resume its service.
logging::logging::log_error("there is a segmentation fault");

// Use this when there is an fatal error 
// and the server needs to shut down.
logging::logging::log_fatal("I don't have any memory left");
```

### Common Pitfalls

Here's some issues we anticipate you would encounter, and how to resolve them.

#### Why does my test work with ```make test```, but fails when run individually?

Most likely this has to do with the working directory. When you run ```make test```, the working directory is ```tests/``` as specified in ```CMakeLists.txt```. When you run the test indiviudally, the working directory is your current directory.

#### Why running integration test individually does not work?

```integration_test.sh``` is written such that it is expected to be run from ```${CMAKE_CURRENT_BINARY_DIR}/```. Please don't ```cd``` into ```integration_test``` folder.

#### My changes to integration test are not reflected. Why?

Changes to integration test is not watched. You need to run ```cmake``` again.

#### Why are there so many config files? I'm confused.

Here's some description of the config files.

```configs/development.conf```: This is for running the server manually in development.

```configs/production.conf```: This is for deployment on Google Cloud.

```tests/integration_test.conf```: This is used by ```integration_test.sh```.

```tests/request_handler/request_handler_factory_test.conf```: This is used by ```request_handler_test.cc```.

