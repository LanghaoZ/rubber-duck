
#include "logging/logs.h"

void Logs::init() {
    boost::log::add_common_attributes();
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
    boost::log::add_file_log
    (
        boost::log::keywords::file_name = "../log/SERVERLOG_%N.log",
        boost::log::keywords::rotation_size = 10 * 1024 * 1024,
        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        boost::log::keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%]: %Message%",
        boost::log::keywords::auto_flush = true
    );

    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::trace
    );

    boost::log::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%]: %Message%");
}

void Logs::log_trace(std::string msg) {
    BOOST_LOG_TRIVIAL(trace) << msg << std::endl;
}

void Logs::log_debug(std::string msg) {
    BOOST_LOG_TRIVIAL(debug) << msg << std::endl;
}

void Logs::log_info(std::string msg) {
    BOOST_LOG_TRIVIAL(info) << msg << std::endl;
}

void Logs::log_warning(std::string msg) {
    BOOST_LOG_TRIVIAL(warning) << msg << std::endl;
}

void Logs::log_error(std::string msg) {
    BOOST_LOG_TRIVIAL(error) << msg << std::endl;
}

void Logs::log_fatal(std::string msg) {
    BOOST_LOG_TRIVIAL(fatal) << msg << std::endl;
}

void Logs::log_signal() {
    log_warning("Received termination signal. Cancelling all outstanding asynchronous operations...");
    log_warning("Shutting down the server...");
}

void Logs::log_request(http::server::request req, tcp::socket& sock) {
    std::string msg = req.method + " " + req.uri + " HTTP/" + 
                      std::to_string(req.http_version_major) + 
                      "." + std::to_string(req.http_version_minor) + 
                      " FROM: " + sock.remote_endpoint().address().to_string();
    log_trace(msg);
}