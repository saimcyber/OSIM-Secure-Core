#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <ctime>
#include <string>

class TimeManager {
public:
    // Check if a task has expired based on its TTL
    static bool isTaskExpired(const char* taskCreationTime, const char* ttl);
    
    // Convert TTL string to seconds (format: 1d, 2h, 30m, etc.)
    static int convertTTLToSeconds(const char* ttl);
    
    // Calculate expiration time based on creation time and TTL
    static time_t calculateExpirationTime(const char* creationTime, const char* ttl);
    
    // Format: returns time string in YYYY-MM-DD HH:MM:SS format
    static std::string getFormattedTime(time_t timeValue);
    
    // Get current time as formatted string
    static std::string getCurrentTimeString();
};

#endif // TIME_MANAGER_H