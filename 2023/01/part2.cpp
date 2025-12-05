// Advent of code Day 1 Part 2 solution
// Patrick Seute

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

bool substr_is_digit(const std::string &full_string, std::size_t start_index, char &character_found) {
    static const std::string digit_strings[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    int index_found = -1;
    //For each word in digit_strings
    for(std::size_t j = 0; j < 9; j++) {
        //For each letter in the word
        bool full_match = true;
        for(std::size_t k = 0; k < digit_strings[j].length(); k++) {
            //If this character doesn't match the substring at the same index break to next word in list
            if(digit_strings[j][k] != full_string[start_index+k]) {
                full_match = false;
                break;
            }
        }
        //If it makes it all the way through the word without breaking it is a match, save it and break
        if(full_match) {
            index_found = j;
            break;
        }
    }
    //If there was a match found, set the character and return true
    if(index_found != -1) {
        character_found =  (index_found+1) + '0';
        return true;
    }
    return false;
}

int get_calibration_value(const std::string &text) {
    std::string result_text = "";
    bool found_first = false;
    char last_found;
    char digit_char_found;
    for(std::size_t i = 0; i < text.length(); i++) {
        const char curr = text[i];
        //If the character is a digit, 
        if('0' <= curr && curr <= '9') {
            //add it if it's the first, otherwise save as last
            if(!found_first) {
                result_text += curr;
                found_first = true;
            }
            last_found = curr;
        }
        //else if this character is the start of a word that represents a digit
        else if(substr_is_digit(text, i, digit_char_found)) {
            //add it if it's the first, otherwise save as last
            if(!found_first) {
                result_text += digit_char_found;
                found_first = true;
            }
            last_found = digit_char_found;
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
        std::ifstream in_file(filename);
        std::string line;
        int sum = 0;
        while(in_file >> line) {
            int calibration_value = get_calibration_value(line);
            if(calibration_value != -1) {
                sum += calibration_value;
            } else {
                std::cerr << "No digit in line " << line << '\n';
            }
        }
        std::cout << "Sum: " << sum << "\n";
    }
    return 0;
}
