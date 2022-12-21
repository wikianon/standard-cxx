#include "messageconverter.hxx"

class MessageSender
{ 
  MessageCoverter * mMsgCoverterPtr;
  
 public:
  MessageSender() :mMsgCoverterPtr(NULL){}
 
  void sendMessage(std::string msg)
  {
   if(mMsgCoverterPtr)
   msg = mMsgCoverterPtr->convert(msg);
   std::cout<<"Message Sent :: "<<msg<<std::endl;
  }
  
  void setMsgCoverterPtr(MessageCoverter * msgCoverterPtr) 
  {
   mMsgCoverterPtr = msgCoverterPtr;
  }
};