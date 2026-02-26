#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

TEST(SuitRank) {
    Card two_spades = Card();
    ASSERT_EQUAL(two_spades.get_rank(), TWO);
    ASSERT_EQUAL(two_spades.get_suit(), SPADES);

    Card three_spades = Card(THREE, SPADES);
    ASSERT_EQUAL(three_spades.get_rank(), THREE);
    ASSERT_EQUAL(three_spades.get_suit(), SPADES);
    ASSERT_EQUAL(three_spades.get_suit(CLUBS), SPADES);
}

TEST(CardType) {
    Card three_spades = Card(THREE, SPADES);
    ASSERT_FALSE(three_spades.is_face_or_ace());
    ASSERT_FALSE(three_spades.is_right_bower(CLUBS));
    ASSERT_FALSE(three_spades.is_left_bower(CLUBS));
    ASSERT_FALSE(three_spades.is_trump(CLUBS));
}

TEST(CardSelf) {
    Card three_spades = Card(THREE, SPADES);
    ASSERT_FALSE(three_spades < three_spades);
    ASSERT_TRUE(three_spades <= three_spades);
    ASSERT_FALSE(three_spades > three_spades);
    ASSERT_TRUE(three_spades >= three_spades);
    ASSERT_TRUE(three_spades == three_spades);
    ASSERT_FALSE(three_spades != three_spades);
}

TEST(SuitNext) {
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
}

TEST(LessSelf) {
    Card three_spades = Card(THREE, SPADES);
    ASSERT_FALSE(Card_less(three_spades, three_spades, CLUBS));
    ASSERT_FALSE(Card_less(three_spades, three_spades, three_spades,
                           CLUBS));
}

TEST(Insertion) {
    Card three_spades = Card(THREE, SPADES);
    ostringstream oss;
    oss << three_spades;
    ASSERT_EQUAL(oss.str(), "Three of Spades");
}

TEST(Extraction) {
    istringstream iss("Three of Spades");
    Card c;
    iss >> c;
    ASSERT_EQUAL(c, Card(THREE, SPADES));
}

TEST(RightBower) {
    Card jack_hearts(JACK, HEARTS);

    ASSERT_TRUE(jack_hearts.is_right_bower(HEARTS));
    ASSERT_FALSE(jack_hearts.is_right_bower(SPADES));
}

TEST(LeftBower) {
    Card jack_diamonds(JACK, DIAMONDS);

    ASSERT_TRUE(jack_diamonds.is_left_bower(HEARTS));
    ASSERT_FALSE(jack_diamonds.is_left_bower(SPADES));
}

TEST(GetSuitTrump) {
    Card jack_diamonds(JACK, DIAMONDS);

    ASSERT_EQUAL(jack_diamonds.get_suit(HEARTS), HEARTS);
}

TEST(IsTrump) {
    Card ace_hearts(ACE, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);

    ASSERT_TRUE(ace_hearts.is_trump(HEARTS));
    ASSERT_TRUE(jack_diamonds.is_trump(HEARTS));
    ASSERT_FALSE(ace_hearts.is_trump(SPADES));
}

TEST(RankComparison) {
    Card ace_spades(ACE, SPADES);
    Card king_spades(KING, SPADES);

    ASSERT_TRUE(king_spades < ace_spades);
}

TEST(TrumpBeatsNonTrump) {
    Card ace_hearts(ACE, HEARTS);
    Card ace_spades(ACE, SPADES);

    ASSERT_TRUE(Card_less(ace_spades, ace_hearts, 
                            ace_spades, HEARTS));
}

TEST(RightBowerHighest) {
    Card jack_hearts(JACK, HEARTS);
    Card ace_hearts(ACE, HEARTS);

    ASSERT_FALSE(Card_less(jack_hearts, ace_hearts, 
                             jack_hearts, HEARTS));
}

TEST(LeftBowerBeatsOtherTrump) {
    Card jack_diamonds(JACK, DIAMONDS); 
    Card ace_hearts(ACE, HEARTS);

    ASSERT_FALSE(Card_less(jack_diamonds, ace_hearts,
                           jack_diamonds, HEARTS));
}

TEST(LedSuitMatters) {
    Card ace_spades(ACE, SPADES);
    Card king_clubs(KING, CLUBS);
    Card led_spades(THREE, SPADES);

    ASSERT_FALSE(Card_less(ace_spades, king_clubs,
                           led_spades, HEARTS));
}

//TEST(NeitherTrumpNorLed) {
//    Card ten_clubs(TEN, CLUBS);
//    Card nine_spades(NINE, SPADES);
//    Card led_hearts(THREE, HEARTS);

//    ASSERT_TRUE(Card_less(ten_clubs, nine_spades,
//                          led_hearts, HEARTS));
//}

TEST(NonTrumpSameSuit) {
    Card ten_clubs(TEN, CLUBS);
    Card nine_clubs(NINE, CLUBS);
    Card led_hearts(THREE, HEARTS);

    ASSERT_FALSE(Card_less(ten_clubs, nine_clubs,
                           led_hearts, HEARTS));
}

TEST(SuitNextAll) {
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
    ASSERT_EQUAL(Suit_next(SPADES), CLUBS);
    ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
    ASSERT_EQUAL(Suit_next(DIAMONDS), HEARTS);
}

TEST(FaceOrAce) {
    ASSERT_TRUE(Card(JACK, SPADES).is_face_or_ace());
    ASSERT_TRUE(Card(QUEEN, SPADES).is_face_or_ace());
    ASSERT_TRUE(Card(KING, SPADES).is_face_or_ace());
    ASSERT_TRUE(Card(ACE, SPADES).is_face_or_ace());
    ASSERT_FALSE(Card(TEN, SPADES).is_face_or_ace());
}

TEST(LeftBowerRedBlack) {
    ASSERT_TRUE(Card(JACK, DIAMONDS).is_left_bower(HEARTS));
    ASSERT_TRUE(Card(JACK, SPADES).is_left_bower(CLUBS));
}

TEST(RightBeatsLeft) {
    Card right(JACK, HEARTS);
    Card left(JACK, DIAMONDS);

    ASSERT_FALSE(Card_less(right, left, right, HEARTS));
}

TEST(GetSuitNormalCard) {
    Card ace_spades(ACE, SPADES);
    ASSERT_EQUAL(ace_spades.get_suit(HEARTS), SPADES);
}

TEST(CardLessRankOnly) {
    ASSERT_TRUE(Card(TWO, CLUBS) < Card(THREE, CLUBS));
}

TEST(TrumpBeatsLed) {
    Card trump_card(TEN, HEARTS);
    Card led_card(ACE, SPADES);

    ASSERT_FALSE(Card_less(trump_card, led_card,
                           led_card, HEARTS));
}

TEST(LedSuitRankOrder) {
    Card king_spades(KING, SPADES);
    Card queen_spades(QUEEN, SPADES);
    Card led_spades(TWO, SPADES);

    ASSERT_TRUE(Card_less(queen_spades, king_spades,
                          led_spades, HEARTS));
}

TEST(LedBeatsOther) {
    Card led_card(TWO, CLUBS);
    Card ace_clubs(ACE, CLUBS);
    Card ace_spades(ACE, SPADES);

    ASSERT_FALSE(Card_less(ace_clubs, ace_spades,
                           led_card, HEARTS));
}

TEST(OperatorConsistency) {
    Card two(TWO, CLUBS);
    Card three(THREE, CLUBS);

    ASSERT_TRUE(two < three);
    ASSERT_TRUE(two <= three);
    ASSERT_TRUE(three > two);
    ASSERT_TRUE(three >= two);
    ASSERT_TRUE(two != three);
}

TEST_MAIN()