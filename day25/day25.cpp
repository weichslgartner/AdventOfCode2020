#include "common.h"
#include <fmt/core.h>

constexpr auto divider_init { 20201227 };
constexpr auto value_init { 1 };

template<Integral T>
constexpr auto perform_scheme(T value, T subject_number, T dvidier) {
	value *= subject_number;
	value = value % dvidier;
	return value;
}

template<Integral T>
constexpr T calc_loop_size(T pub_key, T subject_number = 7) {
	auto value { static_cast<T>(value_init) };
	auto dvidier { static_cast<T>(divider_init) };
	T i = static_cast<T>(0);
	while (value != pub_key) {
		value = perform_scheme(value, subject_number, dvidier);
		i++;
	}
	return i;
}

template<Integral T>
constexpr T encrypt(T subject_number, T loop_size) {
	auto value { static_cast<T>(value_init) };
	auto dvidier { static_cast<T>(divider_init) };
	for (T i { 0 }; i < loop_size; ++i) {
		value = perform_scheme(value, subject_number, dvidier);
	}
	return value;
}

int main() {
	constexpr auto pubkey_card { 10705932 };
	constexpr auto pubkey_door { 12301431 };
	auto loop_size_card = calc_loop_size(pubkey_card);
	fmt::print("Part 1: {}\n", encrypt(pubkey_door, loop_size_card));
	return EXIT_SUCCESS;
}

