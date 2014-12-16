#include <iostream>
#include <algorithm>
#include <sstream>
#include <limits>

struct tournament_tree {
	typedef std::pair<double, size_t> item_type;

	tournament_tree(size_t k) {
		keys.resize(k,
			std::make_pair(0, 0));
		treeStorage.resize(k - 1, 0);
		tree = &treeStorage[0] - 1;
		for (size_t i = 0; i < k; ++i) {
			tree[(i + k) >> 1] = i;
		}
		for (size_t i = k / 2; --i;) {
			tree[i] = tree[2 * i];
		}
	}

	void change_key(size_t index, item_type v) {
		keys[index] = v;
		size_t node = (index + keys.size()) >> 1;
		// std::cout << node << std::endl;
		tree[node] =
			(keys[index] < keys[index ^ 1]) ? index : index ^ 1;
		while (node > 1) {
			size_t opponent = node ^ 1;
			size_t parent = node >> 1;
			tree[parent] =
				(keys[tree[node]] < keys[tree[opponent]])
				? tree[node] : tree[opponent];
			node = parent;
			// std::cout << node << std::endl;
		}
	}

	size_t top_index() {
		return tree[1];
	}

	item_type top() {
		return keys[tree[1]];
	}

	void replace_top(item_type v) {
		// size_t i = top_index();
		// std::cout << "Top is " << i;
		// std::cout << " with value " <<
		// 	keys[i].first << std::endl;
		change_key(top_index(), v);
	}

	item_type * begin() {
		return &keys[0];
	}

	item_type * end() {
		return &keys[0] + keys.size();
	}

	std::vector<item_type> keys;
	std::vector<size_t> treeStorage;
	size_t * tree;
};

int main() {
	const size_t k = 64;
	std::vector<std::pair<std::string, double> > lines;
	std::string line;
	tournament_tree t(k);
	double topKey = 0.0;
	while (std::getline(std::cin, line)) {
		std::stringstream ss(line);
		double key;
		char c;
		ss >> key >> c;
		switch (c) {
			case 'G': key *= 1024; // fallthru
			case 'M': key *= 1024; // fallthru
			case 'K': key *= 1024;
		}
		lines.emplace_back(std::move(line), key);
		if (key <= t.top().first) continue;
		t.replace_top(std::make_pair(key, lines.size() - 1));
		if (key > topKey) {
			topKey = key;
			std::vector<size_t> top(k);
			for (size_t i = 0; i < k; ++i) {
				top[i] = t.begin()[i].second;
			}
			std::sort(top.begin(), top.end(),
				[&] (size_t a, size_t b) {
					return lines[a].second < lines[b].second;
				});
			std::cout << std::string(79, '-') << std::endl;
			for (size_t x : top) {
				std::cout << lines[x].first << '\n';
			}
		}
	}
	std::sort(lines.begin(), lines.end(),
		[] (const std::pair<std::string, double> & a,
			const std::pair<std::string, double> & b) {
			return a.second < b.second;
		});
	size_t threshold = 1000;
	for (size_t i = std::max(lines.size(), threshold) - threshold; i < lines.size(); ++i) {
		std::cout << lines[i].first << '\n';
	}
	std::cout << std::flush;
	return 0;
}
