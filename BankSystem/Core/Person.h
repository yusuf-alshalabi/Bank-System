#pragma once

#include <iostream>
#include <string>

class Person
{
private:
    std::string _FirstName;
    std::string _LastName;
    std::string _Email;
    std::string _Phone;

public:
    // Default & Parameterized Constructor
    Person(const std::string& FirstName = "", const std::string& LastName = "", const std::string& Email = "", const std::string& Phone = "")
    {
        _FirstName = FirstName;
        _LastName = LastName;
        _Email = Email;
        _Phone = Phone;
    }

    // Property Set & Get: FirstName
    void SetFirstName(const std::string& FirstName)
    {
        _FirstName = FirstName;
    }

    std::string GetFirstName() const
    {
        return _FirstName;
    }
    __declspec(property(get = GetFirstName, put = SetFirstName)) std::string FirstName;

    // Property Set & Get: LastName
    void SetLastName(const std::string& LastName)
    {
        _LastName = LastName;
    }

    std::string GetLastName() const
    {
        return _LastName;
    }
    __declspec(property(get = GetLastName, put = SetLastName)) std::string LastName;

    // Property Set & Get: Email
    void SetEmail(const std::string& Email)
    {
        _Email = Email;
    }

    std::string GetEmail() const
    {
        return _Email;
    }
    __declspec(property(get = GetEmail, put = SetEmail)) std::string Email;

    // Property Set & Get: Phone
    void SetPhone(const std::string& Phone)
    {
        _Phone = Phone;
    }

    std::string GetPhone() const
    {
        return _Phone;
    }
    __declspec(property(get = GetPhone, put = SetPhone)) std::string Phone;

    // Helper Methods
    std::string FullName() const
    {
        return _FirstName + " " + _LastName;
    }

    void Print() const
    {
        std::cout << "\nInfo:";
        std::cout << "\n___________________";
        std::cout << "\nFirstName: " << _FirstName;
        std::cout << "\nLastName : " << _LastName;
        std::cout << "\nFull Name: " << FullName();
        std::cout << "\nEmail    : " << _Email;
        std::cout << "\nPhone    : " << _Phone;
        std::cout << "\n___________________\n";
    }
};