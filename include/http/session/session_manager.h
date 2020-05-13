//
// session_manager.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SESSION_MANAGER_HPP
#define HTTP_SESSION_MANAGER_HPP

#include <set>
#include "http/session/session.h"

namespace http {
namespace server {

/// Manages open connections so that they may be cleanly stopped when the server
/// needs to shut down.
class session_manager
{
public:
  session_manager(const session_manager&) = delete;
  session_manager& operator=(const session_manager&) = delete;

  /// Construct a connection manager.
  session_manager();

  /// Add the specified connection to the manager and start it.
  void start(std::shared_ptr<session> c);

  /// Stop the specified connection.
  void stop(std::shared_ptr<session> c);

  /// Stop all connections.
  void stop_all();

private:
  /// The managed connections.
  std::set<std::shared_ptr<session>> sessions_;
};

} // namespace server
} // namespace http

#endif // HTTP_SESSION_MANAGER_HPP