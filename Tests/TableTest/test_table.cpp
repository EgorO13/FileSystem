#include <catch2/catch_test_macros.hpp>
#include "Table/table.h"
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <stdexcept>


TEST_CASE("Constructors and assignment operators") {
    SECTION("Default constructor") {
        Table<int, std::string> table;
        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);
        REQUIRE(table.capacity() == 0);
    }

    SECTION("Initializer list constructor") {
        Table<int, std::string> table = {{1, "one"}, {2, "two"}, {3, "three"}};
        REQUIRE(table.size() == 3);
        REQUIRE(table.contains(1));
        REQUIRE(table.contains(2));
        REQUIRE(table.contains(3));
    }

    SECTION("Copy constructor") {
        Table<int, std::string> original = {{1, "a"}, {2, "b"}};
        Table<int, std::string> copy(original);
        REQUIRE(original.size() == 2);
        REQUIRE(copy.size() == 2);
        REQUIRE(copy[1] == "a");
        REQUIRE(copy[2] == "b");
        original[1] = "modified";
        REQUIRE(copy[1] == "a");
    }

    SECTION("Move constructor") {
        Table<int, std::string> original = {{1, "x"}, {2, "y"}};
        Table<int, std::string> moved(std::move(original));
        REQUIRE(moved.size() == 2);
        REQUIRE(moved[1] == "x");
        REQUIRE(moved[2] == "y");
        REQUIRE(original.empty());
    }

    SECTION("Iterator range constructor") {
        std::vector<std::pair<int, std::string>> vec = {{3, "c"}, {1, "a"}, {2, "b"}};
        Table<int, std::string> table(vec.begin(), vec.end());
        REQUIRE(table.size() == 3);
        REQUIRE(table[1] == "a");
        REQUIRE(table[2] == "b");
        REQUIRE(table[3] == "c");
    }

    SECTION("Copy assignment operator") {
        Table<int, std::string> table1 = {{1, "a"}};
        Table<int, std::string> table2 = {{2, "b"}, {3, "c"}};
        table1 = table2;
        REQUIRE(table1.size() == 2);
        REQUIRE(table1[2] == "b");
        REQUIRE(table1[3] == "c");
        REQUIRE(table2.size() == 2);
    }

    SECTION("Move assignment operator") {
        Table<int, std::string> table1 = {{1, "a"}};
        Table<int, std::string> table2 = {{2, "b"}, {3, "c"}};
        table1 = std::move(table2);
        REQUIRE(table1.size() == 2);
        REQUIRE(table1[2] == "b");
        REQUIRE(table1[3] == "c");
        REQUIRE(table2.empty());
    }
}


TEST_CASE("Capacity and memory management") {
    Table<int, std::string> table;

    SECTION("Empty and size") {
        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);
        table.insert(TablePair<int, std::string>(1, "one"));
        REQUIRE_FALSE(table.empty());
        REQUIRE(table.size() == 1);
    }

    SECTION("Max size") {
        REQUIRE(table.max_size() > 0);
        REQUIRE(table.max_size() >= table.size());
    }

    SECTION("Reserve and capacity") {
        table.reserve(100);
        REQUIRE(table.capacity() >= 100);

        size_t old_capacity = table.capacity();
        table.reserve(50);
        REQUIRE(table.capacity() == old_capacity);

        table.insert(TablePair<int, std::string>(1, "one"));
        table.insert(TablePair<int, std::string>(2, "two"));
        REQUIRE(table.capacity() >= 100);

        table.reserve(1000);
        REQUIRE(table.capacity() >= 1000);
        REQUIRE(table[1] == "one");
        REQUIRE(table[2] == "two");
    }

    SECTION("Shrink to fit") {
        table.reserve(100);
        REQUIRE(table.capacity() >= 100);

        table.insert(TablePair<int, std::string>(1, "one"));
        table.insert(TablePair<int, std::string>(2, "two"));
        table.shrink_to_fit();

        REQUIRE(table.capacity() >= 2);
        REQUIRE(table.capacity() < 100);
        REQUIRE(table[1] == "one");
        REQUIRE(table[2] == "two");
    }

    SECTION("Clear") {
        table = {{1, "a"}, {2, "b"}, {3, "c"}};
        REQUIRE(table.size() == 3);
        table.clear();
        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);
        table.insert(TablePair<int, std::string>(4, "d"));
        REQUIRE(table.size() == 1);
    }
}


TEST_CASE("Insert operations") {
    Table<int, std::string> table;

    SECTION("Insert range") {
        std::vector<std::pair<int, std::string>> vec = {{1, "x"}, {2, "y"}, {1, "z"}};
        table.insert(vec.begin(), vec.end());
        REQUIRE(table.size() == 2);
        REQUIRE(table[1] == "x");
        REQUIRE(table[2] == "y");
    }

    SECTION("Insert initializer_list") {
        table.insert({{1, "a"}, {2, "b"}, {1, "c"}});
        REQUIRE(table.size() == 2);
        REQUIRE(table[1] == "a");
        REQUIRE(table[2] == "b");
    }

    SECTION("Insert or assign") {
        auto [it1, ins1] = table.insert_or_assign(1, "new");
        REQUIRE(ins1);
        REQUIRE(it1->value == "new");

        auto [it2, ins2] = table.insert_or_assign(1, "updated");
        REQUIRE_FALSE(ins2);
        REQUIRE(it2->value == "updated");

        auto [it3, inserted3] = table.insert_or_assign(2, std::string("temp"));
        REQUIRE(inserted3);
        REQUIRE(it3->value == "temp");
    }

    SECTION("Insert new element with TablePair rvalue") {
        Table<int, std::string> table;

        TablePair<int, std::string> pair(1, std::string("one"));
        auto [it, inserted] = table.insert(std::move(pair));

        REQUIRE(inserted);
        REQUIRE(it->key == 1);
        REQUIRE(it->value == "one");
    }

    SECTION("Insert existing element with TablePair rvalue") {
        Table<int, std::string> table = {{1, "old_value"}};
        TablePair<int, std::string> pair(1, std::string("new_value"));
        auto [it, inserted] = table.insert(std::move(pair));
        REQUIRE_FALSE(inserted);
        REQUIRE(it->key == 1);
        REQUIRE(it->value == "old_value");
        REQUIRE(table.size() == 1);
    }

    SECTION("Insert multiple elements with TablePair move") {
        Table<int, std::string> table;
        table.insert(TablePair<int, std::string>(3, std::string("three")));
        table.insert(TablePair<int, std::string>(1, std::string("one")));
        table.insert(TablePair<int, std::string>(2, std::string("two")));

        REQUIRE(table.size() == 3);
        REQUIRE(table[1] == "one");
        REQUIRE(table[2] == "two");
        REQUIRE(table[3] == "three");
        std::vector<int> keys;
        for (const auto& p : table) keys.push_back(p.key);
        REQUIRE(keys == std::vector<int>{1, 2, 3});
    }

    SECTION("Insert TablePair with complex value type") {
        Table<int, std::vector<int>> table;

        std::vector<int> data = {1, 2, 3, 4, 5};
        TablePair<int, std::vector<int>> pair(42, std::move(data));

        auto [it, inserted] = table.insert(std::move(pair));

        REQUIRE(inserted);
        REQUIRE(it->key == 42);
        REQUIRE(it->value == std::vector<int>{1, 2, 3, 4, 5});
        REQUIRE(data.empty());
    }

    SECTION("Insert or assign with perfect forwarding") {
        Table<std::string, std::string> str_table;

        std::string key1 = "key1";
        std::string value1 = "value1";
        auto [it1, ins1] = str_table.insert_or_assign(key1, value1);
        REQUIRE(ins1);
        REQUIRE(str_table["key1"] == "value1");

        std::string value2 = "value2";
        auto [it2, ins2] = str_table.insert_or_assign(key1, std::move(value2));
        REQUIRE_FALSE(ins2);
        REQUIRE(str_table["key1"] == "value2");
        REQUIRE(value2.empty());

        std::string key3 = "key3";
        std::string value3 = "value3";
        auto [it3, inserted3] = str_table.insert_or_assign(std::move(key3), value3);
        REQUIRE(inserted3);
        REQUIRE(str_table["key3"] == "value3");
        REQUIRE(key3.empty());

        std::string key4 = "key4";
        std::string value4 = "value4";
        auto [it4, inserted4] = str_table.insert_or_assign(std::move(key4), std::move(value4));
        REQUIRE(inserted4);
        REQUIRE(str_table["key4"] == "value4");
        REQUIRE(key4.empty());
        REQUIRE(value4.empty());
    }

    SECTION("Emplace") {
        auto [it1, ins1] = table.emplace(1, "first");
        REQUIRE(ins1);
        REQUIRE(it1->key == 1);
        REQUIRE(it1->value == "first");

        auto [it2, ins2] = table.emplace(1, "second");
        REQUIRE_FALSE(ins2);
        REQUIRE(it2->value == "first");
    }

    SECTION("Try emplace") {
        Table<std::string, int> int_table;

        auto [it1, ins1] = int_table.try_emplace(std::string("key"), 42);
        REQUIRE(ins1);
        REQUIRE(it1->value == 42);

        auto [it2, ins2] = int_table.try_emplace(std::string("key"), 99);
        REQUIRE_FALSE(ins2);
        REQUIRE(it2->value == 42);
    }
}


TEST_CASE("Erase operations") {
    Table<int, std::string> table = {{1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}};

    SECTION("Erase by key") {
        bool removed = table.erase(2);
        REQUIRE(removed);
        REQUIRE(table.size() == 3);
        REQUIRE_FALSE(table.contains(2));

        bool not_removed = table.erase(99);
        REQUIRE_FALSE(not_removed);
        REQUIRE(table.size() == 3);
    }

    SECTION("Erase by iterator") {
        auto it = table.find(2);
        REQUIRE(it != table.end());
        auto next = table.erase(it);
        REQUIRE(table.size() == 3);
        REQUIRE_FALSE(table.contains(2));
        REQUIRE(next->key == 3);
    }

    SECTION("Erase by const_iterator") {
        auto it = table.cbegin();
        ++it;
        table.erase(it);
        REQUIRE(table.size() == 3);
        REQUIRE_FALSE(table.contains(2));
    }

    SECTION("Erase range") {
        auto first = table.begin();
        auto last = table.begin();
        std::advance(last, 2);
        table.erase(first, last);
        REQUIRE(table.size() == 2);
        REQUIRE_FALSE(table.contains(1));
        REQUIRE_FALSE(table.contains(2));
        REQUIRE(table.contains(3));
        REQUIRE(table.contains(4));
    }
}

TEST_CASE("Element access") {
    Table<int, std::string> table = {{1, "one"}, {3, "three"}};

    SECTION("At method") {
        REQUIRE(table.at(1) == "one");
        REQUIRE(table.at(3) == "three");
        table.at(1) = "uno";
        REQUIRE(table.at(1) == "uno");
        REQUIRE_THROWS_AS(table.at(99), std::out_of_range);
    }

    SECTION("Const at method") {
        const Table<int, std::string> const_table = {{1, "one"}, {3, "three"}};
        REQUIRE(const_table.at(1) == "one");
        REQUIRE(const_table.at(3) == "three");
        REQUIRE_THROWS_AS(const_table.at(99), std::out_of_range);
    }

    SECTION("Operator[] - basic functionality") {
        REQUIRE(table[1] == "one");
        table[1] = "uno";
        REQUIRE(table[1] == "uno");
        REQUIRE(table.size() == 2);

        std::string& value = table[2];
        REQUIRE(table.size() == 3);
        REQUIRE(value == "");
        REQUIRE(table[2] == "");

        table[2] = "two";
        REQUIRE(table[2] == "two");
        REQUIRE(table.size() == 3);
    }

    // SECTION("Operator[] - edge cases") {
    //     table = {{1, "a"}, {5, "e"}};
    //     table[3];
    //     REQUIRE(table.size() == 3);
    //     REQUIRE(table[3] == "");
    //     table = {{5, "e"}, {10, "j"}};
    //     table[1];
    //     REQUIRE(table.size() == 3);
    //     REQUIRE(table[1] == "");
    //     table = {{1, "a"}, {5, "e"}};
    //     table[10];
    //     REQUIRE(table.size() == 3);
    //     REQUIRE(table[10] == "");
    // }

    SECTION("Operator[] with rvalue key") {
        Table<std::string, int> str_table;
        str_table[std::string("key1")] = 5;
        REQUIRE(str_table["key1"] == 5);
        REQUIRE(str_table.size() == 1);

        str_table["key2"];
        REQUIRE(str_table.size() == 2);
        REQUIRE(str_table["key2"] == 0);

        str_table["key1"] = 10;
        REQUIRE(str_table["key1"] == 10);
        REQUIRE(str_table.size() == 2);
    }

    SECTION("Operator[] with complex types") {
        Table<std::string, std::vector<int>> complex_table;
        complex_table["key1"];
        REQUIRE(complex_table.size() == 1);
        REQUIRE(complex_table["key1"].empty());

        complex_table["key1"].push_back(1);
        REQUIRE(complex_table["key1"] == std::vector<int>{1});

        complex_table["key2"] = {1, 2, 3};
        REQUIRE(complex_table["key2"] == std::vector<int>{1, 2, 3});
        REQUIRE(complex_table.size() == 2);
    }

    // SECTION("Extract method") {
    //     table = {{1, "one"}, {2, "two"}};
    //     std::string value = table.extract(1);
    //     REQUIRE(value == "one");
    //     REQUIRE(table.size() == 1);
    //     REQUIRE_FALSE(table.contains(1));
    //     REQUIRE_THROWS_AS(table.extract(99), std::out_of_range);
    // }
}

TEST_CASE("Search operations") {
    Table<int, std::string> table = {{1, "a"}, {3, "c"}, {5, "e"}, {7, "g"}};
    const Table<int, std::string> const_table = {{1, "a"}, {3, "c"}, {5, "e"}, {7, "g"}};

    SECTION("Find") {
        auto it = table.find(3);
        REQUIRE(it != table.end());
        REQUIRE(it->key == 3);
        REQUIRE(it->value == "c");

        auto not_found = table.find(99);
        REQUIRE(not_found == table.end());
    }

    SECTION("Const find") {
        auto it = const_table.find(3);
        REQUIRE(it != const_table.end());
        REQUIRE(it->key == 3);
        REQUIRE(it->value == "c");
    }

    SECTION("Contains") {
        REQUIRE(table.contains(3));
        REQUIRE_FALSE(table.contains(4));
    }

    SECTION("Count") {
        REQUIRE(table.count(3) == 1);
        REQUIRE(table.count(4) == 0);
    }

    SECTION("Lower bound") {
        auto it1 = table.lower_bound(3);
        REQUIRE(it1 != table.end());
        REQUIRE(it1->key == 3);

        auto it2 = table.lower_bound(4);
        REQUIRE(it2 != table.end());
        REQUIRE(it2->key == 5);

        auto it3 = table.lower_bound(10);
        REQUIRE(it3 == table.end());
    }

    SECTION("Const lower bound") {
        auto it1 = const_table.lower_bound(3);
        REQUIRE(it1 != const_table.end());
        REQUIRE(it1->key == 3);
    }

    SECTION("Upper bound") {
        auto it1 = table.upper_bound(3);
        REQUIRE(it1 != table.end());
        REQUIRE(it1->key == 5);

        auto it2 = table.upper_bound(4);
        REQUIRE(it2 != table.end());
        REQUIRE(it2->key == 5);

        auto it3 = table.upper_bound(7);
        REQUIRE(it3 == table.end());
    }

    SECTION("Const upper bound") {
        auto it1 = const_table.upper_bound(3);
        REQUIRE(it1 != const_table.end());
        REQUIRE(it1->key == 5);
    }

    SECTION("Equal range") {
        table = {{1, "a"}, {2, "b"}, {4, "d"}};
        auto [lower, upper] = table.equal_range(2);
        REQUIRE(lower->key == 2);
        REQUIRE(lower->value == "b");
        REQUIRE(upper->key == 4);
        REQUIRE(std::distance(lower, upper) == 1);

        auto [lower2, upper2] = table.equal_range(3);
        REQUIRE(lower2 == upper2);
        REQUIRE(lower2->key == 4);
    }

    SECTION("Const equal range") {
        const Table<int, std::string> const_table2 = {{1, "a"}, {2, "b"}, {4, "d"}};
        auto [lower, upper] = const_table2.equal_range(2);
        REQUIRE(lower->key == 2);
        REQUIRE(lower->value == "b");
        REQUIRE(upper->key == 4);
        REQUIRE(std::distance(lower, upper) == 1);
    }
}

TEST_CASE("Iterators") {
    Table<int, std::string> table = {{1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}};
    const Table<int, std::string> const_table = {{1, "a"}, {2, "b"}, {3, "c"}};

    SECTION("Forward iterators") {
        auto it = table.begin();
        REQUIRE(it->key == 1);
        REQUIRE(it->value == "a");
        ++it;
        REQUIRE(it->key == 2);
        REQUIRE(it->value == "b");

        auto begin = table.begin();
        auto end = table.end();
        REQUIRE(std::distance(begin, end) == 4);
    }

    SECTION("Const forward iterators") {
        auto it = const_table.begin();
        REQUIRE(it->key == 1);
        REQUIRE(it->value == "a");
        ++it;
        REQUIRE(it->key == 2);
        REQUIRE(it->value == "b");
    }

    SECTION("Reverse iterators - basic functionality") {
        std::vector<int> keys;
        for (auto it = table.rbegin(); it != table.rend(); ++it) {
            keys.push_back(it->key);
        }
        REQUIRE(keys == std::vector<int>{4, 3, 2, 1});

        std::vector<std::string> values;
        for (auto it = table.rbegin(); it != table.rend(); ++it) {
            values.push_back(it->value);
        }
        REQUIRE(values == std::vector<std::string>{"d", "c", "b", "a"});
    }

    SECTION("Reverse iterators - modification") {
        auto rit = table.rbegin();
        REQUIRE(rit->key == 4);
        REQUIRE(rit->value == "d");

        rit->value = "D";
        REQUIRE(table[4] == "D");

        ++rit;
        REQUIRE(rit->key == 3);
        rit->value = "C";
        REQUIRE(table[3] == "C");
    }

    SECTION("Reverse iterators - arithmetic operations") {
        auto rit1 = table.rbegin();
        auto rit2 = table.rbegin() + 2;

        REQUIRE(rit2 - rit1 == 2);
        REQUIRE(rit1 < rit2);
        REQUIRE(rit2 > rit1);
        REQUIRE(rit1 <= rit2);
        REQUIRE(rit2 >= rit1);

        auto rit3 = rit1 + 3;
        REQUIRE(rit3->key == 1);
        REQUIRE(rit3->value == "a");

        auto rit4 = rit3 - 1;
        REQUIRE(rit4->key == 2);
        REQUIRE(rit4->value == "b");
    }

    SECTION("Reverse iterators - indexing") {
        auto rit = table.rbegin();
        REQUIRE(rit[0].key == 4);
        REQUIRE(rit[0].value == "d");
        REQUIRE(rit[1].key == 3);
        REQUIRE(rit[1].value == "c");
        REQUIRE(rit[2].key == 2);
        REQUIRE(rit[2].value == "b");
        REQUIRE(rit[3].key == 1);
        REQUIRE(rit[3].value == "a");
    }

    SECTION("Reverse iterators - decrement") {
        auto rit = table.rend();
        --rit;
        REQUIRE(rit->key == 1);
        REQUIRE(rit->value == "a");
        --rit;
        REQUIRE(rit->key == 2);
        REQUIRE(rit->value == "b");

        auto rit2 = table.rend();
        rit2--;
        REQUIRE(rit2->key == 1);
    }

    SECTION("Const reverse iterators") {
        std::vector<int> keys;
        for (auto it = const_table.crbegin(); it != const_table.crend(); ++it) {
            keys.push_back(it->key);
        }
        REQUIRE(keys == std::vector<int>{3, 2, 1});

        std::vector<std::string> values;
        for (auto it = const_table.crbegin(); it != const_table.crend(); ++it) {
            values.push_back(it->value);
        }
        REQUIRE(values == std::vector<std::string>{"c", "b", "a"});
    }

    SECTION("Const reverse iterators - arithmetic operations") {
        auto rit1 = const_table.crbegin();
        auto rit2 = const_table.crbegin() + 2;

        REQUIRE(rit2 - rit1 == 2);
        REQUIRE(rit1 < rit2);
        REQUIRE(rit2 > rit1);

        REQUIRE((rit1 + 1)->key == 2);
        REQUIRE((rit2 - 1)->key == 2);
    }

    SECTION("Reverse iterators on empty table") {
        Table<int, std::string> empty_table;
        REQUIRE(empty_table.rbegin() == empty_table.rend());
        REQUIRE(empty_table.crbegin() == empty_table.crend());

        const Table<int, std::string> const_empty_table;
        REQUIRE(const_empty_table.rbegin() == const_empty_table.rend());
        REQUIRE(const_empty_table.crbegin() == const_empty_table.crend());
    }

    SECTION("Reverse iterators - conversion between const and non-const") {
        Table<int, std::string>::reverse_iterator non_const_rit = table.rbegin();
        Table<int, std::string>::const_reverse_iterator const_rit = non_const_rit;
        REQUIRE(const_rit->key == 4);
        REQUIRE(const_rit->value == "d");
    }

    SECTION("Reverse iterators - comparison with forward iterators") {
        auto rit = table.rbegin();
        auto it = table.end();
        --it;
        REQUIRE(rit->key == it->key);
        REQUIRE(rit->value == it->value);

        ++rit;
        --it;
        REQUIRE(rit->key == it->key);
        REQUIRE(rit->value == it->value);
    }

    SECTION("Reverse iterators - modification through dereference") {
        auto rit = table.rbegin();
        auto& ref = *rit;
        ref.value = "MODIFIED";
        REQUIRE(table[4] == "MODIFIED");

        auto rit2 = table.rbegin() + 1;
        rit2->value = "ALSO_MODIFIED";
        REQUIRE(table[3] == "ALSO_MODIFIED");
    }

    SECTION("Iterator arithmetic") {
        auto it1 = table.begin();
        auto it2 = table.begin() + 2;
        REQUIRE(it2 - it1 == 2);
        REQUIRE(it1 < it2);
        REQUIRE(it2 > it1);
        REQUIRE((it1 + 2) == it2);
        REQUIRE((it2 - 2) == it1);
    }

    SECTION("Iterator dereference and arrow") {
        auto it = table.begin();
        REQUIRE((*it).key == 1);
        REQUIRE((*it).value == "a");
        REQUIRE(it->key == 1);
        REQUIRE(it->value == "a");
    }

    SECTION("Const iterator dereference and arrow") {
        auto it = const_table.begin();
        REQUIRE((*it).key == 1);
        REQUIRE((*it).value == "a");
        REQUIRE(it->key == 1);
        REQUIRE(it->value == "a");
    }
}


TEST_CASE("Comparison operations") {
    SECTION("Equality and inequality") {
        Table<int, std::string> table1 = {{1, "a"}, {2, "b"}};
        Table<int, std::string> table2 = {{1, "a"}, {2, "b"}};
        Table<int, std::string> table3 = {{1, "a"}, {2, "c"}};
        Table<int, std::string> table4 = {{1, "a"}, {3, "b"}};
        Table<int, std::string> table5 = {{1, "a"}};

        REQUIRE(table1 == table2);
        REQUIRE_FALSE(table1 == table3);
        REQUIRE_FALSE(table1 == table4);
        REQUIRE_FALSE(table1 == table5);

        REQUIRE_FALSE(table1 != table2);
        REQUIRE(table1 != table3);
    }

    SECTION("Relational operators") {
        Table<int, std::string> table1 = {{1, "a"}, {2, "b"}};
        Table<int, std::string> table2 = {{1, "a"}, {2, "c"}};
        Table<int, std::string> table3 = {{1, "b"}, {2, "a"}};
        Table<int, std::string> table4 = {{1, "a"}, {2, "b"}, {3, "c"}};

        REQUIRE(table1 < table2);
        REQUIRE(table1 < table3);
        REQUIRE(table1 < table4);
        REQUIRE_FALSE(table2 < table1);

        REQUIRE(table1 <= table2);
        REQUIRE(table1 <= table4);

        REQUIRE(table2 > table1);
        REQUIRE(table4 > table1);

        REQUIRE(table2 >= table1);
        REQUIRE(table4 >= table1);
    }
}


TEST_CASE("Special cases and edge cases") {
    SECTION("Empty table operations") {
        Table<int, std::string> table;
        REQUIRE(table.find(1) == table.end());
        REQUIRE(table.lower_bound(1) == table.end());
        REQUIRE(table.upper_bound(1) == table.end());
        REQUIRE(table.equal_range(1).first == table.end());
        REQUIRE(table.equal_range(1).second == table.end());
        REQUIRE(table.count(1) == 0);
        REQUIRE_FALSE(table.contains(1));
        REQUIRE_THROWS_AS(table.at(1), std::out_of_range);
        REQUIRE(table.erase(1) == 0);
    }

    SECTION("Swap operations") {
        Table<int, std::string> table1 = {{1, "a"}, {2, "b"}};
        Table<int, std::string> table2 = {{3, "c"}, {4, "d"}};

        table1.swap(table2);
        REQUIRE(table1.size() == 2);
        REQUIRE(table1.contains(3));
        REQUIRE(table1.contains(4));
        REQUIRE(table2.size() == 2);
        REQUIRE(table2.contains(1));
        REQUIRE(table2.contains(2));

        swap(table1, table2);
        REQUIRE(table1.contains(1));
        REQUIRE(table1.contains(2));
        REQUIRE(table2.contains(3));
        REQUIRE(table2.contains(4));
    }

    SECTION("TablePair move constructor") {
        std::string key = "key";
        std::string value = "value";
        TablePair<std::string, std::string> pair1(key, value);
        TablePair<std::string, std::string> pair2(std::move(pair1));
        REQUIRE(pair2.key == "key");
        REQUIRE(pair2.value == "value");
        REQUIRE(pair1.key == "key");
    }

    SECTION("Stress test for operator[]") {
        Table<int, int> table;
        for (int i = 100; i >= 0; --i) {
            table[i];
        }
        REQUIRE(table.size() == 101);

        for (int i = 0; i <= 100; ++i) {
            REQUIRE(table.contains(i));
            REQUIRE(table[i] == 0);
        }

        for (int i = 0; i <= 100; i += 2) {
            table[i] = i * 2;
        }

        for (int i = 0; i <= 100; ++i) {
            if (i % 2 == 0) REQUIRE(table[i] == i * 2);
            else REQUIRE(table[i] == 0);
        }
    }
}