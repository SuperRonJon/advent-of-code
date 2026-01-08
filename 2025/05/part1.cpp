#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct range {
    std::uint64_t min;
    std::uint64_t max;
};

range to_range(std::string s) {
    std::string first_value;
    std::size_t i{0};
    while(s[i] != '-') {
        first_value += s[i];
        i++;
    }
    i++;
    std::string second_value;
    while(i < s.size()) {
        second_value += s[i];
        i++;
    }
    range result{std::stoull(first_value), std::stoull(second_value)};
    return result;
}

bool id_in_range(std::uint64_t id, std::vector<range> fresh_range) {
    for(std::size_t i = 0; i < fresh_range.size(); i++) {
        if(fresh_range[i].min <= id && id <= fresh_range[i].max) {
            return true;
        }
    }
    return false;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cerr << "Please give input filename as command line argument\n";
        return 1;
    }
    std::string filename{argv[1]};
    std::ifstream in_file{filename};
    bool get_ranges = true;
    std::string line;
    std::vector<range> ranges;
    std::uint64_t fresh_count{0};
    while(std::getline(in_file, line)) {
        if(line == "") {
            get_ranges = false;
            continue;
        }
        if(get_ranges) {
            ranges.push_back(to_range(line));
        }
        else {
            std::uint64_t item_id = std::stoull(line);
            if(id_in_range(item_id, ranges)) {
                fresh_count++;
            }
        }
    }
    std::cout << "Fresh ingredients: " << fresh_count << "\n";
    return 0;
}