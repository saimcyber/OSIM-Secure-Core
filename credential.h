#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#include "employee.h"
using namespace std;

class CredentialManager {
private:
    void caesarDecrypt(char* text, int key);
    Employee* createUserByRole(const char* uname, const char* pass, const char* role);

public:
    Employee* loginSystem();
};

#endif // CREDENTIAL_H
