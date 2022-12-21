#include <iostream>
//Message Converter Class
class MessageCoverter
{
 public:
 std::string convert(std::string msg)
 {
  msg = "[START]" + msg + "[END]";
  return msg;
 }
};