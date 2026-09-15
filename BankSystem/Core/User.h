#pragma once
#include <iostream>
#include <string>
#include "Person.h"
#include "../../Libs/Cpp-Library-Collection/Lib/String.h"
#include "../../Libs/Cpp-Library-Collection/Lib/Date.h"
#include "../../Libs/Cpp-Library-Collection/Lib/Util.h"
#include "Infrastructure/PasswordHasher.h"
#include "Infrastructure/AtomicFileStore.h"
#include <vector>

using namespace std;
class User : public Person
{
public:

    struct stLoginRegisterRecord
    {
        string DateTime;
        string UserName;
        string Password;
        int Permissions;

    };

private:

    bool _MarkedForDelete = false;

    static stLoginRegisterRecord _ConvertLogInRegisterLineToRecord(string Line, string Seperator = "#//#")
    {
        stLoginRegisterRecord LoginRegisterRecord;

        vector <string> LoginRegisterDataLine = Bank::Persistence::AtomicFileStore::SplitPreserve(Line, Seperator);

        // skip malformed records instead of indexing out of bounds
        if (LoginRegisterDataLine.size() < 4)
            return LoginRegisterRecord;

        LoginRegisterRecord.DateTime = LoginRegisterDataLine[0];
        LoginRegisterRecord.UserName = LoginRegisterDataLine[1];
        LoginRegisterRecord.Password = LoginRegisterDataLine[2];
        LoginRegisterRecord.Permissions = stoi(LoginRegisterDataLine[3]);

        return LoginRegisterRecord;

    }

    string _PrepareLogInRecord(string Seperator = "#//#")
    {
        string LoginRecord = "";
        LoginRecord += Core::Date::GetSystemDateTime() + Seperator;
        LoginRecord += UserName + Seperator;
        LoginRecord += Bank::Security::PasswordHasher::HashIfNeeded(Password) + Seperator;
        LoginRecord += to_string(Permissions);
        return LoginRecord;
    }

    static User _GetEmptyUserObject()
    {
        return User(enMode::EmptyMode, "", "", "", "", "", "", 0);
    }

public:

    enum enMode { EmptyMode = 0, UpdateMode = 1, AddNewMode = 2 };

    enum enPermissions {
        eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
		pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64, pLoginRegister = 128
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

    bool IsEmpty() const
    {
        return (_Mode == enMode::EmptyMode);
    }

    bool MarkedForDeleted() const
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

    static User Find(string UserName);
    static User Find(string UserName, string Password);

    enum enSaveResults { svFaildEmptyObject = 0, svSucceeded = 1, svFaildUserExists = 2 };

    enSaveResults Save();

    static bool IsUserExist(string UserName);

    bool Delete();

    static User GetAddNewUserObject(string UserName)
    {
        return User(enMode::AddNewMode, "", "", "", "", UserName, "", 0);
    }

    static vector <User> GetUsersList();

    bool CheckAccessPermission(enPermissions Permission) const
    {
        if (this->Permissions == enPermissions::eAll)
            return true;

        if ((Permission & this->Permissions) == Permission)
            return true;
        else
            return false;

    }

    void RegisterLogIn()
    {

        string stDataLine = _PrepareLogInRecord();

        Bank::Persistence::AtomicFileStore::AppendLine("LoginRegister.txt", stDataLine);

    }

    static  vector <stLoginRegisterRecord> GetLoginRegisterList()
    {
        vector <stLoginRegisterRecord> vLoginRegisterRecord;

        for (const string& Line : Bank::Persistence::AtomicFileStore::LoadLines("LoginRegister.txt"))
        {
            stLoginRegisterRecord LoginRegisterRecord = _ConvertLogInRegisterLineToRecord(Line);

            // skip malformed records
            if (LoginRegisterRecord.DateTime.empty())
                continue;

            vLoginRegisterRecord.push_back(LoginRegisterRecord);
        }

        return vLoginRegisterRecord;

    }

private:
    enMode _Mode;
    string _UserName;
    string _Password;
    int _Permissions;
};

// Persistence is delegated to UserRepository below. The header is included
// after the class so the include graph has no cycles: UserRepository.h only
// consumes the fully-defined User type.
#include "Repositories/UserRepository.h"

inline User User::Find(string UserName)
{
    return Bank::Data::UserRepository::FindByUserName(UserName);
}

inline User User::Find(string UserName, string Password)
{
    return Bank::Data::UserRepository::FindByUserNameAndPassword(UserName, Password);
}

inline User::enSaveResults User::Save()
{
    switch (_Mode)
    {
    case enMode::EmptyMode:
        return enSaveResults::svFaildEmptyObject;

    case enMode::UpdateMode:
    {
        vector <User> users = Bank::Data::UserRepository::LoadAll().Records;
        bool updated = false;
        for (User& user : users)
        {
            if (user.UserName == _UserName)
            {
                user = *this;
                updated = true;
                break;
            }
        }
        if (!updated)
            return enSaveResults::svFaildEmptyObject;

        return Bank::Data::UserRepository::SaveAll(users)
            ? enSaveResults::svSucceeded : enSaveResults::svFaildEmptyObject;
    }

    case enMode::AddNewMode:
    {
        //This will add new record to file or database
        if (Bank::Data::UserRepository::Exists(_UserName))
            return enSaveResults::svFaildUserExists;

        vector <User> users = Bank::Data::UserRepository::LoadAll().Records;
        users.push_back(*this);

        if (!Bank::Data::UserRepository::SaveAll(users))
            return enSaveResults::svFaildEmptyObject;

        //We need to set the mode to update after add new
        _Mode = enMode::UpdateMode;
        return enSaveResults::svSucceeded;
    }
    }
    return enSaveResults::svFaildEmptyObject;
}

inline bool User::IsUserExist(string UserName)
{
    return Bank::Data::UserRepository::Exists(UserName);
}

inline bool User::Delete()
{
    if (!Bank::Data::UserRepository::Exists(_UserName))
        return false;

    vector <User> users = Bank::Data::UserRepository::LoadAll().Records;
    bool deleted = false;
    for (User& user : users)
    {
        if (user.UserName == _UserName)
        {
            user._MarkedForDelete = true;
            deleted = true;
            break;
        }
    }

    if (!deleted || !Bank::Data::UserRepository::SaveAll(users))
        return false;

    *this = _GetEmptyUserObject();
    return true;
}

inline vector <User> User::GetUsersList()
{
    return Bank::Data::UserRepository::LoadAll().Records;
}