#include <iostream>
#include <istream>
#include <fstream>
#include "Pack.hpp"
#include <vector>
#include "Player.hpp"
#include "Card.hpp"


using namespace std;

class Game {
    public:
        Game(const vector<Player*> &players_in, Pack pack_in, bool doShuffle_in, int points_to_win_in) 
        :players(players_in), pack(pack_in), doShuffle(doShuffle_in), points_to_win(points_to_win_in), 
        team02_score(0), team13_score(0) {};
      
        void play() {
            int handNum = 0;
            int dealer = 0;

            while (team02_score < points_to_win && team13_score < points_to_win) {
                cout << "Hand " << handNum << endl;
                cout << *players[dealer] << " deals" << endl;

                shuffle();
                Card upcard = deal(dealer, handNum);
                cout << upcard << " turned up" << endl;

                int trumpOrderer = 0;
                Suit trumpSuit = make_trump(upcard, dealer, trumpOrderer);
                
                int tricks02 = 0;
                int tricks13 = 0;
                play_tricks(dealer, trumpSuit, trumpOrderer, tricks02, tricks13);
                score_hand(trumpOrderer, tricks02, tricks13);
                dealer = (dealer + 1) % 4;
                ++handNum;
            }

            if (team02_score >= points_to_win) {
                cout << *players[0] << " and " << *players[2] << " win!" << endl;
            } else {
                cout << *players[1] << " and " << *players[3] << " win!" << endl;
            }
        }
    
        private:
        std::vector<Player*> players;
        Pack pack;
        bool doShuffle;
        int points_to_win;
        int team02_score;
        int team13_score;

        void shuffle() {
            if (doShuffle) {
                pack.shuffle();
            }
                else {
                    pack.reset();
                }
        }

        Card deal(int dealer, int handNum) {

            for (int i = dealer + 1; i < 5 + dealer; ++i) {
                int pos = (i - dealer - 1) % 4;
                if (pos%2 == 0) {
                    players[i%4]->add_card(pack.deal_one());
                    players[i%4]->add_card(pack.deal_one());
                    players[i%4]->add_card(pack.deal_one());
                } else {
                    players[i%4]->add_card(pack.deal_one());
                    players[i%4]->add_card(pack.deal_one());
                }
            }
            
            for (int i = dealer + 1; i < 5 + dealer; ++i) {
                int pos = (i - dealer - 1) % 4;
                if (pos%2 == 0) {
                    players[i%4]->add_card(pack.deal_one());
                    players[i%4]->add_card(pack.deal_one());
                } else {
                    players[i%4]->add_card(pack.deal_one());
                    players[i%4]->add_card(pack.deal_one());
                    players[i%4]->add_card(pack.deal_one());
                }
            }
            
            Card upcard = pack.deal_one();
            return upcard;
        }

        Suit make_trump(const Card &upcard, int dealer, int &trumpOrderer) {
            Suit order_up_suit;

            for (int i = 1; i <= 4; ++i) {
                int player_ind = (dealer + i) % 4;
                bool is_dealer = (player_ind == dealer);
        
                if (players[player_ind]->make_trump(upcard, is_dealer, 1, order_up_suit)) {
                    cout << *players[player_ind] << " orders up " << order_up_suit << endl;
                    players[dealer]->add_and_discard(upcard);
                    cout << endl;
                    trumpOrderer = player_ind;
                    return order_up_suit;
                } else {
                    cout << *players[player_ind] << " passes" << endl;
                }
            }
        
            for (int i = 1; i <= 4; ++i) {
                int player_ind = (dealer + i) % 4;
                bool is_dealer = (player_ind == dealer);
        
                if (players[player_ind]->make_trump(upcard, is_dealer, 2, order_up_suit)) {
                    cout << *players[player_ind] << " orders up " << order_up_suit << endl;
                    cout << endl;
                    trumpOrderer = player_ind;
                    return order_up_suit;
                } else {
                    cout << *players[player_ind] << " passes" << endl;
                }
            }
            return order_up_suit;
        }

        void play_tricks(int dealer, Suit trump, int trumpOrderer,
                    int &tricks02, int &tricks13) {
        
            int leader = (dealer + 1) % 4;
        
            for (int trick = 0; trick < 5; ++trick) {
        
                Card led_card = players[leader]->lead_card(trump);
                cout << led_card << " led by " << *players[leader] << endl;
        
                Card winning_card = led_card;
                int winning_player = leader;
        
                for (int i = 1; i < 4; ++i) {
                    int player_ind = (leader + i) % 4;
                    Card played = players[player_ind]->play_card(led_card, trump);
                    cout << played << " played by " << *players[player_ind] << endl;
        
                    if (Card_less(winning_card, played, led_card, trump)) {
                        winning_card = played;
                        winning_player = player_ind;
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

    void score_hand(int trumpOrderer, int tricks02, int tricks13) {
    
        int trump_team = trumpOrderer % 2; // 0 = team 02, 1 = team 13
        int trump_tricks;

        if (trump_team == 0) {
            trump_tricks = tricks02;
        } 
        
        else {
            trump_tricks = tricks13;
        }

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

    string n = "noshuffle";
    string s = "shuffle";
    string h = "Human";
    string simp = "Simple";

    if (argc != 12 || (atoi(argv[3]) > 100 || atoi(argv[3]) < 1) 
    || (n.compare(argv[2]) != 0 && s.compare(argv[2]) != 0)
    || (simp.compare(argv[5]) != 0 && h.compare(argv[5]) != 0)
    || (simp.compare(argv[7]) != 0 && h.compare(argv[7]) != 0)
    || (simp.compare(argv[9]) != 0 && h.compare(argv[9]) != 0)
    || (simp.compare(argv[11]) != 0 && h.compare(argv[11]) != 0))  {
    
//    cout << "Usage: " << "euchre.exe " << argv[1] << " " << argv[2] 
//    << " " << argv[3] << " " << argv[4] << " " << argv[5] << " " << 
//    argv[6] << " " << argv[7] << " " << argv[8] << " " << argv[9] 
//    << " " << argv[10] << " " << argv[11] << " " << argv[12] << " " <<
//    endl;
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

    const bool doShuffle = (s.compare(argv[2]) == 0);
    const int points_to_win = atoi(argv[3]);
    Pack pack(file);
    vector<Player*> players;
    for (int i = 4; i <= 10; i += 2) {
        players.push_back(Player_factory(argv[i], argv[i+1]));
    }

    for (int i = 0; i < argc; ++i) {
        cout << argv[i] << " ";
    }

    cout << endl;

    Game game(players, pack, doShuffle, points_to_win);
    game.play();
    
    for (size_t i = 0; i < players.size(); ++i) {
      delete players[i];
    }
}