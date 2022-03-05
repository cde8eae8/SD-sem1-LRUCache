#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "lrucache.h"

namespace std {
template <typename T>
std::ostream& operator<<(std::ostream& out, std::optional<T> const& o) {
	if (o.has_value()) {
		return out << "optional(" << o.value() << ")";
	} else {
		return out << "optional::empty()";
	}
}
}

TEST_CASE("empty") {
	LRUCache<int, int> cache{1};
	CHECK_FALSE(cache.get(0).has_value());
	CHECK_FALSE(cache.get(1).has_value());
}

TEST_CASE("replace value") {
	LRUCache<int, int> cache{1};
	cache.put(1, 1);
	CHECK(std::optional<int>{1} == cache.get(1));
	cache.put(1, 2);
	CHECK(std::optional<int>{2} == cache.get(1));
}

TEST_CASE("remove last used") {
	LRUCache<int, int> cache{1};
	cache.put(1, 1);
	CHECK(cache.get(1) == std::optional<int>{1});
	cache.put(2, 2);
	CHECK(cache.get(1) == std::nullopt);
	CHECK(cache.get(2) == std::optional<int>{2});
}

TEST_CASE("remove last used reordered") {
	LRUCache<int, int> cache{2};
	cache.put(1, 1);
	cache.put(2, 2);
	cache.get(1);
	cache.put(1, 2);
	CHECK(cache.get(1) == std::optional<int>{2});
	CHECK(cache.get(2) == std::optional<int>{2});

	cache.get(1);
	cache.put(3, 3);
	CHECK(cache.get(3) == std::optional<int>{3});
	CHECK(cache.get(2) == std::nullopt);
	CHECK(cache.get(1) == std::optional<int>{2});
}

TEST_CASE("multiple values") {
	size_t size{10};
	size_t step2{3};

	LRUCache<int, int> cache(size);
	for (size_t i = 0; i < size; ++i) {
		cache.put(i, i);
	}

	for (size_t i = 0; i < size; ++i) {
		CHECK(cache.get(i) == std::optional<int>{i});
	}

	for (int i = size - 1; i >= 0; --i) {
		CHECK(cache.get(i) == std::optional<int>{i});
	}

	for (size_t i = 0; i < step2; ++i) {
		cache.put(size + i, size + i);
	}

	for (size_t i = 0; i < step2; ++i) {
		CHECK(cache.get(size + i) == std::optional<int>{size + i});
	}

	for (size_t i = 0; i < size - step2; ++i) {
		CHECK(cache.get(i) == std::optional<int>{i});
	}
}
