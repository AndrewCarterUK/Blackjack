#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "blackjack.h"

typedef enum blackjack_result_t_ {
    BLACKJACK_DEALER_BUST = 2,
    BLACKJACK_WIN = 1,
    BLACKJACK_DRAW = 0,
    BLACKJACK_LOSE = -1,
    BLACKJACK_PLAYER_BUST = -2
} blackjack_result_t;

int blackjack_get_hand_score(
    blackjack_card_t *hand, int hand_length
)
{
    int aces, score, i;

    for (i = 0, score = 0, aces = 0; i < hand_length; i++) {
        switch (hand[i]) {
            case BLACKJACK_JACK:
            case BLACKJACK_QUEEN:
            case BLACKJACK_KING:
                score += 10;
                break;

            case BLACKJACK_ACE:
                score += 11;
                aces++;
                break;

            default:
                score += hand[i];
                break;
        }
    }

    while (score > 21 && aces > 0) {
        score -= 10;
        aces--;
    }

    return score;
}

blackjack_result_t blackjack_get_result(
    blackjack_card_t *hand, int hand_length,
    blackjack_card_t *dealer_hand, int dealer_hand_length
)
{
    int player_score, dealer_score;

    player_score = blackjack_get_hand_score(hand, hand_length);
    dealer_score = blackjack_get_hand_score(dealer_hand, dealer_hand_length);

    if (player_score > 21) {
        return BLACKJACK_PLAYER_BUST;
    }

    if (dealer_score > 21) {
        return BLACKJACK_DEALER_BUST;
    }

    if (player_score > dealer_score) {
        return BLACKJACK_WIN;
    }

    if (player_score < dealer_score) {
        return BLACKJACK_LOSE;
    }

    return BLACKJACK_DRAW;
}

void blackjack_simulate_dealer(
    blackjack_card_t *deck, int deck_length,
    blackjack_card_t *dealer_hand, int *dealer_hand_length
)
{
    while (blackjack_get_hand_score(dealer_hand, *dealer_hand_length) <= 16) {
        dealer_hand[*dealer_hand_length] = *deck;
        (*dealer_hand_length)++, deck++, deck_length--;
    }
}

blackjack_result_t blackjack_simulate_stand(
    blackjack_card_t *deck, int deck_length,
    blackjack_card_t *hand, int hand_length,
    blackjack_card_t dealer_card
)
{
    blackjack_card_t dealer_hand[BLACKJACK_MAX_HAND_LENGTH];
    int dealer_hand_length = 1;

    dealer_hand[0] = dealer_card;
    blackjack_simulate_dealer(deck, deck_length, dealer_hand, &dealer_hand_length);

    return blackjack_get_result(hand, hand_length, dealer_hand, dealer_hand_length);
}

blackjack_result_t blackjack_simulate_hit(
    blackjack_card_t *deck, int deck_length,
    blackjack_card_t *hand, int hand_length,
    blackjack_card_t dealer_card
)
{
    blackjack_card_t dealer_hand[BLACKJACK_MAX_HAND_LENGTH];
    int dealer_hand_length = 1;

    dealer_hand[0] = dealer_card;

    hand[hand_length] = *deck;
    hand_length++, deck++, deck_length--;

    blackjack_simulate_dealer(deck, deck_length, dealer_hand, &dealer_hand_length);

    return blackjack_get_result(hand, hand_length, dealer_hand, dealer_hand_length);
}

void blackjack_shuffle_cards(
    blackjack_card_t *cards, int length
)
{
    blackjack_card_t temp;
    int i, j, k;

    for (k = 0; k < 10; k++) {
        for (i = 0; i < length; i++) {
            j = rand() % length;

            temp = cards[i % length];
            cards[i] = cards[j];
            cards[j] = temp;
        }
    }
}

void blackjack_run_simulations(
    blackjack_card_t *deck, int deck_length,
    blackjack_card_t *hand, int hand_length,
    blackjack_card_t dealer_card,
    blackjack_simulation_results_t *results,
    int simulations
)
{
    blackjack_result_t result;

    memset(results, 0, sizeof(blackjack_simulation_results_t));

    while (simulations--) {
        blackjack_shuffle_cards(deck, deck_length);

        result = blackjack_simulate_stand(deck, deck_length, hand, hand_length, dealer_card);

        switch (result) {
            case BLACKJACK_DEALER_BUST: results->stand_dealer_bust++; // fall through
            case BLACKJACK_WIN: results->stand_wins++; break;
            case BLACKJACK_DRAW: results->stand_draws++; break;
            case BLACKJACK_PLAYER_BUST: // shouldn't happen, fall through
            case BLACKJACK_LOSE: results->stand_loses++; break;
        }

        result = blackjack_simulate_hit(deck, deck_length, hand, hand_length, dealer_card);

        switch (result) {
            case BLACKJACK_DEALER_BUST: results->hit_dealer_bust++; // fall through
            case BLACKJACK_WIN: results->hit_wins++; break;
            case BLACKJACK_DRAW: results->hit_draws++; break;
            case BLACKJACK_PLAYER_BUST: results->hit_bust++; // fall through
            case BLACKJACK_LOSE: results->hit_loses++; break;
        }
    }
}
