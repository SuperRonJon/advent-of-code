#include <fstream>
#include <iostream>
#include <string>

int get_calibration_value(std::string text) {
    std::string result_text = "";
    bool found_first = false;
    char last_found;
    for(int i = 0; i < text.length(); i++) {
        const char curr = text[i];
        if('0' <= curr && curr <= '9') {
            if(!found_first) {
                result_text += curr;
                found_first = true;
            }
            last_found = curr;
        }
    }
    if(!found_first) {
        return -1;
    }
    result_text += last_found;
    return std::stoi(result_text);
}

int main(int argc, char** argv) {
    if(argc == 2) {
        std::string filename = argv[1];
        std::cout << "Opening " << filename << "\n";
        std::ifstream in_file(filename);
        std::string line;
        int sum = 0;
        while(in_file >> line) {
            int calibration_value = get_calibration_value(line);
            if(calibration_value == -1) {
                std::cerr << "No digit in line " << line << '\n';
            } else {
                sum += calibration_value;
            }
        }
        std::cout << "Sum: " << sum << "\n";
    }
    return 0;
}
