#include <fstream>
#include <iostream>
#include <string>
#include <cstddef>

int get_joltage(const std::string &batteries) {
    std::string joltage_string;
    std::size_t first_largest_digit_index{0};
    int current_largest = 0;

    for(std::size_t i = 0; i < batteries.size() - 1; i++) {
        int current_digit = batteries[i] - '0';
        if(current_digit > current_largest) {
            current_largest = current_digit;
            first_largest_digit_index = i;
        }
    }

    joltage_string += '0' + current_largest;
    current_largest = 0;
    for(std::size_t i = first_largest_digit_index + 1; i < batteries.size(); i++) {
        int current_digit = batteries[i] - '0';
        if(current_digit > current_largest) {
            current_largest = current_digit;
        }
    }
    joltage_string += '0' + current_largest;
    return std::stoi(joltage_string);
}

int main(int argc, char **argv) {
    if(argc == 2) {
        std::string filename = argv[1];
        std::ifstream in_file{filename};
        std::string line;
        int line_number = 1;
        long long joltage_sum = 0;
        while(in_file >> line) {
            int current_joltage = get_joltage(line);
            joltage_sum += current_joltage;
        }
        std::cout << "Total joltage: " << joltage_sum << "\n";
    }
}