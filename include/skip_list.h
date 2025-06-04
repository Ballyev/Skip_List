#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <random>

template <typename T>
class skip_list {
private:
    struct Node {
        T value;
        std::vector<Node*> forward;
        explicit Node(const T& val, size_t level)
            : value(val), forward(level, nullptr) {}
    };

    size_t max_level;
    size_t current_level;
    size_t count;
    Node* header;
    std::default_random_engine rng;
    std::bernoulli_distribution coin_flip;

    size_t random_level() {
        size_t level = 1;
        while (level < max_level && coin_flip(rng)) {
            ++level;
        }
        return level;
    }

    Node* get_node_by_value(const T& value, size_t level, const skip_list& list) const {
        Node* current = list.header;
        for (int i = list.current_level - 1; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->value < value) {
                current = current->forward[i];
            }
        }
        return current->forward[level];
    }

public:
    class iterator;
    class const_iterator;
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit iterator(Node* node) : current(node) {}

        reference operator*() const { return current->value; }
        pointer operator->() const { return &current->value; }

        iterator& operator++() {
            if (current) current = current->forward[0];
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return !(*this == other); }

    private:
        Node* current;
        friend class skip_list;
    };

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        explicit const_iterator(const Node* node) : current(const_cast<Node*>(node)) {}

        reference operator*() const { return current->value; }
        pointer operator->() const { return &current->value; }

        const_iterator& operator++() {
            if (current) current = current->forward[0];
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const const_iterator& other) const { return current == other.current; }
        bool operator!=(const const_iterator& other) const { return !(*this == other); }

    private:
        Node* current;
        friend class skip_list;
    };

    explicit skip_list(size_t max_level = 16)
        : max_level(max_level),
          current_level(0),
          count(0),
          header(new Node(T(), max_level)),
          rng(),
          coin_flip(0.5)
    {}

    ~skip_list() {
        if (!header) return;

        Node* current = header->forward[0];
        while (current) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
        delete header;
    }

    iterator begin() { return iterator(header->forward[0]); }
    iterator end() { return iterator(nullptr); }

    const_iterator begin() const { return const_iterator(header->forward[0]); }
    const_iterator end() const { return const_iterator(nullptr); }

    const_iterator cbegin() const { return const_iterator(header->forward[0]); }
    const_iterator cend() const { return const_iterator(nullptr); }

    iterator find(const T& value) {
        Node* current = header;
        for (int i = current_level - 1; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->value < value) {
                current = current->forward[i];
            }
        }
        current = current->forward[0];
        if (current && current->value == value) {
            return iterator(current);
        } else {
            return end();
        }
    }

    std::pair<iterator, bool> insert(const T& value) {
        std::vector<Node*> update(max_level, nullptr);
        Node* current = header;
        for (int i = current_level - 1; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        current = current->forward[0];
        if (current && current->value == value) {
            return {iterator(current), false};
        }

        size_t new_level = random_level();
        if (new_level > current_level) {
            for (size_t i = current_level; i < new_level; ++i) {
                update[i] = header;
            }
            current_level = new_level;
        }

        Node* new_node = new Node(value, new_level);
        for (size_t i = 0; i < new_level; ++i) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }

        ++count;
        return {iterator(new_node), true};
    }

    bool erase(const T& value) {
        std::vector<Node*> update(max_level, nullptr);
        Node* current = header;
        for (int i = current_level - 1; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];
        if (current && current->value == value) {
            for (size_t i = 0; i < current_level; ++i) {
                if (update[i]->forward[i] != current) break;
                update[i]->forward[i] = current->forward[i];
            }
            delete current;
            while (current_level > 1 && header->forward[current_level - 1] == nullptr) {
                --current_level;
            }
            --count;
            return true;
        }
        return false;
    }

    iterator lower_bound(const T& value) {
        Node* current = header;
        for (int i = current_level - 1; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->value < value) {
                current = current->forward[i];
            }
        }
        current = current->forward[0];
        return iterator(current);
    }

    iterator upper_bound(const T& value) {
        Node* current = header;
        for (int i = current_level - 1; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->value <= value) {
                current = current->forward[i];
            }
        }
        current = current->forward[0];
        return iterator(current);
    }

    bool empty() const { return count == 0; }
    size_t size() const { return count; }

    void clear() {
        if (!header) return;

        Node* current = header->forward[0];
        while (current) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }

        for (size_t i = 0; i < max_level; ++i) {
            header->forward[i] = nullptr;
        }

        current_level = 0;
        count = 0;
    }

    skip_list(const skip_list& other)
        : max_level(other.max_level),
          current_level(other.current_level),
          count(other.count),
          header(new Node(T(), max_level)),
          rng(other.rng),
          coin_flip(other.coin_flip) {
        Node* current_other = other.header->forward[0];
        Node* prev = header;

        while (current_other) {
            Node* new_node = new Node(current_other->value, current_other->forward.size());

            for (size_t i = 0; i < new_node->forward.size(); ++i) {
                if (current_other->forward[i]) {
                    T val = current_other->forward[i]->value;
                    Node* target = header;
                    for (int j = current_level - 1; j >= 0; --j) {
                        while (target->forward[j] && target->forward[j]->value < val) {
                            target = target->forward[j];
                        }
                    }
                    new_node->forward[i] = target->forward[i];
                } else {
                    new_node->forward[i] = nullptr;
                }
            }

            prev->forward[0] = new_node;
            prev = new_node;
            current_other = current_other->forward[0];
        }
    }

    skip_list(skip_list&& other) noexcept
        : max_level(other.max_level),
          current_level(other.current_level),
          count(other.count),
          header(other.header),
          rng(std::move(other.rng)),
          coin_flip(other.coin_flip) {
        other.header = nullptr;
        other.count = 0;
        other.current_level = 0;
    }


    skip_list& operator=(const skip_list& other) {
        if (this != &other) {
            clear();
            delete header;

            max_level = other.max_level;
            current_level = other.current_level;
            count = other.count;
            header = new Node(T(), max_level);
            rng = other.rng;
            coin_flip = other.coin_flip;

            Node* current_other = other.header->forward[0];
            Node* prev = header;
            while (current_other) {
                Node* new_node = new Node(current_other->value, current_other->forward.size());
                for (size_t i = 0; i < new_node->forward.size(); ++i) {
                    new_node->forward[i] = get_node_by_value(new_node->value, i, other);
                }
                prev->forward[0] = new_node;
                prev = new_node;
                current_other = current_other->forward[0];
            }
        }
        return *this;
    }


    skip_list& operator=(skip_list&& other) noexcept {
        if (this != &other) {
            clear();
            delete header;

            max_level = other.max_level;
            current_level = other.current_level;
            count = other.count;
            header = other.header;
            rng = std::move(other.rng);
            coin_flip = other.coin_flip;

            other.header = nullptr;
            other.count = 0;
            other.current_level = 0;
        }
        return *this;
    }
};


template <typename T>
bool operator==(const skip_list<T>& a, const skip_list<T>& b) {
    if (a.size() != b.size()) return false;
    auto it_a = a.begin();
    auto it_b = b.begin();
    while (it_a != a.end()) {
        if (*it_a != *it_b) return false;
        ++it_a;
        ++it_b;
    }
    return it_b == b.end();
}

template <typename T>
bool operator!=(const skip_list<T>& a, const skip_list<T>& b) {
    return !(a == b);
}