#include <iostream>
#include <array>
#include <string>
#include <cassert>
#include <algorithm>
#include "Player.hpp"
#include <vector>

using namespace std;

class Simple : public Player {
public:
    Simple(const string &name_in) : name(name_in) {}

    const string & get_name() const override {
        return name;
    }

    void add_card(const Card &c) override {
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);
    }

    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, Suit &order_up_suit) const override {
        if (round == 1) {
            Suit trump = upcard.get_suit();
            int count = 0;
            for (int i = 0; i < hand.size(); ++i){
                const Card &c = hand[i];
                Suit e = c.get_suit(trump);
                bool faceOrAce = (c.get_rank() == JACK ||
                c.get_rank() == QUEEN || c.get_rank() == KING || 
                c.get_rank() == ACE);

                if (e == trump && faceOrAce) {
                    count++;
                }
                if (count >= 2) {
                    order_up_suit = trump;
                    return true;
                }
            }
            return false;
        }
        
        Suit trump = Suit_next(upcard.get_suit());
        int count = 0;

        for (size_t i = 0; i < hand.size(); ++i){
            const Card &c = hand[i];
            Suit e = c.get_suit(trump);

            bool faceOrAce = (c.get_rank() == JACK ||
                c.get_rank() == QUEEN || c.get_rank() == KING || 
                c.get_rank() == ACE);

            if (e == trump && faceOrAce) {
                count++;
            }
        }

        if(count >= 1) {
            order_up_suit = trump;
            return true;
        }

        if (is_dealer) {
            order_up_suit = trump;
            return true;
        }

        else {
            return false;
        }
    }

    void add_and_discard(const Card &upcard) override {
        hand.push_back(upcard);

        int lowest_index = 0;
        for (size_t i = 1; i < hand.size(); ++i) {
            if (Card_less(hand[i], hand[lowest_index], upcard.get_suit())) {
                lowest_index = i;
            }
        }
        hand.erase(hand.begin() + lowest_index);
    }

    Card lead_card(Suit trump) override {
    int selected_index = 0;
    bool has_non_trump = false;

    for (int i = 0; i < hand.size(); ++i) {
        if (!hand[i].is_trump(trump)) {
            has_non_trump = true;
            break;
        }
    }

    if (has_non_trump) {
        bool first_found = false;

        for (int i = 0; i < hand.size(); ++i) {
            if (!hand[i].is_trump(trump)) {
                if (!first_found) {
                    selected_index = i;
                    first_found = true;
                }
                else if (hand[selected_index].get_rank() < hand[i].get_rank()) {
                    selected_index = i;
                }
            }
        }
    }
    else {
        for (int i = 1; i < hand.size(); ++i) {
            if (Card_less(hand[selected_index], hand[i], trump)) {
                selected_index = i;
            }
        }
    }

    Card chosen = hand[selected_index];
    hand.erase(hand.begin() + selected_index);
    return chosen;
    }

    Card play_card(const Card &led_card, Suit trump) override {
    
    Suit led_suit = led_card.get_suit(trump);

    bool can_follow = false;
    for (int i = 0; i < hand.size(); ++i) {
        const Card &c = hand[i];
        if (c.get_suit(trump) == led_suit) {
            can_follow = true;
            break;
        }
    }

    int index = 0;

    if (can_follow) {
        bool first_found = false;

        for (int i = 0; i < hand.size(); ++i) {
            if (hand[i].get_suit(trump) == led_suit) {
                if (!first_found) {
                    index = i;
                    first_found = true;
                }
                else if (Card_less(hand[index], hand[i],
                                   led_card, trump)) {
                    index = i;
                }
            }
        }
    }
    else {
    
        for (int i = 1; i < hand.size(); ++i) {
            if (Card_less(hand[i], hand[index],
                          led_card, trump)) {
                index = i;
            }
        }
    }

    Card chosen = hand[index];
    hand.erase(hand.begin() + index);
    return chosen;
}

private:
    string name;
    vector<Card> hand;
};

class Human : public Player {
public:
    Human(const string &name_in) : name(name_in) {}

    const string & get_name() const override {
        return name;
    }

    void add_card(const Card &c) override {
        hand.push_back(c);
    }

    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, Suit &order_up_suit) const override {
        vector<Card> sorted = hand;
        sort(sorted.begin(), sorted.end());
        for (int i = 0; i < sorted.size(); ++i) {
            cout << "Human player " << name << "'s hand: "
                 << "[" << i << "] " << sorted[i] << endl;
        }
        cout << "Human player " << name
             << ", please enter a suit, or \"pass\":" << endl;
        string input;
        cin >> input;

        if (input != "pass") {
            order_up_suit = string_to_suit(input);
            return true;
        }

        return false;
    }

    void add_and_discard(const Card &upcard) override {
        hand.push_back(upcard);
        sort(hand.begin(), hand.end());
        print_hand();
        cout << "Human player " << name
             << ", please select a card to discard:" << endl;
        int index;
        cin >> index;
        hand.erase(hand.begin() + index);
    }

Card lead_card(Suit trump) override {
        sort(hand.begin(), hand.end());
        print_hand();
        cout << "Human player " << name
             << ", please select a card:" << endl;
        int index;
        cin >> index;
        Card chosen = hand[index];
        hand.erase(hand.begin() + index);
        return chosen;
    }

    Card play_card(const Card &led_card, Suit trump) override {
        sort(hand.begin(), hand.end());
        print_hand();
        cout << "Human player " << name
             << ", please select a card:" << endl;
        int index;
        cin >> index;
        Card chosen = hand[index];
        hand.erase(hand.begin() + index);
        return chosen;
    }

private:
    string name;
    vector<Card> hand;

    void print_hand() const {
        for (int i = 0; i < hand.size(); ++i) {
            cout << "Human player " << name << "'s hand: "
                 << "[" << i << "] " << hand[i] << endl;
        }
    }
};

Player * Player_factory(const string &name,
                         const string &strategy) {
    if (strategy == "Simple") {
        return new Simple(name);
    }
     else if (strategy == "Human") {
        return new Human(name);
    }
    else {
        assert(false);
        return nullptr;
    }
}

ostream & operator<<(ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}