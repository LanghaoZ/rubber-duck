#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "gtest/gtest.h"
#include "logging/logging.h"
#include <stdio.h> 

namespace logging {

class LoggingTest : public ::testing::Test 
{
protected:
  std::string filename_ = "../log/TESTLOG_0.log";
  std::string msg_ = "Log Test";
  std::ifstream login_;

  void trim_log_content(std::string& log_msg) 
  {
    log_msg = log_msg.substr(log_msg.find("]:") + 3);
  }

  std::string get_last_log_message() 
  {
    login_.open(filename_);
    std::string lastline;
    std::getline(login_, lastline);
    std::string currline;
    while (std::getline(login_, currline)) 
    {
      lastline = currline;
    }
    login_.close();
    return lastline;
  }
};

TEST_F(LoggingTest, LogTraceMsg) 
{
  remove(filename_.c_str());
  logging::init(filename_);
  logging::log_trace(msg_);
  std::string log_msg = get_last_log_message();
  int index = log_msg.find("trace");
  trim_log_content(log_msg);

  EXPECT_EQ(msg_, log_msg);
  EXPECT_GE(index, 0);
}

TEST_F(LoggingTest, LogDebugMsg) 
{
  remove(filename_.c_str());
  logging::init(filename_);
  logging::log_debug(msg_);
  std::string log_msg = get_last_log_message();
  int index = log_msg.find("debug");
  trim_log_content(log_msg);

  EXPECT_EQ(msg_, log_msg);
  EXPECT_GE(index, 0);
}

TEST_F(LoggingTest, LogInfoMsg) 
{
  remove(filename_.c_str());
  logging::init(filename_);
  logging::log_info(msg_);
  std::string log_msg = get_last_log_message();
  int index = log_msg.find("info");
  trim_log_content(log_msg);

  EXPECT_EQ(msg_, log_msg);
  EXPECT_GE(index, 0);
}

TEST_F(LoggingTest, LogWarningMsg) 
{
  remove(filename_.c_str());
  logging::init(filename_);
  logging::log_warning(msg_);
  std::string log_msg = get_last_log_message();
  int index = log_msg.find("warning");
  trim_log_content(log_msg);

  EXPECT_EQ(msg_, log_msg);
  EXPECT_GE(index, 0);
}

TEST_F(LoggingTest, LogErrorMsg) 
{
  remove(filename_.c_str());
  logging::init(filename_);
  logging::log_error(msg_);
  std::string log_msg = get_last_log_message();
  int index = log_msg.find("error");
  trim_log_content(log_msg);

  EXPECT_EQ(msg_, log_msg);
  EXPECT_GE(index, 0);
}

TEST_F(LoggingTest, LogFatalMsg) 
{
  remove(filename_.c_str());
  logging::init(filename_);
  logging::log_fatal(msg_);
  std::string log_msg = get_last_log_message();
  int index = log_msg.find("fatal");
  trim_log_content(log_msg);

  EXPECT_EQ(msg_, log_msg);
  EXPECT_GE(index, 0);
}

} // namespace logging