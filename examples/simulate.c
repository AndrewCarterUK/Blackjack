#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../blackjack.h"

const int simulations = 100;

const char * card_to_string(blackjack_card_t card)
{
    switch (card) {
        case BLACKJACK_ACE: return "ACE";
        case BLACKJACK_JACK: return "JACK";
        case BLACKJACK_QUEEN: return "QUEEN";
        case BLACKJACK_KING: return "KING";
    }

    return "NUMBER";
}

int main(int argc, char *argv[])
{
    blackjack_simulation_results_t results;
    blackjack_card_t deck[52], hand[3], card, dealer_card;
    int suit, position;

    srand(time(NULL));

    for (card = BLACKJACK_ACE, position = 0; card <= BLACKJACK_KING; card++) {
        for (suit = 0; suit < 4; suit++) {
            deck[position++] = card;
        }
    }

    blackjack_shuffle_cards(deck, position);

    hand[0] = deck[0];
    hand[1] = deck[1];
    dealer_card = deck[2];

    printf(
        "Player Hand:\n"
            "\tCard 1: %s (%d)\n"
            "\tCard 2: %s (%d)\n"
        "\n"
        "Dealer Hand:\n"
            "\tCard 1: %s (%d)\n"
        "\n",
        card_to_string(hand[0]), hand[0],
        card_to_string(hand[1]), hand[1],
        card_to_string(dealer_card), dealer_card
    );

    blackjack_run_simulations(&(deck[3]), 52 - 3, hand, 2, dealer_card, &results, simulations);

    printf(
        "Twist Results:\n"
            "\tWin Probability:         %d%%\n"
            "\tLose Probability:        %d%%\n"
            "\tDraw Probablity:         %d%%\n"
            "\tBust Probability:        %d%%\n"
            "\tDealer Bust Probability: %d%%\n"
        "\n"
        "Stick Results:\n"
            "\tWin Probability:         %d%%\n"
            "\tLose Probability:        %d%%\n"
            "\tDraw Probablity:         %d%%\n"
            "\tDealer Bust Probability: %d%%\n"
        "\n",
        results.twist_wins, results.twist_loses, results.twist_draws, results.twist_bust, results.twist_dealer_bust,
        results.stick_wins, results.stick_loses, results.stick_draws, results.stick_dealer_bust
    );

    return 0;
}