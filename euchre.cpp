#include <iostream>
#include <istream>
#include "Pack.hpp"
#include <vector>
#include "Player.hpp"
#include "Card.hpp"


using namespace std;

class Game {
    public:
      Game(const vector<Player*> &players_in, Pack pack_in, bool doShuffle_in, int points_to_win_in) 
        :players(players_in), pack(pack_in), doShuffle(doShuffle_in), points_to_win(points_to_win_in), team02_score(0), team13_score(0) {};
      void play() {
          int handNum = 0;
          int dealer = 0;

          while (team02_score < points_to_win && team13_score < points_to_win) {
              cout << "Hand " << handNum << endl;
              cout << player[dealer] << " deals" << endl;

              shuffle();
              Card upcard = deal(dealer);
              cout << upcard << " turned up" << endl;

              int trump_orderer = 0;
              Suit trumpSuit = make_trump(upcard, dealer, trump_orderer);
              
              int tricks02 = 0;
              int tricks13 = 0;
              play_tricks(dealer, trumpSuit, trump_orderer, tricks02, tricks13);
              score_hand(trump_orderer, tricks02, tricks13);
              dealer = (dealer + 1) % 4;
              ++handNum;
          }

          if (team 02_score <= points_to_win) {
              cout << *players[0] << " and " << *players[2] << " win the hand" << endl;
          } else {
              cout << *players[1] << " and " << *players[3] << " win the hand" << endl;
          }
      }
    
    private:
      std::vector<Player*> players;
      Pack pack;
      bool doShuffle;
    
      void shuffle() {
          if (doShuffle) {
              pack.shuffle()
            else {
                pack.reset();
            }
          }
      }
      Card deal(int dealer) {
        player[0] = (dealer + 1) % 4;  
        player[1] = (dealer + 2) % 4;
        player[2] = (dealer + 3) % 4;
        player[3] = (dealer + 4) % 4;  

        for (size_t i = 0; i < 4; ++i) {
            if (i%2 = 0) {
                players[i]->add_card(pack.deal_one());
                players[i]->add_card(pack.deal_one());
                players[i]->add_card(pack.deal_one());
            } else {
                players[i]->add_card(pack.deal_one());
                players[i]->add_card(pack.deal_one());
            }
        }
          
        for (size_t i = 0; i < 4; ++i) {
            if (i%2 = 0) {
                players[i]->add_card(pack.deal_one());
                players[i]->add_card(pack.deal_one());
            } else {
                players[i]->add_card(pack.deal_one());
                players[i]->add_card(pack.deal_one());
                players[i]->add_card(pack.deal_one());
            }
        }
          
        Card upcard = pack.deal_one();
        return upcard;
     }
      Suit make_trump(const Card &upcard, int dealer, int &trump_orderer) {
        Suit order_up_suit;

        for (int i = 1; i <= 4; ++i) {
            int player_idx = (dealer + i) % 4;
            bool is_dealer = (player_idx == dealer);
    
            if (players[player_idx]->make_trump(upcard, is_dealer, 1, order_up_suit)) {
                cout << *players[player_idx] << " orders up " << order_up_suit << endl;
                players[dealer]->add_and_discard(upcard);
                cout << endl;
                trump_orderer = player_idx;
                return order_up_suit;
            } else {
                cout << *players[player_idx] << " passes" << endl;
            }
        }
    
        for (int i = 1; i <= 4; ++i) {
            int player_idx = (dealer + i) % 4;
            bool is_dealer = (player_idx == dealer);
    
            if (players[player_idx]->make_trump(upcard, is_dealer, 2, order_up_suit)) {
                cout << *players[player_idx] << " orders up " << order_up_suit << endl;
                cout << endl;
                trump_orderer = player_idx;
                return order_up_suit;
            } else {
                cout << *players[player_idx] << " passes" << endl;
            }
        }
     }
      void play_tricks(int dealer, Suit trump, int trump_orderer,
                 int &tricks02, int &tricks13) {
    
        int leader = (dealer + 1) % 4;
    
        for (int trick = 0; trick < 5; ++trick) {
    
            Card led_card = players[leader]->lead_card(trump);
            cout << led_card << " led by " << *players[leader] << endl;
    
            Card winning_card = led_card;
            int winning_player = leader;
    
            for (int i = 1; i < 4; ++i) {
                int player_idx = (leader + i) % 4;
                Card played = players[player_idx]->play_card(led_card, trump);
                cout << played << " played by " << *players[player_idx] << endl;
    
                if (Card_less(winning_card, played, led_card, trump)) {
                    winning_card = played;
                    winning_player = player_idx;
                }
            }
    
            cout << *players[winning_player] << " takes the trick" << endl;
            cout << endl;
    
            if (winning_player == 0 || winning_player == 2) {
                tricks02++;
            } else {
                tricks13++;
            }
    
            leader = winning_player;
        }
    }

    void score_hand(int trump_orderer, int tricks02, int tricks13) {
    
        int trump_team = trump_orderer % 2; // 0 = team 0&2, 1 = team 1&3
        int trump_tricks   = (trump_team == 0) ? tricks02 : tricks13;
        int defender_tricks = (trump_team == 0) ? tricks13 : tricks02;
    
        if (trump_tricks >= 3) {
            if (trump_team == 0) {
                cout << *players[0] << " and " << *players[2] << " win the hand" << endl;
            } else {
                cout << *players[1] << " and " << *players[3] << " win the hand" << endl;
            }

            if (trump_tricks == 5) {
                cout << "march!" << endl;
                if (trump_team == 0) { 
                    team02_score += 2;
                } else {                 
                    team13_score += 2;
                }
            } else {
                if (trump_team == 0) {
                    team02_score += 1;
                } else {
                    team13_score += 1;
                }
            }
        } else {
            int defender_team = 1 - trump_team;
    
            if (defender_team == 0) {
                cout << *players[0] << " and " << *players[2] << " win the hand" << endl;
            } else {
                cout << *players[1] << " and " << *players[3] << " win the hand" << endl;
            }
    
            cout << "euchred!" << endl;
    
            if (defender_team == 0) {
                team02_score += 2;
            } else {
                team13_score += 2;
            }
        }
    
        cout << *players[0] << " and " << *players[2] << " have " << team02_score << " points" << endl;
        cout << *players[1] << " and " << *players[3] << " have " << team13_score << " points" << endl;
        cout << endl;
    }
};

int main(int argc, char *argv[]) {

    if (argc != 12 || (atoi(argv[3]) > 100 || atoi(argv[3]) < 1) 
    || ((argv[2] != "noshuffle") || (argv[2] != "shuffle")) || 
    ((argv[5] != "Simple" || argv[7] != "Simple" || argv[9] != "Simple" 
    || argv[11] != "Simple" || argv[5] != "Human" || argv[7] != "Human" 
    || argv[9] != "Human" || argv[11] != "Human"))) {
    
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
    
     return 67;
    }
    const string pack_filename = argv[1];
    ifstream file(pack_filename);
    if (!file.is_open()) {
     cout << "Error opening " << pack_filename << endl;
    }

    const bool doShuffle = (string(argv[2] == "shuffle");
    const int points_to_win = atoi(argv[3]);
    Pack pack(pack_filename);
    vector<Player*> players;
    for (int i = 4; i < 10; i += 2) {
        players.push_back(Player_factory(argv[i], argv[i+1]));
    }

    for (int i = 0; i < argc; ++i) {
        cout << argc[i] << endl;
    }

    Game game(players, pack, doShuffle, points_to_win);
    game.play();
    
    for (size_t i = 0; i < players.size(); ++i) {
      delete players[i];
    }
}