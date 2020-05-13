//
// session_manager.cc
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "http/session/session_manager.h"
#include "http/session/session.h"

namespace http {
namespace server {

session_manager::session_manager()
{
}

void session_manager::start(std::shared_ptr<session> c)
{
  sessions_.insert(c);
  c->start();
}

void session_manager::stop(std::shared_ptr<session> c)
{
  sessions_.erase(c);
  c->stop();
}

void session_manager::stop_all()
{
  for (auto c: sessions_)
    c->stop();
  sessions_.clear();
}

} // namespace server
} // namespace http