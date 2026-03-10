//
// Created by justinw on 08/03/2026.
//

#ifndef PYC___VARIANT_UTILS_H
#define PYC___VARIANT_UTILS_H
#include <variant>

#endif //PYC___VARIANT_UTILS_H

template<typename ...NoMono, typename ...WithMono>
std::variant<NoMono...> extract_value(std::variant<WithMono...> &variant);