#include "Pack.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(DefaultConstructorOrder) {
    Pack pack;

    Card first = pack.deal_one();
    ASSERT_EQUAL(first, Card(NINE, SPADES));

    Card last;
    for (int i = 0; i < 23; ++i) {
        last = pack.deal_one();
    }

    ASSERT_EQUAL(last, Card(ACE, DIAMONDS));
}

TEST(EmptyTest) {
    Pack pack;

    ASSERT_FALSE(pack.empty());

    for (int i = 0; i < 24; ++i) {
        pack.deal_one();
    }

    ASSERT_TRUE(pack.empty());
}

TEST(ResetTest) {
    Pack pack;

    pack.deal_one();
    pack.deal_one();

    pack.reset();

    Card first = pack.deal_one();
    ASSERT_EQUAL(first, Card(NINE, SPADES));
}

TEST(StreamConstructor) {
    string input =
        "Nine of Spades\n"
        "Ten of Spades\n"
        "Jack of Spades\n"
        "Queen of Spades\n"
        "King of Spades\n"
        "Ace of Spades\n"
        "Nine of Hearts\n"
        "Ten of Hearts\n"
        "Jack of Hearts\n"
        "Queen of Hearts\n"
        "King of Hearts\n"
        "Ace of Hearts\n"
        "Nine of Clubs\n"
        "Ten of Clubs\n"
        "Jack of Clubs\n"
        "Queen of Clubs\n"
        "King of Clubs\n"
        "Ace of Clubs\n"
        "Nine of Diamonds\n"
        "Ten of Diamonds\n"
        "Jack of Diamonds\n"
        "Queen of Diamonds\n"
        "King of Diamonds\n"
        "Ace of Diamonds\n";

    istringstream iss(input);
    Pack pack(iss);

    ASSERT_EQUAL(pack.deal_one(), Card(NINE, SPADES));
}

TEST(ShuffleChangesOrder) {
    Pack pack1;
    Pack pack2;

    pack2.shuffle();

    bool different = false;

    for (int i = 0; i < 24; ++i) {
        if (pack1.deal_one() != pack2.deal_one()) {
            different = true;
        }
    }

    ASSERT_TRUE(different);
}

TEST(ShuffleResetsIndex) {
    Pack pack;

    pack.deal_one();
    pack.shuffle();

    ASSERT_FALSE(pack.empty());
}

TEST_MAIN()
