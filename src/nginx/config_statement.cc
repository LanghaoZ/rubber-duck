#include "nginx/config_statement.h"
#include "nginx/config.h"

namespace nginx {

std::string config_statement::to_string(int depth) 
{
  std::string serialized_statement;

  for (int i = 0; i < depth; ++i) 
  {
    serialized_statement.append("  ");
  }

  for (unsigned int i = 0; i < tokens_.size(); ++i) 
  {
    if (i != 0) 
    {
      serialized_statement.append(" ");
    }
    serialized_statement.append(tokens_[i]);
  }

  if (child_block_.get() != nullptr) 
  {
    serialized_statement.append(" {\n");
    serialized_statement.append(child_block_->to_string(depth + 1));
    for (int i = 0; i < depth; ++i) 
    {
      serialized_statement.append("  ");
    }
    serialized_statement.append("}");
  } 
  else 
  {
    serialized_statement.append(";");
  }

  serialized_statement.append("\n");
  return serialized_statement;
}

} // namespace nginx