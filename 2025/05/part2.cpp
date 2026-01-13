#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct range {
    std::uint64_t min;
    std::uint64_t max;

    bool operator <(const range &other) const {
        return min < other.min;
    }
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

std::uint64_t count_ids_in_ranges(std::vector<range> ranges) {
    std::uint64_t count{0};
    for(std::size_t i = 0; i < ranges.size(); i++) {
        count += (ranges[i].max - ranges[i].min) + 1;
    }
    return count;
}

std::vector<range> merge_ranges(std::vector<range> &ranges) {
    std::sort(ranges.begin(), ranges.end());
    std::vector<range> merged;
    if(ranges.size() == 0) {
        return merged;
    }
    merged.push_back(ranges[0]);
    for(std::size_t i = 1; i < ranges.size(); i++) {
        range& last_range = merged[merged.size() - 1];
        if(ranges[i].min <= last_range.max) {
            if(ranges[i].max > last_range.max){
                last_range.max = ranges[i].max;
            }
        }
        else {
            merged.push_back(ranges[i]);
        }
    }
    return merged;
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
    while(std::getline(in_file, line)) {
        if(line == "") {
            get_ranges = false;
            continue;
        }
        if(get_ranges) {
            ranges.push_back(to_range(line));
        }
    }
    std::vector<range> merged_ranges = merge_ranges(ranges);
    std::uint64_t fresh_count = count_ids_in_ranges(merged_ranges);
    std::cout << "Fresh ingredients: " << fresh_count << "\n";
    return 0;
}