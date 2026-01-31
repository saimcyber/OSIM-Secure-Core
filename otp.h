#ifndef OTP_H
#define OTP_H

#include <ctime>
#include <iostream>
using namespace std;

int generateOTP();
bool verifyOTP(int generatedOTP, int timeoutSeconds = 30);

#endif // OTP_H
