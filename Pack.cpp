#include <iostream>
#include <array>
#include <string>
#include <cassert>
#include "Pack.hpp"
#include "Card.hpp"

using namespace std;

// EFFECTS: Initializes the Pack to be in the following standard order:
//          the cards of the lowest suit arranged from lowest rank to
//          highest rank, followed by the cards of the next lowest suit
//          in order from lowest to highest rank, and so on. 
// NOTE: The standard order is the same as that in pack.in.
// NOTE: Do NOT use pack.in in your implementation of this function
// NOTE: The pack is initially full, with no cards dealt.
Pack::Pack() : next(0) {
    next = 0;
    int index = 0;
    for (int s = SPADES; s <= DIAMONDS; s++) {
        for (int r = NINE; r <= ACE; r++) {
            cards[index] = Card(static_cast<Rank>(r), static_cast<Suit>(s));
            index++;
        }
    }
}

// REQUIRES: pack_input contains a representation of a Pack in the
//           format required by the project specification
// MODIFIES: pack_input
// EFFECTS: Initializes Pack by reading from pack_input.
// NOTE: The pack is initially full, with no cards dealt.
Pack::Pack(istream& pack_input) {
    next = 0;
    string rank_input = "";
    string ofString = "";
    string suit_input = "";
    int index = 0;
    while (pack_input >> rank_input >> ofString >> suit_input) {
        Rank r = string_to_rank(rank_input);
        Suit s = string_to_suit(suit_input); 
        Card c = {r, s};
        cards[index] = c;
        index++;
    }
}

// REQUIRES: cards remain in the Pack
// EFFECTS: Returns the next card in the pack and increments the next index

 Card Pack::deal_one() {
    assert(next < PACK_SIZE);  
    Card c = cards[next];      
    next++;                    
    return c;                  
}

// EFFECTS: Resets next index to first card in the Pack
void Pack::reset() {
    next = 0;
}

// EFFECTS: Shuffles the Pack and resets the next index. This
//          performs an in shuffle seven times. See
//          https://en.wikipedia.org/wiki/In_shuffle.
void Pack::shuffle() {
    static constexpr int half = PACK_SIZE/2;
    Card cardsfirsthalf[half];
    Card cardssecondhalf[half];

    for (int k = 0; k < 7; k++) {
        for (int i = 0; i < half; i++) {
            cardsfirsthalf[i] = cards[i];
        }
        for (int i = 0; i < half; i++) {
            cardssecondhalf[i] = cards[i+half];
        }
        
        for (int j = 0; j < half; j++) {
            cards[j*2] = cardssecondhalf[j];
            cards[j*2 + 1] = cardsfirsthalf[j];
        }
    }
    next = 0;
}

// EFFECTS: returns true if there are no more cards left in the pack
bool Pack::empty() const {
    if (next >= PACK_SIZE) {
        return true;
    }
    return false;
}