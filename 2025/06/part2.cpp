#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Problem {
public:
    Problem(const std::vector<std::string> &number_strings, const char operation) {
        set_operation(operation);
        for(std::size_t i = 0; i < number_strings[0].size(); i++) {
            std::string curr_num;
            for(std::size_t j = 0; j < number_strings.size(); j++) {
                if(number_strings[j][i] != ' ') {
                    curr_num += number_strings[j][i];
                }
            }
            m_numbers.push_back(std::stoul(curr_num));
        }
    }
    std::uint64_t solve() const {
        if(m_numbers.empty()) {
            return 0ULL;
        }
        std::uint64_t result = m_numbers[0];
        for(std::size_t i = 1; i < m_numbers.size(); i++) {
            switch(m_operation) {
                case ADD:
                    result += m_numbers[i];
                    break;
                case MULTIPLY:
                    result *= m_numbers[i];
                    break;
            }
        }
        return result;
    }

private:
    enum Operation {
        ADD,
        MULTIPLY
    };
    void set_operation(char c) {
        switch(c) {
            case '+':
                m_operation = ADD;
                break;
            case '*':
                m_operation = MULTIPLY;
                break;
            default:
                break;
        }
    }
    std::vector<std::uint32_t> m_numbers;
    Operation m_operation;
};

std::vector<Problem> create_problems(const std::vector<std::string> &lines, const std::vector<char> &operations) {
    std::size_t problems_added{0};
    std::size_t current_start = 0;
    std::vector<Problem> problems;
    for(std::size_t i = 0; i < lines[0].size(); i++) {
        bool is_problem_end = true;
        for(const std::string &line : lines) {
            if(line[i] != ' ') {
                is_problem_end = false;
                break;
            }
        }
        if(is_problem_end || i == lines[0].size() - 1) {
            if(i == lines[0].size() - 1) {
                i++;
            }
            std::vector<std::string> num_strings;
            for(std::size_t j = 0; j < lines.size(); j++) {
                num_strings.push_back(lines[j].substr(current_start, i - current_start));
            }
            current_start = i + 1;
            Problem new_problem(num_strings, operations[problems_added]);
            problems_added++;
            problems.push_back(new_problem);
        }
    }    
    return problems;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cerr << "Must give input filename\n";
        return 1;
    }
    std::vector<std::string> lines;
    std::vector<char> operations;
    const std::string filename{argv[1]};
    std::ifstream in_file{filename};
    std::string line;
    while(std::getline(in_file, line)) {
        if(line[0] != '+' && line[0] != '*') {
            lines.push_back(line);
        }
        else {
            for(const char c : line) {
                if(c != ' ') {
                    operations.push_back(c);
                }
            }
        }
    }
    const std::vector<Problem> problems = create_problems(lines, operations);
    std::uint64_t total_result{0};
    for(const Problem &problem : problems) {
        total_result += problem.solve();
    }
    std::cout << "Result: " << total_result << "\n";
    return 0;
}