#include <Trie/trie.hpp>
#include <gtest/gtest.h>

class TrieTest : public ::testing::Test {
protected:
    void SetUp() override {
        t.insert("wargame");
        t.insert("wombat");
        t.insert("wolfram");
        t.insert("world");
        t.insert("work");
    }

    trie<std::string> t;
};

TEST_F(TrieTest, SizeTest) {
ASSERT_EQ(t.size(), 5);
}

TEST_F(TrieTest, HasTest) {
ASSERT_FALSE(t.has("war"));
ASSERT_TRUE(t.has("wargame"));
}

TEST_F(TrieTest, CompleteTest) {
auto candidates = t.complete("wo");
ASSERT_EQ(candidates.size(), 4);

candidates = t.complete("warg");
ASSERT_EQ(candidates.size(), 1);
ASSERT_EQ(*candidates[0], "wargame");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
