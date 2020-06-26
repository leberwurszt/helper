#pragma once
#include <iostream>
#include <string>
#include <list>
#include <algorithm>

class MessageCommand
{
    private:
        std::list<std::string> messageList;

    public:
        MessageCommand();
        ~MessageCommand();

        std::string PopMessage();
        void PushMessage(std::string message);
};