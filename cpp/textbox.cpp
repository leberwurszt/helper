#include "textbox.h"

Textbox::Textbox()
{}

Textbox::Textbox(int maxLength)
{
    this->maxLength = maxLength;
}

Textbox::Textbox(std::string text)
{
    this->text = text;
    editPointer = this->text.length();
}

Textbox::Textbox(std::string text, int maxLength)
{
    this->text = text;
    this->maxLength = maxLength;
    editPointer = this->text.length();
}

bool Textbox::Append(std::string text)
{
    if(this->text.length() + text.length() <= maxLength)
    {
        this->text += text;
        editPointer = this->text.length();

        return true;
    }
    else
        return false;
}

bool Textbox::Insert(std::string text)
{
    if(this->text.length() + text.length() <= maxLength)
    {
        std::string newString = this->text.substr(0, editPointer);
        newString += text;
        newString += this->text.substr(editPointer, this->text.length() - editPointer);

        editPointer += text.length();
        this->text = newString;
        return true;
    }
    else
        return false;
}

bool Textbox::Delete(int length)
{
    if(length <= editPointer)
    {
        std::string newString = this->text.substr(0, editPointer - length);
        newString += this->text.substr(editPointer, this->text.length() - editPointer);

        this->text = newString;
        editPointer -= length;

        return true;
    }
    else
        return false;
}

void Textbox::Clear()
{
    text = "";
    editPointer = 0;
}

bool Textbox::MovePointer(int offset)
{
    if(editPointer + offset >= 0 && editPointer + offset <= text.length())
    {
        editPointer += offset;
        return true;
    }
    else
        return false;
}

std::string Textbox::Show()
{
    std::string stringShow = text.substr(0, editPointer);
    stringShow += '_';

    stringShow += text.substr(editPointer, text.length() - editPointer);
    return stringShow;

}

bool Textbox::SetPointer(int pos)
{
    if(pos < 0 && pos >= text.length())
    {
        editPointer = pos;
        return true;
    }
    else
        return false;
}

bool Textbox::SetText(std::string text)
{
    if(text.length() <= maxLength)
    {
        this->text = text;
        editPointer = this->text.length();
        
        return true;
    }
    else
        return false;
}

void Textbox::SetMaxLength(int maxLength)
{
    this->maxLength = maxLength;
}

void Textbox::SetActive(bool active)
{
    this->active = active;
}

int Textbox::GetPointer()
{
    return editPointer;
}

std::string Textbox::GetText()
{
    return text;
}

bool Textbox::IsActive()
{
    return active;
}