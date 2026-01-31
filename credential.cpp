#include "credential.h"
#include "otp.h"
#include "string_utils.h"
#include "audit.h"
#include <fstream>
#include <cstring>
#include <conio.h> // For getch
#include <iostream>
using namespace std;

void CredentialManager::caesarDecrypt(char* text, int key) {
    for (int i = 0; text[i] != '\0'; i++) {
        text[i] -= key;
    }
}

Employee* CredentialManager::createUserByRole(const char* uname, const char* pass, const char* role) {
    if (manual_strcmp(role, "Junior") == 0) return new Junior(uname, pass);
    if (manual_strcmp(role, "Manager") == 0) return new Manager(uname, pass);
    if (manual_strcmp(role, "Director") == 0) return new Director(uname, pass);
    if (manual_strcmp(role, "Executive") == 0) return new Executive(uname, pass);
    return nullptr;
}

Employee* CredentialManager::loginSystem() {
    char uname[50], pass[50];
    int attempts = 0;

    while (attempts < 3) {
        cout << "\033[38;5;46mUsername: \033[0m";
        cin.getline(uname, sizeof(uname));
        cout << "\033[38;5;46mPassword: \033[0m";

        // Custom password input with asterisks
        int i = 0;
        char ch;
        while (i < 49) {
            ch = getch();
            if (ch == '\r' || ch == '\n') { // Enter key
                pass[i] = '\0';
                break;
            }
            else if (ch == 8 && i > 0) { // Backspace
                i--;
                cout << "\b \b";
            }
            else if (ch != 8) { // Normal character
                pass[i++] = ch;
                cout << '*';
            }
        }
        pass[i] = '\0';
        cout << endl;

        ifstream fin("users.txt");
        if (!fin) {
            cout << "Error opening users.txt\n";
            return nullptr;
        }

        char line[200]; // Increased buffer size
        bool found = false;
        while (fin.getline(line, sizeof(line))) {
            char storedUser[50] = { 0 }, storedPass[50] = { 0 }, role[20] = { 0 };
            int linePos = 0, fieldPos = 0;

            // Extract storedUser (until first comma)
            while (line[linePos] != ',' && line[linePos] != '\0') {
                storedUser[fieldPos++] = line[linePos++];
            }
            storedUser[fieldPos] = '\0';
            if (line[linePos] == ',') linePos++; // Skip comma
            fieldPos = 0;

            // Extract storedPass (until second comma)
            while (line[linePos] != ',' && line[linePos] != '\0') {
                storedPass[fieldPos++] = line[linePos++];
            }
            storedPass[fieldPos] = '\0';
            if (line[linePos] == ',') linePos++; // Skip comma
            fieldPos = 0;

            // Extract role (rest of the line)
            while (line[linePos] != '\0' && line[linePos] != '\r' && line[linePos] != '\n') {
                role[fieldPos++] = line[linePos++];
            }
            role[fieldPos] = '\0';

            // Decrypt the stored credentials
            caesarDecrypt(storedUser, 3);
            caesarDecrypt(storedPass, 3);

          
    

            if (manual_strcmp(storedUser, uname) == 0 && manual_strcmp(storedPass, pass) == 0) {
                int otp = generateOTP();
                if (!verifyOTP(otp)) {
                    AuditLogger::log(uname, "LOGIN", "FAILED_OTP");
                    fin.close();
                    return nullptr;
                }
                AuditLogger::log(uname, "LOGIN", "SUCCESS");
                fin.close();
                return createUserByRole(uname, pass, role);
            }
        }
        fin.close();

        attempts++;
        cout << "\033[38;5;196m Invalid credentials. Attempts left: " << (3 - attempts) << "\033[0m\n";
        AuditLogger::log(uname, "LOGIN", "FAILED_CREDENTIALS");
    }

    return nullptr;
}