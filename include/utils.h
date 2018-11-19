#ifndef UTILS_H
#define UTILS_H

long double randomDouble();

long double randomDoubleWithout1();

long double randomDoubleWithout0();

/**
 * Checks if the string is a correct integer.
 * @param s String.
 * @return True if it is, false otherwise.
 */
bool isInteger(const char* s);

/**
 * Checks if the string is a correct double.
 * @param s String.
 * @return True if yes, false otherwise.
 */
bool isDouble(const char* s);

/**
 * Checks if the string is a correct boolean.
 * @param s String.
 * @return True if yes (s = "T" or s = "F"), false otherwise.
 */
bool isBoolean(const char* s);

#endif
