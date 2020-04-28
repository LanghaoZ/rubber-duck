#include <boost/asio.hpp>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include "gtest/gtest.h"
#include "logging/logs.h"
#include <stdio.h> 

class LogTest : public :: testing::Test {
    protected:
        std::string filename = "../log/TESTLOG_0.log";
        std::string msg = "Log Test";
        std::ifstream login;
        void trim_log_content(std::string& log_msg) {
            log_msg = log_msg.substr(log_msg.find("]:") + 3);
        }
        std::string get_last_log_message() {
            login.open(filename);
            std::string lastline;
            std::getline(login, lastline);
            std::string currline;
            while (std::getline(login, currline)) {
                lastline = currline;
            }
            login.close();
            return lastline;
        }
};

TEST_F(LogTest, LogTraceMsg) {

    remove(filename.c_str());
    Logs::init(true);
    Logs::log_trace(msg);
    std::string log_msg = get_last_log_message();
    int index = log_msg.find("trace");
    trim_log_content(log_msg);

    EXPECT_TRUE(msg == log_msg && index >= 0);
}

TEST_F(LogTest, LogDebugMsg) {

    remove(filename.c_str());
    Logs::init(true);
    Logs::log_debug(msg);
    std::string log_msg = get_last_log_message();
    int index = log_msg.find("debug");
    trim_log_content(log_msg);

    EXPECT_TRUE(msg == log_msg && index >= 0);
}

TEST_F(LogTest, LogInfoMsg) {

    remove(filename.c_str());
    Logs::init(true);
    Logs::log_info(msg);
    std::string log_msg = get_last_log_message();
    int index = log_msg.find("info");
    trim_log_content(log_msg);

    EXPECT_TRUE(msg == log_msg && index >= 0);
}

TEST_F(LogTest, LogWarningMsg) {

    remove(filename.c_str());
    Logs::init(true);
    Logs::log_warning(msg);
    std::string log_msg = get_last_log_message();
    int index = log_msg.find("warning");
    trim_log_content(log_msg);

    EXPECT_TRUE(msg == log_msg && index >= 0);
}

TEST_F(LogTest, LogErrorMsg) {

    remove(filename.c_str());
    Logs::init(true);
    Logs::log_error(msg);
    std::string log_msg = get_last_log_message();
    int index = log_msg.find("error");
    trim_log_content(log_msg);

    EXPECT_TRUE(msg == log_msg && index >= 0);
}

TEST_F(LogTest, LogFatalMsg) {

    remove(filename.c_str());
    Logs::init(true);
    Logs::log_fatal(msg);
    std::string log_msg = get_last_log_message();
    int index = log_msg.find("fatal");
    trim_log_content(log_msg);

    EXPECT_TRUE(msg == log_msg && index >= 0);
}