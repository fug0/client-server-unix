#ifndef DATA_RESOLVER_HPP
#define DATA_RESOLVER_HPP

#include <string>
#include <algorithm>
#include <iostream>
#include <atomic>

class DataResolver {
public:
    static void InputStrung(std::string& str, std::atomic<bool>& is_conn);
    static void StringFormat(std::string& str);
    static uint16_t CountStrNumsSum(std::string &str);
    
private:
    static bool IsOnlyDigits(std::string& str);
};

#endif /* DATA_RESOLVER_HPP */