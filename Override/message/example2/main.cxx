#include "newmessage.hxx"

int main(void)
{
   MessageSender msgSenderObj;
   
   MessageCoverter * newMsgConverterPtr = new NewMessageCoverter();
   msgSenderObj.setMsgCoverterPtr(newMsgConverterPtr);

   msgSenderObj.sendMessage("Hello World");
   delete newMsgConverterPtr;

  return 0;
}