/**
* @author Tim Luchterhand
* @date 27.11.24
* @file inout.hpp
* @brief Contains functions for reading and writing dimacs format.
*/

#ifndef INOUT_HPP
#define INOUT_HPP

#include <ostream>
#include <istream>
#include <vector>
#include <iterator>
#include <sstream>

#include "basic_structures.hpp"
#include "Clause.hpp"
#include "util/concepts.hpp"


/**
 * @brief Namespace containing dimacs io utilities
 */
namespace sat::inout {
    /**
     * Converts an integer to a sat::Literal
     * @param val literal value. The value +5 for example corresponds to the positive literal of the variable with
     * ID 5, whereas -5 corresponds to the negative literal
     * @return converted Literal
     */
    Literal from_dimacs(int val) noexcept;

    /**
     * Converts a sat::Literal to a dimacs literal
     * @param l Literal to convert
     * @return 1 based integer representing the literal in dimacs format
     */
    int to_dimacs(Literal l) noexcept;

    /**
     * Reads a SAT problem from a stream
     * @param in input stream to read from
     * @return std::pair containing (all clauses of the problem, the number of variables in the problem)
     */
    auto read_from_dimacs(std::istream &in) -> std::pair<std::vector<std::vector<Literal>>, std::size_t>;

    /**
     * Converts a range of clauses to dimacs format
     * @tparam R clause range type
     * @param clauses A range of clauses
     * @return dimacs string
     */
    template<std::ranges::range R>
    std::string to_dimacs(const R &clauses) {
        static_assert(clause_like<std::ranges::range_value_t<R>>,
                      "The range you passed to this function does not hold elements that are clause-like");
        Literal maxLit = 0;
        std::size_t nClauses = 0;
        for (const auto &c: clauses) {
            ++nClauses;
            for (Literal l: c) {
                maxLit = std::max(maxLit.get(), l.get());
            }
        }

        const std::size_t numVars = var(maxLit).get() + 1;
        std::vector<std::string> lines;
        lines.reserve(nClauses);
        for (const auto &clause: clauses) {
            std::stringstream ss;
            for (auto l: clause) {
                ss << to_dimacs(l) << " ";
            }

            ss << "0";
            lines.emplace_back(ss.str());
        }

        std::stringstream ss;
        ss << "p cnf " << numVars << " " << nClauses << std::endl;
        for (const auto &l: lines) {
            ss << l << std::endl;
        }

        return ss.str();
    }

    /**
     * Converts a range of literals to dimacs format
     * @tparam L Literal range type
     * @param literals the literals to convert
     * @return dimacs string
     */
    template<concepts::typed_range<Literal> L>
    std::string to_dimacs(const L &literals) {
        return to_dimacs(literals | std::views::transform([](auto literal) { return std::vector{literal}; }));
    }
}

#endif //INOUT_HPP
