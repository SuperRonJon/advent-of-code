#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

struct Range {
    unsigned long long start_value;
    unsigned long long end_value;
};

Range range_from_string(const std::string &s) {
    std::string start_value;
    std::string end_value;

    std::size_t i = 0;
    while(s[i] != '-') {
        start_value += s[i];
        i++;
    }
    end_value = s.substr(i+1);
    Range r{ std::stoull(start_value), std::stoull(end_value) };
    return r;
}

bool is_invalid_id(const std::string &id) {
    if(id.size() % 2 != 0) {
        return false;
    }
    const std::size_t half_length = id.size() / 2;
    const std::string first_half = id.substr(0, half_length);
    const std::string second_half = id.substr(half_length);

    return first_half == second_half;
}

unsigned long long sum_invalid_ids_in_range(const Range &r) {
    unsigned long long sum = 0;
    for(unsigned long long i = r.start_value; i <= r.end_value; i++) {
        if(is_invalid_id(std::to_string(i))) {
            sum += i;
        }
    }
    return sum;
}

int main(int argc, char **argv) {
    if(argc == 2) {
        std::string input_file_name{argv[1]};
        std::ifstream input_file{input_file_name};
        std::string full_input;
        input_file >> full_input;
        unsigned long long invalid_sum = 0;
        
        std::string current_range_string;
        for(std::size_t i = 0; i < full_input.size(); i++) {
            if(full_input[i] == ',') {
                Range current_range = range_from_string(current_range_string);
                invalid_sum += sum_invalid_ids_in_range(current_range);
                current_range_string = "";
            }
            else {
                current_range_string += full_input[i];
            }
        }
        if(!current_range_string.empty()) {
            Range current_range = range_from_string(current_range_string);
            invalid_sum += sum_invalid_ids_in_range(current_range);
        }
        std::cout << "Sum of invalid ids: " << invalid_sum << "\n";
    }
    return 0;
}