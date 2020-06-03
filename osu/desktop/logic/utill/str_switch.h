#ifndef STR_SWITCH_H
#define STR_SWITCH_H

#include <string>

#define sswitch(str)  switch(s_s::str_hash_for_switch(str))
#define scase(str)    static_assert(s_s::str_is_correct(str),\
"CASE string contains wrong characters");\
case s_s::str_hash(str, s_s::str_len(str))
#define sdefault  default


namespace s_s {
    typedef unsigned char uchar;
    typedef unsigned long long ullong;

    const ullong N_HASH = static_cast<ullong>(-1);

    constexpr bool str_is_correct(const char *const str) {
        return (static_cast<signed char>(*str) > 0) ? str_is_correct(str + 1) : (*str ? false : true);
    }

    constexpr uchar str_len(const char *const str) {
        return *str ? (1 + str_len(str + 1)) : 0;
    }

    constexpr ullong str_hash(const char *const str, const uchar current_len) {
        return *str ? (static_cast<signed char>(*str) & 0xff) + 31 * str_hash(str + 1, current_len - 1) : 0;
    }

    inline ullong str_hash_for_switch(const char *const str) {
        return (str_is_correct(str)) ? str_hash(str, str_len(str)) : N_HASH;
    }

    inline ullong str_hash_for_switch(const std::string *str) {
        return (str_is_correct(str->c_str())) ? str_hash(str->c_str(), str->length()) : N_HASH;
    }

    inline ullong str_hash_for_switch(const std::string &str) {
        return (str_is_correct(str.c_str())) ? str_hash(str.c_str(), str.length()) : N_HASH;
    }
}

#endif  // STR_SWITCH_H