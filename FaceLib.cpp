#include <algorithm>
#include <string>
#include <iostream>

void ReverseString(std::string& s) {
    std::reverse(s.begin(), s.end());
    std::cout << "Reversed string: " << s << std::endl;
}

void StringLength(const std::string& s) {
    std::cout << "Length of string: " << s.length() << std::endl;
}