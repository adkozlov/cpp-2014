#pragma once

#include <cstring>

inline bool argument_equals(const char* argument, const char* key) {
    return strcmp(argument, key) == 0;
}

inline bool argument_equals(const char* argument, const char* key1, const char* key2)
{
    return argument_equals(argument, key1) || argument_equals(argument, key2);
}

inline bool is_enough_arguments_for_key(const char* argument, const char* key1, const char* key2, const int index, const int argc)
{
    return argument_equals(argument, key1, key2) && (index != argc - 1);
}

inline bool is_not_enough_arguments_for_key(const char* argument, const char* key1, const char* key2, const int index, const int argc)
{
    return argument_equals(argument, key1, key2) && (index == argc - 1);
}