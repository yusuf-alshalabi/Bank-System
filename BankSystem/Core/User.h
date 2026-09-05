#pragma once
#include <iostream>
#include <string>
#include "Person.h"
#include "../../Libs/Cpp-Library-Collection/Lib/String.h"
#include "../../Libs/Cpp-Library-Collection/Lib/Date.h"
#include <vector>
#include <fstream>

using namespace std;
class User : public Person
{
private:

    enum enMode { EmptyMode = 0, UpdateMode = 1, AddNewMode = 2 };
    enMode _Mode;
    string _UserName;
    string _Password;
    int _Permissions;

    bool _MarkedForDelete = false;

    static User _ConvertLinetoUserObject(string Line, string Seperator = "#//#")
    {
        vector<string> vUserData;
        vUserData = Core::String::Split(Line, Seperator);

        return User(enMode::UpdateMode, vUserData[0], vUserData[1], vUserData[2],
            vUserData[3], vUserData[4], vUserData[5], stoi(vUserData[6]));

    }

    static string _ConverUserObjectToLine(User User, string Seperator = "#//#")
    {

        string UserRecord = "";
        UserRecord += User.FirstName + Seperator;
        UserRecord += User.LastName + Seperator;
        UserRecord += User.Email + Seperator;
        UserRecord += User.Phone + Seperator;
        UserRecord += User.UserName + Seperator;
        UserRecord += User.Password + Seperator;
        UserRecord += to_string(User.Permissions);

        return UserRecord;

    }

    static  vector <User> _LoadUsersDataFromFile()
    {

        vector <User> vUsers;

        fstream MyFile;
        MyFile.open("Users.txt", ios::in);//read Mode

        if (MyFile.is_open())
        {

            string Line;


            while (getline(MyFile, Line))
            {

                User User = _ConvertLinetoUserObject(Line);

                vUsers.push_back(User);
            }

            MyFile.close();

        }

        return vUsers;

    }

    static void _SaveUsersDataToFile(vector <User> vUsers)
    {

        fstream MyFile;
        MyFile.open("Users.txt", ios::out);//overwrite

        string DataLine;

        if (MyFile.is_open())
        {

            for (User U : vUsers)
            {
                if (U.MarkedForDeleted() == false)
                {
                    //we only write records that are not marked for delete.  
                    DataLine = _ConverUserObjectToLine(U);
                    MyFile << DataLine << endl;

                }

            }

            MyFile.close();

        }

    }

    void _Update()
    {
        vector <User> _vUsers;
        _vUsers = _LoadUsersDataFromFile();

        for (User& U : _vUsers)
        {
            if (U.UserName == UserName)
            {
                U = *this;
                break;
            }

        }

        _SaveUsersDataToFile(_vUsers);

    }

    void _AddNew()
    {

        _AddDataLineToFile(_ConverUserObjectToLine(*this));
    }

    void _AddDataLineToFile(string  stDataLine)
    {
        fstream MyFile;
        MyFile.open("Users.txt", ios::out | ios::app);

        if (MyFile.is_open())
        {

            MyFile << stDataLine << endl;

            MyFile.close();
        }

    }

    static User _GetEmptyUserObject()
    {
        return User(enMode::EmptyMode, "", "", "", "", "", "", 0);
    }

    std::string _PrepareLoginRegisterLine(std::string Seperator = "#//#") {
        std::string LoginRegisterLine = "";
        LoginRegisterLine += Core::Date::GetSystemDateTime() + Seperator;
        LoginRegisterLine += GetUserName() + Seperator;
        LoginRegisterLine += GetPassword() + Seperator;
        LoginRegisterLine += std::to_string(GetPermissions());
        return LoginRegisterLine;
    }

public:

    enum enPermissions {
        eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
        pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
    };

    User(enMode Mode, string FirstName, string LastName,
        string Email, string Phone, string UserName, string Password,
        int Permissions) :
        Person(FirstName, LastName, Email, Phone)

    {
        _Mode = Mode;
        _UserName = UserName;
        _Password = Password;
        _Permissions = Permissions;
    }

    bool IsEmpty()
    {
        return (_Mode == enMode::EmptyMode);
    }

    bool MarkedForDeleted()
    {
        return _MarkedForDelete;
    }

    string GetUserName() const
    {
        return _UserName;
    }

    void SetUserName(string UserName)
    {
        _UserName = UserName;
    }

    __declspec(property(get = GetUserName, put = SetUserName)) string UserName;

    void SetPassword(string Password)
    {
        _Password = Password;
    }

    string GetPassword() const
    {
        return _Password;
    }
    __declspec(property(get = GetPassword, put = SetPassword)) string Password;

    void SetPermissions(int Permissions)
    {
        _Permissions = Permissions;
    }

    int GetPermissions() const
    {
        return _Permissions;
    }
    __declspec(property(get = GetPermissions, put = SetPermissions)) int Permissions;

    static User Find(string UserName)
    {
        fstream MyFile;
        MyFile.open("Users.txt", ios::in);//read Mode

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                User User = _ConvertLinetoUserObject(Line);
                if (User.UserName == UserName)
                {
                    MyFile.close();
                    return User;
                }
            }

            MyFile.close();

        }

        return _GetEmptyUserObject();
    }

    static User Find(string UserName, string Password)
    {

        fstream MyFile;
        MyFile.open("Users.txt", ios::in);//read Mode

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                User User = _ConvertLinetoUserObject(Line);
                if (User.UserName == UserName && User.Password == Password)
                {
                    MyFile.close();
                    return User;
                }

            }

            MyFile.close();

        }
        return _GetEmptyUserObject();
    }

    enum enSaveResults { svFaildEmptyObject = 0, svSucceeded = 1, svFaildUserExists = 2 };

    enSaveResults Save()
    {

        switch (_Mode)
        {
        case enMode::EmptyMode:
        {
            if (IsEmpty())
            {
                return enSaveResults::svFaildEmptyObject;
            }
            break;
        }

        case enMode::UpdateMode:
        {
            _Update();
            return enSaveResults::svSucceeded;

            break;
        }

        case enMode::AddNewMode:
        {
            //This will add new record to file or database
            if (User::IsUserExist(_UserName))
            {
                return enSaveResults::svFaildUserExists;
            }
            else
            {
                _AddNew();
                //We need to set the mode to update after add new
                _Mode = enMode::UpdateMode;
                return enSaveResults::svSucceeded;
            }

            break;
        }
        }
		return enSaveResults::svFaildEmptyObject;
    }

    static bool IsUserExist(string UserName)
    {

        User User = User::Find(UserName);
        return (!User.IsEmpty());
    }

    bool Delete()
    {
        vector <User> _vUsers;
        _vUsers = _LoadUsersDataFromFile();

        for (User& U : _vUsers)
        {
            if (U.UserName == _UserName)
            {
                U._MarkedForDelete = true;
                break;
            }

        }

        _SaveUsersDataToFile(_vUsers);

        *this = _GetEmptyUserObject();

        return true;

    }

    static User GetAddNewUserObject(string UserName)
    {
        return User(enMode::AddNewMode, "", "", "", "", UserName, "", 0);
    }

    static vector <User> GetUsersList()
    {
        return _LoadUsersDataFromFile();
    }

    bool CheckAccessPermission(enPermissions Permission)
    {
        if (this->Permissions == enPermissions::eAll)
            return true;

        if ((Permission & this->Permissions) == Permission)
            return true;
        else
            return false;

    }

    void AddLoginRegister() {

        std::fstream MyFile;
        MyFile.open("LoginRegister.txt", std::ios::out | std::ios::app);

        if (MyFile.is_open())
        {
            MyFile << _PrepareLoginRegisterLine() << std::endl;
            MyFile.close();
        }
    }
};

