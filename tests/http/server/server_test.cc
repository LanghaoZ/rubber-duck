#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <unordered_map> 
#include "gtest/gtest.h"
#include "http/server/server.h"
#include "http/request_handler/request_handler.h"

namespace http {
namespace server {

void server_8080_thread()
{
  server s(8080);
  s.run();
}

TEST(ServerTest, ServerReadsDataFromClient) 
{
  
  try 
  {
    // run the server in a different thread
    boost::thread s(&server_8080_thread);

    // wait for one second for the server to start up
    // otherwise, client will try to connect to the server
    // that doesn't exist.
    boost::this_thread::sleep(boost::posix_time::seconds(1));

    // client
    // https://www.boost.org/doc/libs/1_42_0/doc/html/boost_asio/example/http/client/sync_client.cpp  
    // connect to the server
    boost::asio::io_service c_io_service;
    tcp::socket c_socket(c_io_service);
    tcp::resolver resolver(c_io_service);
    boost::asio::connect(c_socket, resolver.resolve({"localhost", "8080"}));

    // send a message to the server
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "rubber-duck\r\n";
    
    // Send the request.
    boost::asio::write(c_socket, request);

    // give the server some time to shutdown
    boost::this_thread::sleep(boost::posix_time::seconds(1));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    // the test should fail when there is an exception
    EXPECT_TRUE(false);
  } 
  
}

void server_8081_thread()
{
  server s(8081);
  s.run();
}

TEST(ServerTest, ServerReceivesSignalAndExits) 
{
  
  try {
    // run the server in a different thread
    boost::thread s(&server_8081_thread);

    // wait for one second for the server to start up
    // otherwise, client will try to connect to the server
    // that doesn't exist.
    boost::this_thread::sleep(boost::posix_time::seconds(1));

    // client
    // https://www.boost.org/doc/libs/1_42_0/doc/html/boost_asio/example/http/client/sync_client.cpp  
    // connect to the server
    boost::asio::io_service c_io_service;
    tcp::socket c_socket(c_io_service);
    tcp::resolver resolver(c_io_service);
    boost::asio::connect(c_socket, resolver.resolve({"localhost", "8081"}));

    std::raise(SIGTERM);

    // give the server some time to shutdown
    boost::this_thread::sleep(boost::posix_time::seconds(1));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    // the test should fail when there is an exception
    EXPECT_TRUE(false);
  } 
  
}

} // namespace server
} // namespace http