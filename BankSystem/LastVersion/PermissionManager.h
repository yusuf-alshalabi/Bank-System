#pragma once

//  ||========================================================||
//  || BankSystem Project - Version v1.4.1                    ||
//  || File: PermissionManager.h                              ||
//  || Section: Permission Manager                            ||
//  || Functions to read and check user permissions.          ||
//  ||========================================================||

#include "Globals.h"
#include "InputManager.h"

//=====================================================
//================= Permission Manager ================
//=====================================================

// Read user permissions interactively
int readUserPermissions() {
    int Permissions = 0;

    if (confirmAction("Do you want to give full access? "))
        return Permission::pAll;

    cout << "\nDo you want to give access to : \n ";
    if (confirmAction("Show Client List? "))
        Permissions += Permission::pListClients;
    if (confirmAction("Add New Client? "))
        Permissions += Permission::pAddClient;
    if (confirmAction("Delete Client? "))
        Permissions += Permission::pDeleteClient;
    if (confirmAction("Update Client? "))
        Permissions += Permission::pUpdateClient;
    if (confirmAction("Find Client? "))
        Permissions += Permission::pFindClient;
    if (confirmAction("Transactions? "))
        Permissions += Permission::pTransactions;
    if (confirmAction("Manage Users? "))
        Permissions += Permission::pManageUsers;

    return Permissions;
}
// Check if current user has specific permission
bool hasPermission(Permission permission) {
    if (CurrentUser.Permissions == Permission::pAll)
        return true;

    return (CurrentUser.Permissions & permission) == permission;
}
