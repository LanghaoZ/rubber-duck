// An nginx config file parser.

#ifndef NGINX_CONFIG_PARSER_H
#define NGINX_CONFIG_PARSER_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace nginx {

class config;

// The driver that parses a config file and generates an NginxConfig.
class config_parser {
public:
  config_parser() {}

  // Take a opened config file or file name (respectively) and store the
  // parsed config in the provided NginxConfig out-param.  Returns true
  // iff the input config file is valid.
  bool parse(std::istream* config_file, config* config);
  bool parse(const char* file_name, config* config);

private:
  enum token_type {
    start = 0,
    normal = 1,
    start_block = 2,
    end_block = 3,
    comment = 4,
    statement_end = 5,
    eof = 6,
    error = 7
  };
  const char* token_type_as_string(token_type type);

  enum token_parser_state {
    initial_whitespace = 0,
    single_quote = 1,
    double_quote = 2,
    token_type_comment = 3,
    token_type_normal = 4
  };

  token_type parse_token(std::istream* input, std::string* value);
};

} // namespace nginx


#endif // NGINX_CONFIG_PARSER_H