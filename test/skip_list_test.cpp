    #include <gtest/gtest.h>
    #include "../include/skip_list.h"
    #include <string>

    TEST(SkipListTest, InsertAndDuplicate) {
        skip_list<int> sl;

        auto [it1, ok1] = sl.insert(5);
        EXPECT_TRUE(ok1);
        EXPECT_EQ(*it1, 5);

        auto [it2, ok2] = sl.insert(5);
        EXPECT_FALSE(ok2);
        EXPECT_EQ(*it2, 5);
    }

    TEST(SkipListTest, Find) {
        skip_list<int> sl;
        sl.insert(5);
        sl.insert(10);
        sl.insert(15);

        auto it5 = sl.find(5);
        ASSERT_NE(it5, sl.end());
        EXPECT_EQ(*it5, 5);

        auto it99 = sl.find(99);
        EXPECT_EQ(it99, sl.end());
    }

    TEST(SkipListTest, Erase) {
        skip_list<int> sl;
        sl.insert(5);
        sl.insert(10);

        EXPECT_TRUE(sl.erase(5));
        EXPECT_EQ(sl.size(), 1u);
        EXPECT_EQ(sl.find(5), sl.end());

        EXPECT_FALSE(sl.erase(5));
        EXPECT_EQ(sl.size(), 1u);
    }

    TEST(SkipListTest, Bounds) {
        skip_list<int> sl;
        sl.insert(10);
        sl.insert(20);
        sl.insert(15);

        auto lb = sl.lower_bound(14);
        ASSERT_NE(lb, sl.end());
        EXPECT_EQ(*lb, 15);

        auto ub = sl.upper_bound(15);
        ASSERT_NE(ub, sl.end());
        EXPECT_EQ(*ub, 20);
    }

    TEST(SkipListTest, CopyConstructor) {
        skip_list<int> sl;
        sl.insert(10);
        sl.insert(20);

        skip_list<int> sl2 = sl;
        EXPECT_EQ(sl, sl2);

        sl2.insert(30);
        EXPECT_NE(sl, sl2);
    }

    TEST(SkipListTest, MoveConstructor) {
        skip_list<int> sl;
        sl.insert(10);
        sl.insert(20);

        skip_list<int> sl2 = std::move(sl);
        EXPECT_FALSE(sl2.empty());
        EXPECT_TRUE(sl.empty());
    }

    TEST(SkipListTest, Clear) {
        skip_list<int> sl;
        sl.insert(10);
        sl.insert(20);
        sl.clear();

        EXPECT_TRUE(sl.empty());
        EXPECT_EQ(sl.size(), 0u);
    }

    TEST(SkipListTest, IteratorTraversal) {
        skip_list<int> sl;
        sl.insert(5);
        sl.insert(1);
        sl.insert(3);

        auto it = sl.begin();
        ASSERT_NE(it, sl.end());
        EXPECT_EQ(*it, 1); ++it;
        EXPECT_EQ(*it, 3); ++it;
        EXPECT_EQ(*it, 5); ++it;
        EXPECT_EQ(it, sl.end());
    }



    struct Person {
        std::string name;
        int age;

        bool operator<(const Person& other) const {
            return age < other.age;
        }

        bool operator==(const Person& other) const {
            return age == other.age && name == other.name;
        }
    };

    std::ostream& operator<<(std::ostream& os, const Person& p) {
        return os << p.name << " (" << p.age << ")";
    }

    TEST(SkipListTest, CustomTypeInsertAndFind) {
        skip_list<Person> people;
        people.insert({"Alice", 30});
        people.insert({"Bob", 25});
        people.insert({"Charlie", 35});

        auto it = people.lower_bound(Person{"", 25});
        ASSERT_NE(it, people.end());
        EXPECT_EQ(it->age, 25);
    }

    TEST(SkipListTest, CustomTypeIterator) {
        skip_list<Person> people;

        people.insert({"Alice", 30});
        people.insert({"Bob", 25});
        people.insert({"Charlie", 35});

        auto it = people.begin();
        ASSERT_NE(it, people.end());
        EXPECT_EQ(it->name, "Bob"); ++it;
        EXPECT_EQ(it->name, "Alice"); ++it;
        EXPECT_EQ(it->name, "Charlie"); ++it;
        EXPECT_EQ(it, people.end());
    }


    TEST(SkipListTest, EmptyList) {
        skip_list<int> sl;
        EXPECT_TRUE(sl.empty());
        EXPECT_EQ(sl.size(), 0u);
        EXPECT_EQ(sl.begin(), sl.end());
    }

    TEST(SkipListTest, UpperBoundEdgeCases) {
        skip_list<int> sl;
        sl.insert(10);
        sl.insert(20);
        sl.insert(30);

        EXPECT_EQ(*sl.upper_bound(10), 20);
        EXPECT_EQ(*sl.upper_bound(20), 30);
        EXPECT_EQ(sl.upper_bound(100), sl.end());
    }

    TEST(SkipListTest, LowerBoundEdgeCases) {
        skip_list<int> sl;
        sl.insert(10);
        sl.insert(20);
        sl.insert(30);

        EXPECT_EQ(*sl.lower_bound(15), 20);
        EXPECT_EQ(*sl.lower_bound(20), 20);
        EXPECT_EQ(sl.lower_bound(100), sl.end());
    }