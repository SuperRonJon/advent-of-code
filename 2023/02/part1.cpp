#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

enum Color {
    RED,
    GREEN,
    BLUE,
    NONE
};

struct game_maxes {
    int red;
    int green;
    int blue;
};

struct cube_count {
    int count;
    Color color;
};

Color color_from_string(std::string s) {
    if(s == "red") {
        return RED;
    }
    if(s == "green") {
        return GREEN;
    }
    if(s == "blue") {
        return BLUE;
    }
    return NONE;
    
}

cube_count count_from_string(std::string s) {
    cube_count count;
    std::string number_string;
    std::size_t i;
    for(i = 0; i < s.length(); i++) {
        if(s[i] == ' ') {
            count.count = std::stoi(number_string);
            break;
        } else {
            number_string += s[i];
        }
    }

    std::string color_string = s.substr(i+1, s.length() - i+1);
    count.color = color_from_string(color_string);
    return count;
}

std::vector<std::string> split(std::string s, char delim) {
    std::vector<std::string> result;
    std::string curr = "";
    bool skip_whitespace = false;
    for(std::size_t i = 0; i < s.length(); i++) {
        if(s[i] == delim && curr != "") {
            result.push_back(curr);
            curr = "";
            skip_whitespace = true;
        }
        else if(s[i] == ' ' && skip_whitespace) {
            continue;
        }
        else {
            skip_whitespace = false;
            curr += s[i];
        }
    }
    result.push_back(curr);
    return result;
}

game_maxes get_maxes_for_game(std::string game_string) {
    game_maxes max = {0, 0, 0};

    //Separate each game into individual rounds
    std::vector<std::string> game_rounds = split(game_string, ';');
    for(std::string round : game_rounds) {
        //Separate each round into individual cube colors
        std::vector<std::string> cube_counts = split(round, ',');
        for(std::string count_string : cube_counts) {
            //If the current color count is greater than the highest seen, save it.
            cube_count current_count = count_from_string(count_string);
            switch(current_count.color) {
                case RED:
                    if(current_count.count > max.red) {
                        max.red = current_count.count;
                    }
                    break;
                case GREEN:
                    if(current_count.count > max.green) {
                        max.green = current_count.count;
                    }
                    break;
                case BLUE:
                    if(current_count.count > max.blue) {
                        max.blue = current_count.count;
                    }
                    break;
                case NONE:
                    std::cerr << "Error none color found\n";
                    break;
            }
        }
    }

    return max;
}

std::string get_game_content(std::string line) {
    for(std::size_t i = 0; i < line.length(); i++) {
        if(line[i] == ':') {
            return line.substr(i+2, line.length() - (i+2));
        }
    }
    return "";
}

bool is_possible(game_maxes maxes) {
    return (maxes.red <= MAX_RED && maxes.green <= MAX_GREEN && maxes.blue <= MAX_BLUE);
}

bool game_string_is_possible(std::string game) {
    game_maxes maxes = get_maxes_for_game(get_game_content(game));
    return is_possible(maxes);
}

int main(int argc, char** argv) {
    if(argc == 2) {
        int current_game_id = 1;
        int sum = 0;
        std::string filename = argv[1];
        std::ifstream in_file(filename);
        std::string line = "";
        while(std::getline(in_file, line)) {
            if(game_string_is_possible(line)) {
                sum += current_game_id;
            }
            current_game_id++;
        }
        std::cout << "Sum: " << sum << "\n";
    }
    return 0;
}