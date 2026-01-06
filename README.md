# SatSolverTemplate
SAT solver template for a course (TP) at INSA Toulouse.

## Setup
For this course, you need a modern C++ compiler that supports C++20. I recommend using `g++` or `clang`. `MSVC` should
also work, but I haven't tested it yet. Furthermore, you need `cmake` (at least version 3.14). And finally, I recommend
that you install one of the following libraries on your system: `libdw` (`libdw-dev`), `libdwarf` (`libdwarf-dev`).
These are not necessary but help producing meaningful stacktraces which makes debugging easier. I have no idea whether
these libraries are available for Windows...

All the remaining dependencies are handled by cmake.

## Configure the Project
### If you can, DON'T USE WINDOWS
C++ development on Windows is awful! Use a virtual machine or WSL, I don't care. If you have no other options, you need
to have a recent version of MSVC installed. If you're using VS-Code, make sure to launch it from the Windows developer
console [Developer Command Prompt](https://code.visualstudio.com/docs/cpp/config-msvc#_check-your-microsoft-visual-c-installation)!
**Otherwise debugging from within VSC DOES NOT WORK PROPERLY!!!**

### General Remarks
I recommend that you use a C++ IDE like CLion or an editor like VS-Code. Both have cmake integration making your life a
lot easier. If you like suffering and plan on developing using a basic editor, fret naught, it's not that complicated,
actually. The CMake project automatically configures all targets (the executables that you will run later) for you.
The following targets should be configured automatically:
* all_tests (runs all unit tests)
* test_basic_structures (runs only the tests for the basic structures like Variable and Literal)
* test_clause (runs only the tests for the Clause class)
* test_solver (runs only the tests for the principal member functions of the solver class)
* test_unit_propagation (runs only the tests for the unit propagation)

If you want to add other executables (e.g. a 'solve' executable that reads a problem and tries to solve it), then you
can add them in the main project folder. For example, you could create a `solve.cpp` file. In order to generate a build
target, you need to rerun CMake.

If you want to add a test, then you should add `test_<name of your test>.cpp` in the `Tests`-directory. Again you need
to rerun CMake for the target to be generated.

### CLion
Simply clone the project and open the folder as project with CLion. It should do everything for you. On first launch
CLion will probably ask you to create a CMake profile. Usually the proposed 'Debug' config is exactly what you want.
You can additionally create a 'Release' config by clicking on the + icon above the Debug config and it will most likely
create the appropriate release config automatically for you. When you're done just click ok. You can always add more
configs later on.

CLion should now automatically load the CMake project and configure all targets for you. You can select which target to
build on the top right of your screen. All the targets mentioned above should be available.

### VS-Code
You need to install the official Microsoft `CMake Tools` extension.
Clone the project and open the folder with VS-Code. If asked to trust the project, click on trust. VS-Code will probably
ask you to select a 'Kit' for the project. Here you select your C++-compiler. Once done, the CMake project should
automatically load.

Select the `CMake Tools` extension in the sidebar on the left. Depending on you VSC configuration, you should see three
tabs (usually on the left). In the 'Project Status' tab you should see six drop-down items: 
* Folder: We don't care
* Configure: Here you can select your compiler and the build configuration (Debug / Release)
* Build: We don't care
* Test: We don't care
* Debug: Here you can select a target and launch it with the debugger attached
* Launch: Here you can select a target and launch it in the console

Under the 'Project Outline' tab you should see all libraries and executables of the project. The initial targets listed
above should be visible here. You can alternatively build, launch and debug from here by right-clicking on an executable
and selecting the desired action. Or you simply use the integrated terminal and launch the executable manually (once
built, they should be in you build folder).

If you want to launch / debug an executable from VS-Code while passing command line arguments, you need to add them to
the `.vscode/launch.json`. Open the file and add the arguments as comma separated strings to the `args` variable.
There are three configurations in the `launch.json`:
* `Run current target (gdb)`: Use this config if you're on Linux and use `g++` as compiler
* `Run current target (lldb)`: Use this config if you're on MacOS or on Linux and use `clang` as compiler
* `Run current target (msvc)`: Use this config if you're on Windows

In any case, once you opened any code file, press F5 to run the current *Launch target*. This is the target that is
selected in the `CMake Tools` tab in the sidebar under `Project Status/Launch`. If F5 does not work, use the `Run and
Debug` tab in the sidebar and select the appropriate launch config.

### Manually using CMake
If you don't want to use one of the aforementioned methods, you can manually configure the project and work with any
editor of your choice. This is the most flexible option, and it's actually not that complicated. Just follow the
instructions:
1. Clone the repo and change into the folder
2. Create a build folder for each build config (Debug / Release, e.g. build_dbg / build_rel)
3. To configure the project run the following command (replace <...>):
   ```
   cmake -S . -B <path to you build directory> -DCMAKE_BUILD_TYPE=<release/debug>
   ```
   CMake should run without error. Towards the end of the output you should see a list with the targets mentioned above.
   You can manually specify the compiler using the flag `-DCMAKE_CXX_COMPILER=<the compiler of your choice>`
4. To build a target run the following command
   ```
   cmake --build <path to your build folder> -t <name of the target> -j <number of processors to use>
   ```
   If the build was successful, you can launch the executable directly from the terminal. For example, if you built
   the target `test_clause` then you execute the following command:
   ```
   <name of your build folder>/Tests/test_clause
   ```
5. Whenever you add new target (e.g. the solve executable) make sure to rerun CMake (just retype the command in 3.)

## Project Structure
The goal of this course is to develop a simple SAT-solver in C++. Don't worry if you're not very familiar with this
language. This template should help you focus on the logic of the solver instead of worrying about implementation
details. On the other hand, if you're a C++ pro then feel free to deviate from the template as you see fit.

You can find a doxygen generated documentation of all the structures
[here](https://timmifixedit.github.io/SatSolverTemplate/files.html).

### Implementation of the Solver
The solver will be developed as a C++-class in `Solver/Solver.hpp(cpp)`. As a reminder, in C and C++ we usually *declare*
functions and classes in the header file. A declaration involves only the function signatures and the class members.
The actual *implementation* will be done in the `.cpp` file. Exceptions are template classes and functions, but normally
you don't need to worry about that too much. To facilitate the implementation of the solver, we'll make use of the
following constructs:
* Variable and Literal (in `Solver/basic_structures.hpp(cpp)`)
* Clause (in `Solver/Clause.hpp(cpp)`)

Additionally, I already implemented some helping constructs in `Solver/util` as well as functions for reading and
writing SAT-formulas in `Solver/inout.hpp(cpp)`.

### Unit Tests using GTest
Under the `Tests` directory I implemented some unit tests using the [GTest framework](https://google.github.io/googletest/)
for the different structures that you will implement. They are by no means exhaustive but should give you some idea of
how to use the structures and what they should do. Feel free to add further testcases if you wish (the language is
pretty straightforward). The tests are grouped by implementation task (tests for the basic structures, for the clauses
and for the solver itself). Also, I included 4 test cases for the unit propagation algorithm. Each of those test groups
compiles to a separate testing executable. This means you can run them separately. Alternatively, you can run all tests
using the `all_tests` target. Initially, all tests will fail since the implementations of all methods are missing. If
you want to add a new test executable, make sure to respect the naming convention `test_<name of your test>.cpp`. Or you
can simply add test cases in the test files already present.

You're implementation should pass all unit tests with the exception of the tests in `test_clause.cpp` (if you plan on
implementing the unit propagation using counters, you don't need to implement a Clause-class). Especially the unit
propagation tests will be used to determine your grade for this TP.

## Exercises

### 1. Basic Structures
First we will focus on the implementation of the structures that we use for representing the problem: Variable and
Literal. A Variable is more or less just an ID identifying a variable in the SAT-formula. A Literal represents one of
two values that a variable can take. Since the variables are boolean, the only possible values for let's say a variable
`x` are `x` and `¬x`.

Follow the documentation in `Solver/basic_structures.hpp` to implement all the necessary functions. Remember that the
implementation goes in the file `Solver/basic_structures.cpp` (simply replace all the method bodies).

Test your implementation by building and running the test target `test_basic_structures`.

### 2. Clause
Next we need a structure to represent clauses. This can be a simple list that holds a variable number of literals.
I recommend using [`std::vector`](https://en.cppreference.com/w/cpp/container/vector) from the header `<vector>`.
This is a variable sized array that holds elements in a contiguous block of memory which makes it very efficient in
loops and also provides efficient random access. 

We are going to implement the unit propagation using watch literals. I recommend that you use the `Clause`-class already
declared in `Solver/Clause.hpp`. Again, you will need to implement all functions in the corresponding `.cpp` file. 
The class is designed to be immutable. You are not supposed to change a SAT-`Clause` once it has been declared 
(you cannot add or remove literals). Follow the documentation already provided and test your  code using the test target
`test_clause`.

### 3. Solver
No we get to the actual solver. Depending on the approach that you choose for the unit propagation you need to organize
your Clauses and Literals. In any case, you additionally need to keep track of the current variable assignment, i.e.
the *model*. This means that you need to save the truth value of every variable. A variable can either be `true`,
`false` or `unassigned`. Independently of your unit propagation algorithm you need to be able to efficiently look up
clauses by literals. Since you don't want to have multiple copies of the same clauses in your solver, you need to
organize them using pointers. I recommend that you use [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr)
for that purpose. I already made the type aliases `ClausePointer` and `ConstClausePointer` for you in
`Solver/Solver.hpp`. The advantage with [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) is that
you don't need to worry about memory management.

1. Start by declaring all your data structures within the solver class (organize your clauses, unit literals and model
   etc...). Then implement the constructor and methods like `val`, `satisfied`, `falsified`, ...
   You can run the test target `test_solver` even if you haven't yet finished implementing all methods.
2. Implement the **unit propagation** using the algorithm of you choice. This function is crucial for the SAT-solver,
   make sure it works as intended! Verify your implementation using the simple unit propagation test cases in
   `test_solver` and using the extra tests in `test_unit_propagation`.
3. Once your unit propagation works, you can finally implement the actual solve loop. Start with the DPLL algorithm.
   You will see that at some point you will choose the next variable to decide. You can use the `FirstVariable`
   heuristic from `Solver/heuristics.hpp`. This is probably the simplest heuristic there is (and it is not very good).
   You can also write your own heuristic (now or later). You don't need to use this class and instead implement your
   heuristic however you like (for example directly inside the solver). The advantage of an external heuristic class
   with a well-defined interface is that you can easily replace it later with something more powerful.
4. To test your solver, write a simple executable `solve.cpp` that you place in the main directory. You first need
   to load a problem using the functions from `Solver/input.hpp`. Then you configure your solver, run the search and
   finally print the result. When printing your solver output, please respect the following rules:
   * If the instance is UNSAT, print the word `UNSAT`.
   * If the instance is SAT, print the solution (all unit literals) in DIMACS format. Use the function
     `sat::inout::to_dimacs` from ´Solver/inout.hpp´ and [`std::cout`](https://en.cppreference.com/w/cpp/io/cout).
   * Make sure to respect these rules. You can print other stuff as you like as long as you prepend it with the
     character `c` to mark the line as comment.
5. Once your DPLL search works you can:
   * Implement a better value selection heuristic. If you used the `FirstVariable` heuristic in step 4 I recommend that
     you use this interface to create your own heuristic. Simply make a copy of `FirstVariable` in
     `Solver/heuristics.hpp` and don't forget the implementation in the `.cpp` file. You could for example try
     implementing a heuristic that randomly selects an unassigned variable (there is a random number generator in
     `Solver/util/random.hpp`).
   * Implement restarting.
   * Implement clause learning and ultimately CDCL search. This algorithm is much more powerful than DPLL and should be
     able to resolve even big problem instances.

Note: You will get a good grade if you manage to correctly implement DPLL, no need to do all the tasks in 5. But you
will get an even better grade if you put in some extra work :).

## Tips and Tricks
Here are some useful things that hopefully make your life easier.

### Printing Stuff
In C++ you need to define a custom `<<` operator in order to print your own types. I've already done that for you. If
you want to display variables or literals in the console, you can include the header `printing.hpp`. Example:
```c++
#include <iostream>
#include "Solver/basic_structures.hpp"
#include "Solver/printing.hpp"

int main() {
    using namespace sat;
    Variable x = 5;
    Literal notX = neg(x);
    std::cout << x << std::endl;
    std::cout << notX << std::endl;
    return 0;
}
```

The printing header also adds support for arbitrary ranges and tuples of printable types. For example, you can directly
print a `std::vector<Literal>` (or even your clause class as soon as you implemented the `begin` and `end` functions):
```c++
#include <iostream>
#include <vector>
#include <tuple>
#include "Solver/Clause.hpp"
#include "Solver/printing.hpp"

int main() {
    using namespace sat;
    Clause clause({pos(3), neg(6), neg(1)});
    std::cout << clause << std::endl;
    // You can even print a list of clauses:
    std::vector clauses{clause, clause, clause};
    std::cout << clauses << std::endl;
    // Or a tuple
    std::tuple tuple(clause, Variable(3), neg(4));
    std::cout << tuple << std::endl;
    return 0;
}
```

### Reading and Writing DIMACS
The header `inout.hpp` contains functions for converting literals and clauses to and from DIMACS. Example:
```c++
#include <iostream>
#include <fstream>
#include <string>
#include "Solver/Clause.hpp"
#include "Solver/inout.hpp"

int main() {
    using namespace sat;
    // write
    Clause c{pos(3), neg(4), neg(1)};
    std::string dimacs = inout::to_dimacs(c);
    std::cout << dimacs << std::endl;
    // read
    std::ifstream input("input.cnf");
    assert(input.is_open());
    auto [clauses, numVar] = inout::read_from_dimacs(input);
    return 0;
}
```

### Command Line Parser
When you implement exercise 3.4, you'll probably want to read the problem from a file that you pass via command line
arguments. Additionally, you might want to configure your solver via additional flags. I already implemented a simple
argument parser. It can be found in `Solver/util/cli.hpp`. The main method `parse` will return the path to the problem
instance (the first argument that you pass to your program) and accepts additional options. Let's say that you solver
additionally accepts an argument `verbose` that configures how much information is printed during the solving process.
Also, it optionally accepts a switch arg in order to select between DPLL and CDCL.
```c++
#inclue "Solver/util/cli.hpp"

int main(int argc, char **argv) {
    using namespace sat;
    bool useCDCL = false; // default value for argument: false
    int verbosity = 1; // default value for argument: 1
    const auto instanceFile = cli::parse(argc, argv,
        cli::ValueArg(/* name */ "--verbose", /* target variable */verbosity, /* required argument? */false),
        cli::SwitchArg("--use-cdcl", useCDCL));
    // ... rest of you program
}
```
When you invoke your program the following way: `./solver eval/sat/easy/instance.cnf --verbose 3 --use-cdcl`,
The variable `verbosity` will be 3 and the bool `useCDCL` will be true.

You can add as many argument specifications as you like. Simply add them as arguments to the function `parse`.

## Grading
You should hand in a working solver leveraging at least the DPLL algorithm. Create a simple executable as described in
step 4 above. Your executable needs to accept at least one command line argument (the path to the problem instance).
If the solver accepts more arguments, please document them and give them meaningful default values. The solver needs to
be callable with just the problem instance. When printing your solution, respect the rules also detailed in step 4.
If you don't, you will lose points!

Your solver will be run on a set of different problem instances of different difficulties (the ones under `eval`).
The more instance you can solve, the better. Your solver will be given a timeout of 2 minutes for each instance. When
running your solver, your code will be built in release mode (with all optimizations) to make it faster. If you want
to see how fast your solver can go, you too should build in release mode.

Your solver should at least be able to solve the medium instances (both SAT and UNSAT). If it can solve the harder once,
that's great. But above all, your solver should not crash. Make sure to write proper and safe code!
