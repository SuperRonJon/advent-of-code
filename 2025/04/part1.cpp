#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>

class Grid {
public:
    Grid(std::ifstream &in_file) {
        m_width = 0;
        m_height = 0;
        m_values = "";
        if(in_file.is_open()) {
            std::string line;
            while(in_file >> line) {
                if(m_width == 0) {
                    m_width = line.size();
                }
                m_height++;
                m_values += line;
            }
        }
        else {
            std::cerr << "Input file not open...\n";
        }
    }

    const std::size_t width() const {
        return m_width;
    }

    const std::size_t height() const {
        return m_height;
    }

    const std::string& values() const {
        return m_values;
    }

    const char& char_at(const std::size_t &col, const std::size_t &row) const {
        if(col >= m_width || row >= m_height) {
            throw std::out_of_range("character out of range");
        }
        const std::size_t index = (row * m_width) + col;
        return m_values.at(index);
    }

    char& char_at(const std::size_t &col, const std::size_t &row) {
        return const_cast<char &>(
            static_cast<const Grid *>(this)->char_at(col, row)
        );
    }

    int count_neighbors(const std::size_t& col, const std::size_t &row) const {
        int count = 0;
        const std::size_t LEFT = col - 1;
        const std::size_t RIGHT = col + 1;
        const std::size_t UP = row - 1;
        const std::size_t DOWN = row + 1;

        if((LEFT < m_width && UP < m_height) && char_at(LEFT, UP) == '@') {
            count++;
        }
        if(UP < m_height && char_at(col, UP) == '@') {
            count++;
        }
        if((RIGHT < m_width && UP < m_height) && char_at(RIGHT, UP) == '@') {
            count++;
        }
        if(LEFT < m_width && char_at(LEFT, row) == '@') {
            count++;
        }
        if(RIGHT < m_width && char_at(RIGHT, row) == '@') {
            count++;
        }
        if((LEFT < m_width && DOWN < m_height) && char_at(LEFT, DOWN) == '@') {
            count++;
        }
        if(DOWN < m_height && char_at(col, DOWN) == '@') {
            count++;
        }
        if((RIGHT < m_width && DOWN < m_height) && char_at(RIGHT, DOWN) == '@') {
            count++;
        }
        return count;
    }
private:
    std::size_t m_width;
    std::size_t m_height;
    std::string m_values;
};

int main(int argc, char **argv) {
    if(argc == 2) {
        std::string filename{argv[1]};
        std::ifstream in_file{filename};
        Grid grid{in_file};
        std::size_t count{0};
        for(std::size_t y = 0; y < grid.height(); y++) {
            for(std::size_t x = 0; x < grid.width(); x++) {
                if(grid.char_at(x, y) == '@' && grid.count_neighbors(x, y) < 4) {
                    count++;
                }
            }
        }
        std::cout << "Number of accessible rolls: " << count << "\n";
    }
    return 0;
}