#include "message.h"

Message::Message(const char* sndr, const char* rcvr, const char* cntnt, int t)
    : type(t) {
    sender = new char[strlen(sndr) + 1];
    receiver = new char[strlen(rcvr) + 1];
    content = new char[strlen(cntnt) + 1];
    manual_strcpy(sender, sndr);
    manual_strcpy(receiver, rcvr);
    manual_strcpy(content, cntnt);
}

Message::~Message() {
    delete[] sender;
    delete[] receiver;
    delete[] content;
}

InfoMessage::InfoMessage(const char* sndr, const char* rcvr, const char* cntnt)
    : Message(sndr, rcvr, cntnt, MessageType::INFO) {
}

void InfoMessage::deliver() {
    cout << "[INFO] From: " << sender << "\n" << content << "\n";
}

void InfoMessage::saveToFile() {
    char filename[64] = "inbox_";
    manual_strcat(filename, receiver);
    manual_strcat(filename, ".txt");
    ofstream fout(filename, ios::app);
    fout << "[INFO] From: " << sender << "\n" << content << "\n\n";
    fout.close();
}

AlertMessage::AlertMessage(const char* sndr, const char* rcvr, const char* cntnt)
    : Message(sndr, rcvr, cntnt, MessageType::ALERT) {
}

void AlertMessage::deliver() {
    cout << "[ALERT] From: " << sender << "\n" << content << "\n";
}

void AlertMessage::saveToFile() {
    char filename[64] = "inbox_";
    manual_strcat(filename, receiver);
    manual_strcat(filename, ".txt");
    ofstream fout(filename, ios::app);
    fout << "[ALERT] From: " << sender << "\n" << content << "\n\n";
    fout.close();
}

PrivateMessage::PrivateMessage(const char* sndr, const char* rcvr, const char* cntnt)
    : Message(sndr, rcvr, cntnt, MessageType::PRIVATE) {
    encryptContent();
}

PrivateMessage::~PrivateMessage() {
    delete[] encryptedContent;
}

void PrivateMessage::encryptContent() {
    int len = strlen(content);
    encryptedContent = new char[len + 1];
    for (int i = 0; i < len; ++i)
        encryptedContent[i] = content[i] + 1;
    encryptedContent[len] = '\0';
}

void PrivateMessage::decryptContent(char* output) {
    int len = strlen(encryptedContent);
    for (int i = 0; i < len; ++i)
        output[i] = encryptedContent[i] - 1;
    output[len] = '\0';
}

void PrivateMessage::deliver() {
    char decrypted[1024];
    decryptContent(decrypted);
    cout << "[PRIVATE] From: " << sender << "\n" << decrypted << "\n";
}

void PrivateMessage::saveToFile() {
    char filename[64] = "inbox_";
    manual_strcat(filename, receiver);
    manual_strcat(filename, ".txt");
    ofstream fout(filename, ios::app);
    fout << "[PRIVATE] From: " << sender << "\n" << encryptedContent << "\n\n";
    fout.close();
}
