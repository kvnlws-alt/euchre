#include "Player.hpp"
#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <sstream>
#include <string>


using namespace std;

TEST(test_player_insertion) {
  // Create a Human player
  Player * human = Player_factory("NotRobot", "Human");

  // Print the player using the stream insertion operator
  ostringstream oss1;
  oss1 << * human;

  // Verify that the output is the player's name
  ASSERT_EQUAL(oss1.str(), "NotRobot");

  // Create a Simple player
  Player * alice = Player_factory("Alice", "Simple");

  // Print the player using the stream insertion operator
  ostringstream oss2;
  oss2 << *alice;
  ASSERT_EQUAL(oss2.str(), "Alice");

  // Clean up players that were created using Player_factory()
  delete human;
  delete alice;
}

TEST(test_player_get_name) {
  // Create a player and verify that get_name() returns the player's name
  Player * alice = Player_factory("Alice", "Simple");
  ASSERT_EQUAL(alice->get_name(), "Alice");
  delete alice;
}

TEST(test_simple_player_make_trump) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob makes tump
  Card nine_spades(NINE, SPADES);
  Suit trump;
  bool orderup = bob->make_trump(
    nine_spades,    // Upcard
    true,           // Bob is also the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, SPADES);

  delete bob;
}

TEST(test_simple_player_lead_card) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob adds a card to his hand and discards one card
  bob->add_and_discard(
    Card(NINE, HEARTS) // upcard
  );

  // Bob leads
  Card card_led = bob->lead_card(HEARTS);

  // Verify the card Bob selected to lead
  Card ace_spades(ACE, SPADES);
  ASSERT_EQUAL(card_led, ace_spades); //check led card

  delete bob;
}

TEST(test_simple_player_play_card) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob plays a card
  Card nine_diamonds(NINE, DIAMONDS);
  Card card_played = bob->play_card(
    nine_diamonds,  // Nine of Diamonds is led
    HEARTS    // Trump suit
  ); 

  // Verify the card Bob played
  ASSERT_EQUAL(card_played, Card(NINE, SPADES));
  delete bob;
}

TEST(TestPlayerFactorySimple) {
    Player* p = Player_factory("Kevin", "Simple");
    ASSERT_EQUAL(p->get_name(), "Kevin");

    p->add_card(Card(TWO, SPADES));
    Card c = p->lead_card(SPADES); 
    ASSERT_EQUAL(c, Card(TWO, SPADES));

    delete p; 
}

TEST(TestMakeTrumpSimpleTwo) {
    Player* p = Player_factory("Vishal", "Simple");
    Card upcard(JACK, DIAMONDS);
    Suit trump = SPADES;

    bool ordered = p->make_trump(upcard, false, 2, trump);

    ASSERT_FALSE(ordered);
    delete p;

    Player *p2 = Player_factory("Vish","Simple");
    p2->add_card(Card(TWO, SPADES));
    p2->add_card(Card(THREE, CLUBS));
    p2->add_card(Card(FOUR, SPADES));
    p2->add_card(Card(FIVE, CLUBS));
    p2->add_card(Card(SIX, SPADES));
    ASSERT_TRUE(p2->make_trump(Card(JACK, HEARTS), true, 2, trump));
    ASSERT_EQUAL(trump, DIAMONDS);
    delete p2;

    Player *p3 = Player_factory("V","Simple");
    p3->add_card(Card(TWO, SPADES));
    p3->add_card(Card(THREE, CLUBS));
    p3->add_card(Card(FOUR, SPADES));
    p3->add_card(Card(FIVE, CLUBS));
    p3->add_card(Card(SIX, SPADES));
    ASSERT_FALSE(p3->make_trump(Card(JACK, HEARTS), false, 2, trump));
    delete p3;

    Player *p4 = Player_factory("V","Simple");
    p4->add_card(Card(KING, DIAMONDS));
    p4->add_card(Card(TWO, SPADES));
    p4->add_card(Card(THREE, CLUBS));
    p4->add_card(Card(FOUR, SPADES));
    p4->add_card(Card(FIVE, CLUBS));
    ASSERT_TRUE(p4->make_trump(Card(JACK, HEARTS), false, 2, trump));
    ASSERT_EQUAL(trump, DIAMONDS);
    delete p4;

    Player *p5 = Player_factory("V","Simple");
    p5->add_card(Card(JACK, DIAMONDS));
    p5->add_card(Card(TWO, SPADES));
    p5->add_card(Card(THREE, CLUBS));
    p5->add_card(Card(FOUR, SPADES));
    p5->add_card(Card(FIVE, CLUBS));
    ASSERT_TRUE(p5->make_trump(Card(TWO, DIAMONDS), false, 2, trump));
    ASSERT_EQUAL(trump, HEARTS);
    delete p5;

    Player *p6 = Player_factory("V","Simple");
    ASSERT_TRUE(p6->make_trump(Card(JACK, HEARTS), true, 2, trump));
    ASSERT_EQUAL(trump, DIAMONDS);
    delete p6;

    Player *p7 = Player_factory("","Simple");
    ASSERT_FALSE(p7->make_trump(Card(JACK, HEARTS), false, 2, trump));
    delete p7;
}

TEST(TestMakeTrumpSimpleOne) {
    Player* p = Player_factory("Vishal", "Simple");
    Card upcard(JACK, DIAMONDS);
    Suit upcardSuit = SPADES;

    p->add_card(Card(NINE, DIAMONDS));
    p->add_card(Card(TEN, DIAMONDS));
    p->add_card(Card(QUEEN, DIAMONDS));
    p->add_card(Card(KING, DIAMONDS));
    p->add_card(Card(ACE, DIAMONDS));

    bool ordered = p->make_trump(upcard, false, 1, upcardSuit);

    ASSERT_TRUE(ordered);
    ASSERT_EQUAL(upcardSuit, DIAMONDS);
    delete p;

    Player* p2 = Player_factory("Vishal", "Simple");

    p2->add_card(Card(NINE, CLUBS));
    p2->add_card(Card(TEN, CLUBS));
    p2->add_card(Card(QUEEN, CLUBS));
    p2->add_card(Card(KING, CLUBS));
    p2->add_card(Card(ACE, DIAMONDS));

    ordered = p2->make_trump(upcard, false, 1, upcardSuit);

    ASSERT_FALSE(ordered);
    delete p2;

    Player* p3 = Player_factory("Vishal", "Simple");

    p3->add_card(Card(NINE, CLUBS));
    p3->add_card(Card(TEN, CLUBS));
    p3->add_card(Card(QUEEN, DIAMONDS));
    p3->add_card(Card(KING, CLUBS));
    p3->add_card(Card(ACE, DIAMONDS));

    ordered = p3->make_trump(upcard, false, 1, upcardSuit);

    ASSERT_TRUE(ordered);
    delete p3;
}

TEST(TestAddAndDiscardSimple) {
    Player* p = Player_factory("67", "Simple");
    p->add_card(Card(NINE, CLUBS));
    p->add_card(Card(TEN, CLUBS));
    Card upcard(JACK, CLUBS);
    Suit trump = DIAMONDS;
    p->add_and_discard(upcard);

    Card c1 = p->lead_card(CLUBS);
    Card c2 = p->lead_card(CLUBS);
    ASSERT_TRUE((c1.get_rank() == JACK && c2.get_rank() == TEN) 
    || ((c1.get_rank()) == TEN && c2.get_rank() == JACK));

    delete p;

    Player *p3 = Player_factory("", "Simple");
    upcard = Card(JACK, DIAMONDS);
    p3->add_card(Card(NINE, DIAMONDS));
    p3->add_card(Card(ACE, DIAMONDS));
    p3->add_card(Card(QUEEN, DIAMONDS));
    p3->add_card(Card(KING, DIAMONDS));
    p3->add_card(Card(TEN, DIAMONDS));

    ASSERT_EQUAL(Card(JACK, DIAMONDS), p3->lead_card(trump));
    ASSERT_EQUAL(Card(ACE, DIAMONDS), p3->lead_card(trump));
    ASSERT_EQUAL(Card(KING, DIAMONDS), p3->lead_card(trump));
    ASSERT_EQUAL(Card(QUEEN, DIAMONDS), p3->lead_card(trump));
    ASSERT_EQUAL(Card(TEN, DIAMONDS), p3->lead_card(trump));

    delete p3;

    Player *p4 = Player_factory("", "Simple");
    upcard = Card(NINE, DIAMONDS);
    p4->add_card(Card(JACK, DIAMONDS));
    p4->add_card(Card(ACE, DIAMONDS));
    p4->add_card(Card(QUEEN, DIAMONDS));
    p4->add_card(Card(KING, DIAMONDS));
    p4->add_card(Card(TEN, DIAMONDS));

    ASSERT_EQUAL(Card(JACK, DIAMONDS), p3->lead_card(trump));
    ASSERT_EQUAL(Card(ACE, DIAMONDS), p3->lead_card(trump));
    ASSERT_EQUAL(Card(KING, DIAMONDS), p3->lead_card(trump));
    ASSERT_EQUAL(Card(QUEEN, DIAMONDS), p3->lead_card(trump));
    ASSERT_EQUAL(Card(TEN, DIAMONDS), p3->lead_card(trump));

    delete p4;

}

TEST(TestRemovesLowest){
    Player* p = Player_factory("C", "Simple");

    p->add_card(Card(NINE, CLUBS));
    p->add_card(Card(TEN, CLUBS));
    p->add_card(Card(QUEEN, CLUBS));
    p->add_card(Card(KING, CLUBS));
    p->add_card(Card(ACE, CLUBS));

    Card upcard(JACK, CLUBS);
    p->add_and_discard(upcard);

    Card led = p->lead_card(HEARTS);
    ASSERT_EQUAL(ACE, led.get_rank());

    delete p;
}

TEST(TestGetNameSimple) {
    Player* p = Player_factory("BigDon", "Simple");
    Player* p1 = Player_factory("Don", "Human");
    ASSERT_EQUAL(p->get_name(), "BigDon");
    ASSERT_EQUAL(p1->get_name(), "Don");
    delete p;
    delete p1;
}

TEST(TestLeadPrefersNonTrump) {
    Player* p = Player_factory("Joe", "Simple");

    p->add_card(Card(ACE, SPADES));   
    p->add_card(Card(KING, HEARTS));  
    p->add_card(Card(QUEEN, SPADES));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(NINE, SPADES));

    Card led = p->lead_card(SPADES);

    ASSERT_EQUAL(HEARTS, led.get_suit());
    ASSERT_EQUAL(KING, led.get_rank());

    delete p;
}

TEST(TestLeadTrumpOnly){
    Player *p = Player_factory("B", "Simple");
    p->add_card(Card(TEN, DIAMONDS));
    p->add_card(Card(ACE, DIAMONDS));
    p->add_card(Card(KING, DIAMONDS));
    p->add_card(Card(QUEEN, DIAMONDS));
    p->add_card(Card(JACK, DIAMONDS));

    Suit trump = DIAMONDS;

    Card led = p->lead_card(trump);
    ASSERT_EQUAL(DIAMONDS, led.get_suit());
    ASSERT_EQUAL(JACK, led.get_rank());

    delete p;

    Player *p2 = Player_factory("BB", "Simple");
    p2->add_card(Card(NINE, HEARTS));
    p2->add_card(Card(TEN, HEARTS));
    p2->add_card(Card(ACE, HEARTS));
    p2->add_card(Card(QUEEN, HEARTS));
    p2->add_card(Card(KING, HEARTS));
    led = p2->lead_card(trump);
    ASSERT_EQUAL(HEARTS, led.get_suit());
    ASSERT_EQUAL(ACE, led.get_rank());

    delete p2;

    Player *p3 = Player_factory("BBB", "Simple");
    p2->add_card(Card(JACK, HEARTS));
    p2->add_card(Card(TEN, HEARTS));
    p2->add_card(Card(ACE, HEARTS));
    p2->add_card(Card(QUEEN, HEARTS));
    p2->add_card(Card(KING, HEARTS));

    ASSERT_EQUAL(HEARTS, led.get_suit());
    ASSERT_EQUAL(ACE, led.get_rank());

    delete p3;

    Player *p4 = Player_factory("BBBB", "Simple");
    p2->add_card(Card(JACK, HEARTS));
    p2->add_card(Card(JACK, DIAMONDS));
    p2->add_card(Card(ACE, HEARTS));
    p2->add_card(Card(QUEEN, HEARTS));
    p2->add_card(Card(KING, HEARTS));

    ASSERT_EQUAL(HEARTS, led.get_suit());
    ASSERT_EQUAL(ACE, led.get_rank());

    delete p4;
}

TEST(TestPlayCardFollowsSuit) {
    Player* p = Player_factory("F", "Simple");

    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(KING, HEARTS)); 
    p->add_card(Card(ACE, CLUBS));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(QUEEN, DIAMONDS));

    Card led(KING, HEARTS);
    Card played = p->play_card(led, SPADES);

    ASSERT_EQUAL(HEARTS, played.get_suit());
    ASSERT_EQUAL(KING, played.get_rank());

    delete p;
}

TEST(TestPlayerOutputOperator) {
    Player* p = Player_factory("Ahhhhhhhhhhhh", "Simple");

    ostringstream oss;
    oss << *p;

    ASSERT_EQUAL("Ahhhhhhhhhhhh", oss.str());
    delete p;
}

TEST_MAIN()