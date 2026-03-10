//
// Created by justinw on 08/03/2026.
//

#include <stdexcept>
#include "variant_utils.h"
#include <variant>
template<typename ...NoMono, typename ...WithMono>
std::variant<NoMono...> extract_value(std::variant<WithMono...> &variant) {
    return std::visit([]<typename T0>(T0&& arg) -> std::variant<WithMono... > {
        using T = std::decay_t<T0>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            throw std::runtime_error("Variant contains monostate; cannot extract value.");
        } else {
            return std::variant<NoMono...>{std::forward<T0>(arg)};
        }
    }, variant);
}