#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Structure to hold player data
struct Player {
    int id;
    int purse;
    int points;
    bool isOut;

    Player(int i) : id(i), purse(400), points(0), isOut(false) {}
};

// Structure to hold a player's bet for the current round
struct Bet {
    int playerID;
    bool passed;
    int betAmount;
    int pairValue; // 20, 40, or 60 based on the pair played
};

class CardMoneyGame {
private:
    vector<Player> players;
    int totalRounds = 5;

public:
    CardMoneyGame(int numPlayers) {
        if (numPlayers < 5) numPlayers = 5;
        if (numPlayers > 7) numPlayers = 7;
        for (int i = 0; i < numPlayers; ++i) {
            players.push_back(Player(i + 1));
        }
    }

    void playRound(int roundNum) {
        cout << "\n--- ROUND " << roundNum << " ---" << endl;
        vector<Bet> currentBets;

        // 1. Collect Bets
        for (auto& p : players) {
            Bet b;
            b.playerID = p.id;
            
            cout << "Player " << p.id << " (Purse: " << p.purse << ", Points: " << p.points << ")" << endl;
            cout << "Enter 1 to Bet, 0 to PASS: ";
            int choice;
            cin >> choice;

            if (choice == 0) {
                b.passed = true;
                b.betAmount = 0;
                b.pairValue = 0;
            } else {
                b.passed = false;
                
                // Pair value selection
                cout << "Select pair played (20 for 2-5, 40 for 6-10, 60 for Face/Ace): ";
                cin >> b.pairValue;
                
                // Bet amount selection
                cout << "Enter bet amount (multiple of 20): ";
                cin >> b.betAmount;

                // Validate bet
                if (b.betAmount > p.purse) b.betAmount = p.purse;
                if (b.betAmount % 20 != 0) b.betAmount -= (b.betAmount % 20); // Force multiple of 20
            }
            currentBets.push_back(b);
            cout << "-----------------------" << endl;
        }

        // 2. Resolve Round
        int highestBet = -1;
        int lowestBet = 999999;
        int winnerID = -1;
        int loserID = -1;
        int winnerPairVal = 0;
        int loserPairVal = 0;

        for (const auto& b : currentBets) {
            if (b.passed) continue;

            if (b.betAmount > highestBet) {
                highestBet = b.betAmount;
                winnerID = b.playerID;
                winnerPairVal = b.pairValue;
            }
            if (b.betAmount < lowestBet) {
                lowestBet = b.betAmount;
                loserID = b.playerID;
                loserPairVal = b.pairValue;
            }
        }

        // 3. Apply Scoring and Penalties
        cout << "\n--- ROUND " << roundNum << " RESULTS ---" << endl;
        for (auto& p : players) {
            // Find this player's bet
            Bet myBet;
            for (const auto& b : currentBets) {
                if (b.playerID == p.id) myBet = b;
            }

            if (myBet.passed) {
                p.points -= 20;
                cout << "Player " << p.id << " passed. Points: -20" << endl;
            } else {
                if (p.id == winnerID) {
                    p.points += winnerPairVal;
                    p.purse -= highestBet; // Money bet cannot be used again by winner
                    cout << "Player " << p.id << " had the HIGHEST bet! Gained " << winnerPairVal << " points. Deducted " << highestBet << " from purse." << endl;
                } else if (p.id == loserID) {
                    p.points -= loserPairVal;
                    cout << "Player " << p.id << " had the LOWEST bet! Lost " << loserPairVal << " points." << endl;
                } else {
                    cout << "Player " << p.id << " was in the middle. No points changed." << endl;
                }
            }
            
            // Check for elimination (optional text output)
            if (p.purse <= 0) p.isOut = true; 
        }
    }

    void announceWinner() {
        cout << "\n=== GAME OVER ===" << endl;
        int maxPoints = -999999;
        int winningPlayer = -1;

        for (const auto& p : players) {
            cout << "Player " << p.id << " Final Score: " << p.points << " | Remaining Purse: " << p.purse << endl;
            if (p.points > maxPoints) {
                maxPoints = p.points;
                winningPlayer = p.id;
            }
        }

        cout << "\n*** THE WINNER IS PLAYER " << winningPlayer << " WITH " << maxPoints << " POINTS! ***" << endl;
    }

    void startGame() {
        for (int i = 1; i <= totalRounds; ++i) {
            playRound(i);
        }
        announceWinner();
    }
};

int main() {
    int numPlayers;
    cout << "Welcome to THE CARD MONEY" << endl;
    cout << "Enter number of players (5-7): ";
    cin >> numPlayers;

    CardMoneyGame game(numPlayers);
    game.startGame();

    return 0;
}
