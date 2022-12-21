#include "messagecoverter.hxx"
//In assigned the object of MessageConverter to MessageSender for converting messages before sending i.e.
class MessageSender
{ 
  MessageCoverter *mMsgCoverterPtr;
  
 public:
  MessageSender() :mMsgCoverterPtr(NULL){}
 
  void sendMessage(std::string msg)
  {
   if(mMsgCoverterPtr)
   msg = mMsgCoverterPtr->convert(msg);
   std::cout<<"Message Sent :: "<<msg<<std::endl;
  }
  
  void setMsgCoverterPtr(MessageCoverter *msgCoverterPtr) 
  {
   mMsgCoverterPtr = msgCoverterPtr;
  }
};
