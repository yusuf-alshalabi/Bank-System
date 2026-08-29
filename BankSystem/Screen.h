#pragma once
#include <iostream>
#include <string>

class Screen
{
protected:
    static void _DrawScreenHeader(std::string Title, std::string SubTitle = "")
    {
        std::cout << "\t\t\t\t\t===========================================\n";
        std::cout << "\t\t\t\t\t  " << Title << "\n";
        if (SubTitle != "")
        {
            std::cout << "\t\t\t\t\t  " << SubTitle << "\n";
        }
        std::cout << "\t\t\t\t\t===========================================\n\n";
    }
};