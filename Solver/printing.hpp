/**
* @author Tim Luchterhand
* @date 06.01.26
* @file printing.hpp
* @brief Contains functions ostream operators for various structures: ranges, tuples and the structures
* like variables and literals for easy printing.
*/

#ifndef PRINTING_HPP
#define PRINTING_HPP

#include <ostream>
#include <tuple>
#include <ranges>
#include <concepts>
#include <string>
#include <string_view>

#include "basic_structures.hpp"

template<typename T>
concept printable = requires(const T &obj, std::ostream &os) {
    os << obj;
};


namespace printing_detail {
    template<typename T>
    concept string = std::same_as<std::string, std::remove_cvref_t<T>> or
                     std::same_as<std::string_view, std::remove_cvref_t<T>> or
                     std::same_as<std::decay_t<T>, char *> or
                     std::same_as<std::decay_t<T>, const char *>;

    template<std::size_t Idx, typename Tuple>
    void printElem(std::ostream &os, const Tuple &tuple) {
        if constexpr (Idx != 0) {
            os << ", ";
        }

        os << std::get<Idx>(tuple);
    }
}

namespace std {
    template<printable ...Ts>
    std::ostream &operator<<(std::ostream &os, const std::tuple<Ts...> &tuple) {
        os << "(";
        [&]<std::size_t ... Idx>(std::index_sequence<Idx...>) {
            (printing_detail::printElem<Idx>(os, tuple), ...);
        }(std::make_index_sequence<sizeof...(Ts)>());
        os << ")";
        return os;
    }

    template<printable T, printable U>
    std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &pair) {
        os << "(" << pair.first << ", " << pair.second << ")";
        return os;
    }

    template<std::ranges::range R>
        requires(printable<std::ranges::range_value_t<R>> and
                 not printing_detail::string<R>)
    std::ostream &operator<<(std::ostream &os, R &&range) {
        os << "[";
        bool first = true;
        for (const auto &elem: std::forward<R>(range)) {
            if (first) {
                first = false;
            } else {
                os << ", ";
            }
            os << elem;
        }

        os << "]";
        return os;
    }
}

namespace sat {
    /**
     * Ostream operator allowing for easy printing of variables
     */
    std::ostream &operator<<(std::ostream &os, Variable x);

    /**
     * Ostream operator allowing for easy printing of literals
     */
    std::ostream &operator<<(std::ostream &os, Literal l);
}

#endif
