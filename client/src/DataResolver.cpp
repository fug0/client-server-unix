#include "../include/DataResolver.hpp"

void DataResolver::InputStrung(std::string& str, std::atomic<bool>& is_conn) {
    while (true) {
        if(is_conn == false) {
            std::cout << "Input string of numbers: ";
            std::cin >> str;
            if(str.length() > 64) {
                std::cout << "String size must be less than 64 characters!" << std::endl;
            } else {
                if(DataResolver::IsOnlyDigits(str)) {
                    break;
                } else {
                    std::cout << "String must contain only characters!" << std::endl;
                }
            }
        }
    }
}

void DataResolver::StringFormat(std::string& str) {
    std::sort(str.begin(), str.end(), std::greater<char>());

	for (auto& ch : str) {
		if(!((ch - '0') & 1)) {
			ch = 'K';
		}
	}

    std::string buf_str;
	for (auto& ch : str) {
        buf_str += ch;
		if(ch == 'K') {
			buf_str += "В";
		}
	}
    str = buf_str;
}

uint16_t DataResolver::CountStrNumsSum(std::string &str) {
    uint16_t sum = 0;

    for (const auto& ch : str){
        if(ch >= '0' && ch <= '9') {
            sum += ch - '0';
        }
    }

    return sum;
}

bool DataResolver::IsOnlyDigits(std::string& str) {
    for (const auto& ch : str) {
        if(ch < '0' || ch > '9')
        {
            return false;
        }
    }

    return true;
}