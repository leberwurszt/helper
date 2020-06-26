#include "functions.h"

std::string BoolToYesNoString(bool value)
{
    if(value)
        return "yes";
    else
        return "no";
}