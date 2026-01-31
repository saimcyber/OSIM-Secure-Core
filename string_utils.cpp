#include "string_utils.h"

void manual_strcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void manual_strcat(char* dest, const char* src) {
    int i = 0;
    while (dest[i] != '\0') i++;
    int j = 0;
    while (src[j] != '\0') {
        dest[i++] = src[j++];
    }
    dest[i] = '\0';
}

int manual_strcmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

int manual_atoi(const char* str) {
    int result = 0;
    int i = 0;
    bool negative = false;
    if (str[0] == '-') {
        negative = true;
        i = 1;
    }
    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        }
        else break;
        i++;
    }
    return negative ? -result : result;
}

void manual_itoa(int num, char* str) {
    int i = 0;
    bool isNegative = false;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    if (num < 0) {
        isNegative = true;
        num = -num;
    }
    while (num > 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }
    if (isNegative) str[i++] = '-';
    str[i] = '\0';
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}
