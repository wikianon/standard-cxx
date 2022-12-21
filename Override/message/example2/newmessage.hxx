#include "messagesender.hxx"

class NewMessageCoverter : public MessageCoverter
{
 public:
 //Function Overriding not virtual  
 std::string convert(std::string msg)
 {
  msg = "<START> " + msg + " <END>";
  return msg;
 }
};  
