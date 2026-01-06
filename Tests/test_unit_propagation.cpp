/**
* @author Tim Luchterhand
* @date 28.11.24
* @brief
*/
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <fstream>
#include <algorithm>

#include "Solver.hpp"
#include "inout.hpp"
#include "printing.hpp"
#include "testing_utils.hpp"

sat::Solver loadSolver(const std::string &cnfFile) {
    std::ifstream ifs(cnfFile);
    if (not ifs.is_open()) {
        std::cerr << "Could not open file " << cnfFile  <<". This should never happen" << std::endl;
        std::exit(1);
    }

    auto [clauses, numVariables] = sat::inout::read_from_dimacs(ifs);
    sat::Solver solver(numVariables);
    for (auto &clause : clauses) {
        solver.addClause(sat::Clause(std::move(clause)));
    }

    return solver;
}

auto loadSolution(const std::string &cnfFile) {
    std::ifstream ifs(cnfFile);
    if (not ifs.is_open()) {
        std::cerr << "Could not open file " << cnfFile  <<". This should never happen" << std::endl;
        std::exit(1);
    }

    auto [clauses, _] = sat::inout::read_from_dimacs(ifs);
    return clauses;
}

void testEquality(const std::vector<std::vector<sat::Literal>> &groundTruth, const std::vector<sat::Clause> &result) {
    ASSERT_EQ(groundTruth.size(), result.size()) << "The number of clauses in the solution is not the same";
    for (auto &clause : groundTruth) {
        if(not test::findClause(clause, result)) {
            std::cout << "The following clause in the solution was not found in the output of the solver" << std::endl;
            std::cout << "Expected: " << clause << std::endl;
            std::cout << "Result contains following clauses:" << std::endl;
            for (const auto &rClause : result) {
                std::cout << rClause << std::endl;
            }

            FAIL();
        }
    }
}

TEST(unit_propagation, problem1) {
    using namespace sat;
    auto solver = loadSolver(test::TestData::UnitPropagationProblem1);
    ASSERT_TRUE(solver.unitPropagate()) << "Unit propagation failed";
    auto gt = loadSolution(test::TestData::UnitPropagationSolution1);
    testEquality(gt, solver.rebase());
}

TEST(unit_propagation, problem2) {
    using namespace sat;
    auto solver = loadSolver(test::TestData::UnitPropagationProblem2);
    ASSERT_FALSE(solver.unitPropagate()) << "Succeeded but it shouldn't have";
}

TEST(unit_propagation, problem3) {
    using namespace sat;
    auto solver = loadSolver(test::TestData::UnitPropagationProblem3);
    ASSERT_TRUE(solver.unitPropagate()) << "Unit propagation failed";
    auto gt = loadSolution(test::TestData::UnitPropagationSolution3);
    testEquality(gt, solver.rebase());
}

TEST(unit_propagation, problem4) {
    using namespace sat;
    auto solver = loadSolver(test::TestData::UnitPropagationProblem4);
    ASSERT_TRUE(solver.unitPropagate()) << "Unit propagation failed";
    auto gt = loadSolution(test::TestData::UnitPropagationSolution4);
    testEquality(gt, solver.rebase());
}

#ifndef __RUN_ALL_TESTS__

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif
