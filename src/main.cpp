#include <iostream>
#include <cassert>
#include "../include/skip_list.h"


struct Person {
    std::string name;
    int age;

    bool operator<(const Person& other) const {
        return age < other.age; // Сравнение по возрасту
    }

    bool operator==(const Person& other) const {
        return age == other.age && name == other.name;
    }
};


std::ostream& operator<<(std::ostream& os, const Person& p) {
    return os << p.name << " (" << p.age << ")";
}


void test_skip_list() {
    std::cout << "=== Running tests ===\n";

    skip_list<int> sl;


    auto [it1, ok1] = sl.insert(5);
    assert(ok1 && *it1 == 5);
    std::cout << "Test 1 passed: Insert 5\n";


    auto [it2, ok2] = sl.insert(5);
    assert(!ok2 && it2 == it1);
    std::cout << "Test 2 passed: Duplicate insert ignored\n";


    assert(*sl.find(5) == 5);
    assert(sl.find(99) == sl.end());
    std::cout << "Test 3 passed: Find works\n";


    assert(sl.erase(5));
    assert(sl.empty() && sl.size() == 0);
    assert(!sl.erase(5));
    std::cout << "Test 4 passed: Erase works\n";


    sl.insert(10);
    sl.insert(20);
    sl.insert(15);

    assert(*sl.lower_bound(14) == 15);
    assert(*sl.upper_bound(15) == 20);
    std::cout << "Test 5 passed: Bounds work\n";


    skip_list<int> sl2 = sl;
    assert(sl == sl2);
    sl2.insert(99);
    assert(sl != sl2);
    std::cout << "Test 6 passed: Copy and comparison work\n";


    skip_list<int> sl3 = std::move(sl2);
    assert(!sl3.empty());
    assert(sl2.empty());
    std::cout << "Test 7 passed: Move semantics work\n";


    sl3.clear();
    assert(sl3.empty());
    std::cout << "Test 8 passed: Clear works\n";

    std::cout << "All tests passed successfully!\n";
}


void demo_with_custom_type() {
    std::cout << "\n=== Demo with custom type (Person) ===\n";

    skip_list<Person> people;
    people.insert({"Alice", 30});
    people.insert({"Bob", 25});
    people.insert({"Charlie", 35});
    people.insert({"David", 25}); // дубликат по возрасту

    std::cout << "People in order of age:\n";
    for (const auto& p : people) {
        std::cout << p << "\n";
    }

    auto res = people.find({"Unknown", 25});
    if (res != people.end()) {
        std::cout << "Found person with age 25: " << *res << "\n";
    } else {
        std::cout << "No person found with age 25.\n";

    }
}
int main() {
    test_skip_list();
    demo_with_custom_type();

    return 0;
}