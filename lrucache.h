#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <unordered_map>
#include <optional>
#include <list>

template <typename K, typename V>
class LRUCache {
	class Node {
	public:
		Node(V v, typename std::list<K>::iterator it) {
			value = v;
			iterator = it;
		}

		V value;
		typename std::list<K>::iterator iterator;
	};

public:
	LRUCache(int maxSize) {
		this->maxSize = maxSize;
	}

	std::optional<V> get(K key) {
		if (values.count(key) > 0) {
			Node& node = values.at(key);
			ordered.erase(node.iterator);
			ordered.insert(ordered.begin(), key);
			node.iterator = ordered.begin();

			assert(values.count(key) != 0);
			assert(values.at(key).iterator == ordered.begin());
			assert(*ordered.begin() == key);
			return {node.value};
		} else {
			assert(values.count(key) == 0);
			return std::nullopt;
		}
	}

	void put(K key, V value) {
		if (values.count(key) > 0) {
			Node& node = values.at(key);
			ordered.erase(node.iterator);
			ordered.insert(ordered.begin(), key);
			node.value = value;
			node.iterator = ordered.begin();
		} else {
			if (ordered.size() == maxSize) {
				values.erase(ordered.back());
				ordered.erase(--ordered.end());
			}
			ordered.insert(ordered.begin(), key);
			values.insert({key, Node{value, ordered.begin()}});
		}
		assert(values.count(key) != 0);
		assert(values.at(key).iterator == ordered.begin());
		assert(*ordered.begin() == key);
	}

private:
	std::unordered_map<K, Node> values;
	std::list<K> ordered;
	int maxSize;
};

#endif // LRUCACHE_H
