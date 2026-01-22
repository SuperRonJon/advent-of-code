#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<uint32_t> add_numbers(std::string line) {
    std::vector<uint32_t> numbers;
    std::string curr_num;
    for(std::size_t i = 0; i < line.size(); i++) {
        if(line[i] == ' ') {
            if(!curr_num.empty()) {
                numbers.push_back(std::stoi(curr_num));
                curr_num.clear();
            }
        }
        else {
            curr_num += line[i];
        }
    }
    if(!curr_num.empty()) {
        numbers.push_back(std::stoi(curr_num));
    }
    return numbers;
}

void add_operations(std::vector<char> &operations, std::string line) {
    for(std::size_t i = 0; i < line.size(); i++) {
        if(line[i] != ' ') {
            operations.push_back(line[i]);
        }
    }
}

std::uint64_t do_problems(std::vector<std::vector<uint32_t>> numbers, std::vector<char> operations) {
    std::uint64_t result{0ULL};
    for(std::size_t i = 0; i < operations.size(); i++) {
        std::uint64_t current_result{numbers[0][i]};
        for(std::size_t j = 1; j < numbers.size(); j++) {
            if(operations[i] == '+') {
                current_result += numbers[j][i];
            }
            else if(operations[i] == '*') {
                current_result *= numbers[j][i];
            }
        }
        result += current_result;
    }
    return result;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cerr << "Must give input filename\n";
        return 1;
    }
    std::vector<std::vector<uint32_t>> numbers;
    std::vector<char> operations;
    std::string filename{argv[1]};
    std::ifstream in_file{filename};
    bool get_width = true;
    std::string line;
    std::size_t width = 0;
    while(std::getline(in_file, line)) {
        if(line[0] != '+' && line[0] != '*') {
            numbers.push_back(add_numbers(line));
            if(get_width) {
                width = numbers[0].size();
                get_width = false;
            }
        }
        else {
            add_operations(operations, line);
        }
    }
    std::uint64_t result = do_problems(numbers, operations);
    std::cout << "Result: " << result << "\n";
    return 0;
}