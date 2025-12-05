#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

enum Direction {
    RIGHT,
    LEFT
};

struct Rotation {
    Direction direction;
    unsigned int amount;
};

Rotation rotation_from_string(const std::string &s) {
    Rotation new_rotation;
    if(s[0] == 'L') {
        new_rotation.direction = LEFT;
    }
    else if(s[0] == 'R') {
        new_rotation.direction = RIGHT;
    }
    else {
        throw std::invalid_argument("Invalid direction, must be L or R.");
    }
    new_rotation.amount = std::stoi(s.substr(1));
    return new_rotation;
}

unsigned int modulo(int a, int b) {
    return (b + (a % b)) % b;
}

unsigned int make_rotation(int start_value, Rotation rotation) {
    unsigned int result_value = 0;
    int raw_value = 0;
    switch(rotation.direction) {
        case LEFT:
            raw_value = start_value - rotation.amount;
            result_value = modulo(raw_value, 100);
            break;
        case RIGHT:
            raw_value = start_value + rotation.amount;
            result_value = modulo(raw_value, 100);
            break;
        default:
            throw std::invalid_argument("Invalid rotation");
    }
    return result_value;
}

int main(int argc, char **argv) {
    if(argc == 2) {
        std::string input_file_name{argv[1]};
        std::ifstream input_file{input_file_name};
        std::string current_rotation_string;
        unsigned int dial_value = 50;
        unsigned int zero_count = 0;
        while(input_file >> current_rotation_string) {
            Rotation current_rotation = rotation_from_string(current_rotation_string);
            dial_value = make_rotation(dial_value, current_rotation);
            if(dial_value == 0) {
                zero_count++;
            }
        }
        std::cout << "Password: " << zero_count << "\n";
        return 0;
    }
}