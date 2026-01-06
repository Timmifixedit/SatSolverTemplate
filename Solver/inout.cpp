/**
* @author Tim Luchterhand
* @date 27.11.24
* @brief
*/

#include <cassert>
#include <stdexcept>

#include "inout.hpp"

namespace sat::detail {
    template<char Delim>
    struct Word : std::string {};

    template<char Delim>
    std::istream &operator>>(std::istream &in, Word<Delim> &word) {
        Word<Delim> part;
        while (std::getline(in, part, Delim) && part.empty()) {
        }
        word = part;
        return in;
    }

    template<char Delim>
    auto splitString(const std::string &string) -> std::vector<std::string> {
        std::istringstream iss(string);
        using iter = std::istream_iterator<Word<Delim>>;
        std::vector<std::string> ret(iter{iss}, iter{});
        return ret;
    }
}

namespace sat::inout {
    Literal from_dimacs(int val) noexcept {
        Variable tmp = std::abs(val) - 1;
        return Literal(val < 0 ? neg(tmp) : pos(tmp));
    }

    int to_dimacs(Literal l) noexcept {
        return l.sign() * static_cast<int>(var(l).get() + 1);
    }


    auto read_from_dimacs(std::istream &in) -> std::pair<std::vector<std::vector<Literal>>, std::size_t> {
        std::string line;
        std::vector<std::vector<Literal>> ret;
        std::size_t numVars = 0;
        std::size_t numClauses = 0;
        while (std::getline(in, line)) {
            if (line.starts_with("c")) {
                continue;
            }

            if (line.starts_with("p")) {
                auto parts = detail::splitString<' '>(line);
                if (parts.size() != 4) {
                    throw std::runtime_error("invalid format");
                }

                numVars = std::stol(parts.rbegin()[1]);
                numClauses = std::stoi(parts.back());
                ret.reserve(numClauses);
                break;
            }
        }

        std::size_t lineNo = 0;
        while (lineNo < numClauses) {
            if (!std::getline(in, line)) {
                throw std::runtime_error("not enough lines in given file");
            }

            if (line.starts_with("c")) {
                continue;
            }

            ++lineNo;
            int val;
            std::stringstream ss(line);
            std::vector<Literal> lits;
            while (ss >> val) {
                if (val == 0) {
                    break;
                }

                lits.emplace_back(from_dimacs(val));
            }
            ret.emplace_back(std::move(lits));
        }

        return {std::move(ret), numVars};
    }
}
