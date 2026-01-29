#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

int count_beams(const std::vector<std::string> &lines) {
    int count = 0;
    if(lines.empty()) {
        return count;
    }
    std::unordered_set<std::size_t> beam_locations;
    for(const std::string &line : lines) {
        for(std::size_t i = 0; i < line.size(); i++) {
            if(line[i] == 'S') {
                beam_locations.insert(i);
            }
            if(line[i] == '^') {
                if(beam_locations.find(i) != beam_locations.end()) {
                    count++;
                    beam_locations.erase(i);
                    beam_locations.insert(i - 1);
                    beam_locations.insert(i + 1);
                }
            }
        }
    }
    return count;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cerr << "No input file given\n";
        return 1;
    }

    std::string filename{argv[1]};
    std::ifstream in_file{filename};

    std::string line;
    std::vector<std::string> lines;
    while(std::getline(in_file, line)) {
        lines.push_back(line);
    }
    int beam_count = count_beams(lines);
    std::cout << "Result: " << beam_count << "\n";
    return 0;
}