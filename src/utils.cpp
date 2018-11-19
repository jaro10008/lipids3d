#include"utils.h"
#include<cstdlib>
#include<cctype>

long double randomDouble() {
    return ((long double) rand()) / RAND_MAX;
}

long double randomDoubleWithout1() {
    return ((long double) rand()) / ((long double) RAND_MAX + 1);
}

long double randomDoubleWithout0() {
    return ((long double) rand() + 1) / ((long double) RAND_MAX + 1);
}

bool isInteger(const char* s) {
    int i = -1;
    while (s[++i]) {
        if (!isdigit(s[i])) {
            if (!i && (s[0] == '+' || s[0] == '-') && s[1])
                continue;
            return false;
        }
    }

    if (!i)
        return false;

    return true;
}

bool isDouble(const char* s) {
    int i = -1;
    bool separator = false;

    while (s[++i]) {
        if (!isdigit(s[i])) {
            if (!i && (s[0] == '+' || s[0] == '-') && s[1])
                continue;

            if (s[i] == '.' && i && isdigit(s[i - 1]) && s[i + 1] && !separator) {
                separator = true;
                continue;
            }
            return false;
        }
    }

    if (!i)
        return false;

    return true;
}

bool isBoolean(const char* s) {
    if (!s[0])
        return false;
    return (s[0] == 'T' || s[0] == 'F') && !s[1];
}
