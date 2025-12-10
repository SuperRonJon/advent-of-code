#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>


uint64_t get_joltage(const std::string &batteries, uint32_t joltage_digits) {
    std::string joltage_string;
    std::size_t start_index{0};
    std::size_t num_digits_found{0};

    for(uint32_t i = 0; i < joltage_digits; i++) {
        int current_largest = 0;
        std::size_t max_index = batteries.size() - (joltage_digits - 1 - num_digits_found);
        for(std::size_t j = start_index; j < max_index; j++) {
            int current_digit = batteries[j] - '0';
            if(current_digit > current_largest) {
                current_largest = current_digit;
                start_index = j+1;
            }
        }
        num_digits_found++;
        joltage_string += '0' + current_largest;
    }

    return std::stoull(joltage_string);
}

int main(int argc, char **argv) {
    if(argc == 2) {
        std::string filename = argv[1];
        std::ifstream in_file{filename};
        std::string line;
        uint64_t joltage_sum = 0;
        while(in_file >> line) {
            joltage_sum += get_joltage(line, 12);
        }
        std::cout << "Total joltage: " << joltage_sum << "\n";
    }
}