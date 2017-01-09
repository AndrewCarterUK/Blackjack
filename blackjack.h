#ifndef BLACKJACK_H_
#define BLACKJACK_H_

#define BLACKJACK_MAX_HAND_LENGTH 21

#define BLACKJACK_ACE 1
#define BLACKJACK_JACK 11
#define BLACKJACK_QUEEN 12
#define BLACKJACK_KING 13

typedef int blackjack_card_t;

typedef struct blackjack_simulation_results_t_ {
    int stick_wins, stick_loses, stick_dealer_bust, stick_draws;
    int twist_wins, twist_loses, twist_bust, twist_dealer_bust, twist_draws;
} blackjack_simulation_results_t;

void blackjack_shuffle_cards(
    blackjack_card_t *cards, int length
);

void blackjack_run_simulations(
    blackjack_card_t *deck, int deck_length,
    blackjack_card_t *hand, int hand_length,
    blackjack_card_t dealer_card,
    blackjack_simulation_results_t *results,
    int simulations
);

#endif
