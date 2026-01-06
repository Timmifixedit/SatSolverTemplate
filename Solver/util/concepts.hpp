/**
* @author Tim Luchterhand
* @date 26.11.24
* @file concepts.hpp
* @brief Contains various type traits and concepts.
* @note If you are familiar with the c++ type system, you might find these things useful. If not, don't be scared,
* you don't need to understand them
*/

#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

#include <ostream>
#include <concepts>
#include <ranges>


/**
 * @brief namespace containing various type traits. Prefer using the concepts.
 */
namespace sat::traits {

    template<template<typename...> typename Template, typename T>
    struct is_same_template : std::false_type {};

    template<template<typename...> typename Template, typename... Args>
    struct is_same_template<Template, Template<Args...>> : std::true_type {};

    template<template<typename ...> typename Template, typename T>
    constexpr inline bool is_same_template_v = is_same_template<Template, std::remove_cvref_t<T>>::value;

    template<typename T>
    struct is_scalar {
        static constexpr bool value = std::is_integral_v<T> or std::is_floating_point_v<T>;
    };

    template<typename T>
    inline constexpr bool is_scalar_v = is_scalar<T>::value;

    template<typename ...>
    struct always_false : std::false_type {};

    template<typename ...Ts>
    inline constexpr bool always_false_v = always_false<Ts...>::value;

}

/**
 * @brief namespace containing various type concepts
 */
namespace sat::concepts {
    /**
     * Concept for a callable object that is invocable with various arguments and returns a certain return type
     */
    template<typename Functor, typename Return, typename ...Args>
    concept callable_r = std::invocable<Functor, Args...> &&
                         std::convertible_to<std::invoke_result_t<Functor, Args...>, Return>;

    /**
     * Type that is either some integer or floating point type
     */
    template<typename T>
    concept scalar = traits::is_scalar_v<T>;

    /**
     * C++20 range of a given type
     */
    template<typename R, typename T>
    concept typed_range = std::ranges::range<R> && std::same_as<std::ranges::range_value_t<R>, T>;

    /**
     * C++20 range whose values can be converted to a certain type
     */
    template<typename R, typename T>
    concept ctyped_range = std::ranges::range<R>and std::convertible_to<std::ranges::range_value_t<R>, T>;

    /**
     * C++20 range that contains some incomplete template type
     */
    template<typename R, template<typename> typename T>
    concept ttyped_range = std::ranges::range<R> && traits::is_same_template_v<T, std::ranges::range_value_t<R>>;

    /**
     * Type that is of a given incomplete template type
     */
    template<typename T, template<typename...> typename Template>
    concept same_template = traits::is_same_template_v<Template, T>;

    /**
     * Type that is an l-value reference to a given incomplete template type
     */
    template<typename T, template<typename...> typename Template>
    concept same_template_lvref = same_template<T, Template> and std::is_lvalue_reference_v<T>;

    /**
     * Type that is a const l-value reference to a given incomplete template type
     */
    template<typename T, template<typename...> typename Template>
    concept same_template_clvref = same_template_lvref<T, Template> and std::is_const_v<std::remove_reference_t<T>>;

    /**
     * Type that is an enum
     */
    template<typename T>
    concept enum_type = std::is_enum_v<T>;
}

#endif //CONCEPTS_HPP
