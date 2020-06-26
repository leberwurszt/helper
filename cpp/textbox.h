#pragma once
#include <iostream>
#include <string>

class Textbox
{
    private:
        std::string text;
        int maxLength = 20;
        bool active = false;
        int editPointer = 0;
    public:
        Textbox();
        Textbox(int maxLength);
        
        Textbox(std::string text);
        Textbox(std::string text, int maxLength);

        bool Append(std::string text);
        bool Insert(std::string text);
        bool Delete(int length);

        void Clear();
        bool MovePointer(int offset);
        std::string Show();

        bool SetPointer(int pos);
        bool SetText(std::string text);

        void SetMaxLength(int maxLength);
        void SetActive(bool active);

        int GetPointer();
        std::string GetText();
        bool IsActive();

};