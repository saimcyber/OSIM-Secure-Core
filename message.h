#ifndef MESSAGE_H
#define MESSAGE_H

#include <fstream>
#include <iostream>
#include <cstring>
#include "string_utils.h"
using namespace std;

class MessageType {
public:
    static const int INFO = 0;
    static const int PRIVATE = 1;
    static const int ALERT = 2;
};

class Message {
protected:
    char* sender;
    char* receiver;
    char* content;
    int type;

public:
    Message(const char* sndr, const char* rcvr, const char* cntnt, int t);
    virtual ~Message();
    virtual void deliver() = 0;
    virtual void saveToFile() = 0;
};

class InfoMessage : public Message {
public:
    InfoMessage(const char* sndr, const char* rcvr, const char* cntnt);
    void deliver() override;
    void saveToFile() override;
};

class AlertMessage : public Message {
public:
    AlertMessage(const char* sndr, const char* rcvr, const char* cntnt);
    void deliver() override;
    void saveToFile() override;
};

class PrivateMessage : public Message {
    char* encryptedContent;
    void encryptContent();
    void decryptContent(char* output);

public:
    PrivateMessage(const char* sndr, const char* rcvr, const char* cntnt);
    ~PrivateMessage();
    void deliver() override;
    void saveToFile() override;
};

#endif // MESSAGE_H
