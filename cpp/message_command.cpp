#include "message_command.h"

MessageCommand::MessageCommand()
{
    
}

MessageCommand::~MessageCommand()
{

}

std::string MessageCommand::PopMessage()
{
    if(messageList.size() > 0)
    {
        std::string message = messageList.front();
        messageList.pop_front();
        return message;
    }
    else
        return "";
    
}

void MessageCommand::PushMessage(std::string message)
{
    messageList.push_back(message);
}