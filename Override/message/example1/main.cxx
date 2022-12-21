#include "messagesender.hxx"

int main()
{
 MessageSender msgSenderObj;
 
 MessageCoverter * msgConverterPtr = new MessageCoverter();
 
 msgSenderObj.setMsgCoverterPtr(msgConverterPtr);
 
 msgSenderObj.sendMessage("Hello World");
 delete msgConverterPtr; 

  return 0;
}