/**
* @author Tim Luchterhand
* @date 06.01.26
* @file printing.cpp
* @brief
*/

#include "printing.hpp"

namespace sat {
    std::ostream &operator<<(std::ostream &os, Variable x) {
        os << "Var " << x.get();
        return os;
    }

    std::ostream &operator<<(std::ostream &os, Literal l) {
        auto sign = l.sign() < 0 ? "¬" : "";
        os << "Lit " << sign << var(l).get() << " (" << l.get() << ")";
        return os;
    }
}
