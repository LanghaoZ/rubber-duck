// An nginx config file parser.
//
// See:
//   http://wiki.nginx.org/Configuration
//   http://blog.martinfjordvald.com/2010/07/nginx-primer/
//
// How Nginx does it:
//   http://lxr.nginx.org/source/src/core/ngx_conf_file.c

#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "nginx/config_parser.h"
#include "nginx/config_statement.h"
#include "nginx/config.h"

namespace nginx {

const char* config_parser::token_type_as_string(token_type type) 
{
  switch (type) {
    case start:         return "start";
    case normal:        return "normal";
    case start_block:   return "start_block";
    case end_block:     return "end_block";
    case comment:       return "comment";
    case statement_end: return "statement_end";
    case eof:           return "eof";
    case error:         return "error";
    default:            return "unknown token type";
  }
}

config_parser::token_type config_parser::parse_token(std::istream* input, std::string* value) 
{
  token_parser_state state = initial_whitespace;
  while (input->good()) 
  {
    const char c = input->get();
    if (!input->good()) 
      break;
    
    switch (state) 
    {
      case initial_whitespace:
        switch (c) 
        {
          case '{':
            *value = c;
            return start_block;
          case '}':
            *value = c;
            return end_block;
          case '#':
            *value = c;
            state = token_type_comment;
            continue;
          case '"':
            *value = c;
            state = double_quote;
            continue;
          case '\'':
            *value = c;
            state = single_quote;
            continue;
          case ';':
            *value = c;
            return statement_end;
          case ' ':
          case '\t':
          case '\n':
          case '\r':
            continue;
          default:
            *value += c;
            state = token_type_normal;
            continue;
        }
      case single_quote:
        *value += c;
        if (c == '\'') 
          return normal;
        continue;
      case double_quote:
        *value += c;
        if (c == '"') 
          return normal;
        continue;
      case token_type_comment:
        if (c == '\n' || c == '\r') 
          return comment;
        *value += c;
        continue;
      case token_type_normal:
        if (c == ' ' || c == '\t' || c == '\n' || c == '\t' ||
            c == ';' || c == '{' || c == '}') 
        {
          input->unget();
          return normal;
        }
        *value += c;
        continue;
    }
  }

  // If we get here, we reached the end of the file.
  if (state == single_quote || state == double_quote)
    return error;

  return eof;
}

bool config_parser::parse(std::istream* config_file, nginx::config* config) 
{
  std::stack<nginx::config*> config_stack;
  config_stack.push(config);
  token_type last_token_type = start;
  token_type token_type;
  while (true) 
  {
    std::string token;
    token_type = parse_token(config_file, &token);

    if (token_type == error) 
    {
      break;
    }

    if (token_type == comment) 
    {
      // Skip comments.
      continue;
    }

    if (token_type == start) 
    {
      // Error.
      break;
    } 
    else if (token_type == normal) 
    {
      if (last_token_type == start ||
          last_token_type == statement_end ||
          last_token_type == start_block ||
          last_token_type == end_block ||
          last_token_type == normal) 
      {
        if (last_token_type != normal) 
        {
          config_stack.top()->statements_.emplace_back(new config_statement);
        }
        config_stack.top()->statements_.back().get()->tokens_.push_back(token);
      } 
      else 
      {
        // Error.
        break;
      }
    } 
    else if (token_type == statement_end) 
    {
      if (last_token_type != normal) {
        // Error.
        break;
      }
    } 
    else if (token_type == start_block) 
    {
      if (last_token_type != normal) 
      {
        // Error.
        break;
      }
      nginx::config* const new_config = new nginx::config;
      config_stack.top()->statements_.back().get()->child_block_.reset(new_config);
      config_stack.push(new_config);
    } 
    else if (token_type == end_block) {

      if (last_token_type != statement_end &&
	        last_token_type != start_block &&
	        last_token_type != end_block) 
      {
        // Error.
        break;
      }
      config_stack.pop();
    } 
    else if (token_type == eof) {
      if (last_token_type != statement_end &&
          last_token_type != end_block) 
      {
        // Error.
        break;
      }
      return true;
    } 
    else {
      // Error. Unknown token.
      break;
    }
    last_token_type = token_type;
  }

  printf ("Bad transition from %s to %s\n",
          token_type_as_string(last_token_type),
          token_type_as_string(token_type));
  return false;
}

bool config_parser::parse(const char* file_name, nginx::config* config) 
{
  std::ifstream config_file;
  config_file.open(file_name);
  if (!config_file.good()) 
  {
    printf ("Failed to open config file: %s\n", file_name);
    return false;
  }

  const bool return_value = 
    parse(dynamic_cast<std::istream*>(&config_file), config);
  config_file.close();
  return return_value;
}

} // namespace nginx