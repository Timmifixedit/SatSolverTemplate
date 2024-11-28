/**
* @author Tim Luchterhand
* @date 27.11.24
* @brief
*/

#include "Solver.hpp"
#include "util/exception.hpp"

namespace sat {
    Solver::Solver(unsigned numVariables) {
        throw NOT_IMPLEMENTED;
    }

    bool Solver::addClause(Clause clause) {
        throw NOT_IMPLEMENTED;
    }

    /**
     * Here you have a possible implementation of the rebase-method. It should work out of the box.
     * To use it, remove the throw-expression and un-comment the code below. The implementation requires that
     * your solver has some sort of container of pointer types to clause called 'clauses'
     * (e.g. std::vector<ClausePointer>). Additionally, your solver needs to have a container of unit literals
     * called 'unitLiterals'.
     */
    auto Solver::rebase() const -> std::vector<Clause> {
        throw NOT_IMPLEMENTED;
        /*
        std::vector<Clause> reducedClauses;
        // We check all clauses in the solver. If the clause is SAT (at least one literal is satisfied), we don't include it.
        // Additionally, we remove all falsified literals from the clauses since we only care about unassigned literals.
        for (const auto &c: clauses) {
            bool sat = false;
            // We're creating a new (possibly smaller clause from the current clause c)
            std::vector<Literal> newLits;
            newLits.reserve(c->size());
            // For each literal l in the clause we check if l is satisfied or falsified
            for (auto l: *c) {
                if (satisfied(l)) {
                    // in this case, the whole clause is satisfied and can be removed entirely from the solver
                    sat = true;
                    break;
                }

                if (!falsified(l)) {
                    // only unassigned literals are added to the reduced clause
                    newLits.emplace_back(l);
                }
            }

            if (!sat) {
                // create the new clause (move all the literals inside the Clause-class)
                Clause newClause(std::move(newLits));
                // Check if we already added an equivalent clause
                auto res = std::ranges::find_if(reducedClauses, [&newClause](const auto &clause) {
                    return clause.sameLiterals(newClause);
                });
                if (res == reducedClauses.end()) {
                    // The new clause is not yet contained => add it
                    reducedClauses.emplace_back(std::move(newClause));
                }
            }
        }

        // Finally, we need to add all the unit literals as well
        for (Literal l: unitLiterals) {
            reducedClauses.emplace_back(std::vector{l});
        }

        return reducedClauses;*/
    }

    TruthValue Solver::val(Variable x) const {
        throw NOT_IMPLEMENTED;
    }

    bool Solver::satisfied(Literal l) const {
        throw NOT_IMPLEMENTED;
    }

    bool Solver::falsified(Literal l) const {
        throw NOT_IMPLEMENTED;
    }

    bool Solver::assign(Literal l) {
        throw NOT_IMPLEMENTED;
    }

    bool Solver::unitPropagate() {
        throw NOT_IMPLEMENTED;
    }
} // sat