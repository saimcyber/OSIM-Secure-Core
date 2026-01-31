#ifndef AUDIT_H
#define AUDIT_H

#include <iostream>
using namespace std;

class AuditLogger {
public:
    static void log(const char* username, const char* action, const char* status);
};

#endif // AUDIT_H
