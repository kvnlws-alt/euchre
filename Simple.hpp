#ifndef SIMPLE_HPP
#define SIMPLE_HPP

#include "Player.hpp"
#include <vector>
#include <string>

class Simple : public Player {
public:
    Simple(const std::string &name_in);

    const std::string & get_name() const override;

    void add_card(const Card &c) override;

    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, Suit &order_up_suit) const override;

    void add_and_discard(const Card &upcard) override;

    Card lead_card(Suit trump) override;

    Card play_card(const Card &led_card, Suit trump) override;

private:
    std::string name;
    std::vector<Card> hand;
};

#endif