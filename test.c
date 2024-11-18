#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
constexpr int SUITE_SPADES   = 0x000;
constexpr int SUITE_HEARTS   = 0x080;
constexpr int SUITE_CLUBS    = 0x100;
constexpr int SUITE_DIAMONDS = 0x180;
constexpr int RES_DRAW       = 0;
constexpr int RES_WIN_A      = 1;
constexpr int RES_WIN_B      = -1;
constexpr int RES_INVALID    = 2;

#define SPADES(X)        ((X) | SUITE_SPADES)
#define HEARTS(X)        ((X) | SUITE_HEARTS)
#define CLUBS(X)         ((X) | SUITE_CLUBS)
#define DIAMONDS(X)      ((X) | SUITE_DIAMONDS)

#endif /* __PROGTEST__ */

int getRank(int card) {
    return card & 0xFF;
}

int getSuite(int card) {
    return card & 0x180;
}

int compareRanks(const void* a, const void* b) {
    return getRank(*(int*)a) - getRank(*(int*)b);
}

int isStraightFlush(const int hand[]) {
    int suite = getSuite(hand[0]);
    for (int i = 1; i < 5; i++) {
        if (getSuite(hand[i]) != suite || getRank(hand[i]) != getRank(hand[i - 1]) + 1) {
            return 0;
        }
    }
    return 1;
}

int isFourOfAKind(const int hand[]) {
    return (getRank(hand[0]) == getRank(hand[3]) || getRank(hand[1]) == getRank(hand[4]));
}

int isFullHouse(const int hand[]) {
    return ((getRank(hand[0]) == getRank(hand[2]) && getRank(hand[3]) == getRank(hand[4])) ||
            (getRank(hand[0]) == getRank(hand[1]) && getRank(hand[2]) == getRank(hand[4])));
}

int isFlush(const int hand[]) {
    int suite = getSuite(hand[0]);
    for (int i = 1; i < 5; i++) {
        if (getSuite(hand[i]) != suite) {
            return 0;
        }
    }
    return 1;
}

int isStraight(const int hand[]) {
    for (int i = 1; i < 5; i++) {
        if (getRank(hand[i]) != getRank(hand[i - 1]) + 1) {
            return 0;
        }
    }
    return 1;
}

int isThreeOfAKind(const int hand[]) {
    return (getRank(hand[0]) == getRank(hand[2]) || getRank(hand[1]) == getRank(hand[3]) || getRank(hand[2]) == getRank(hand[4]));
}

int isTwoPair(const int hand[]) {
    return ((getRank(hand[0]) == getRank(hand[1]) && getRank(hand[2]) == getRank(hand[3])) ||
            (getRank(hand[0]) == getRank(hand[1]) && getRank(hand[3]) == getRank(hand[4])) ||
            (getRank(hand[1]) == getRank(hand[2]) && getRank(hand[3]) == getRank(hand[4])));
}

int isOnePair(const int hand[]) {
    for (int i = 0; i < 4; i++) {
        if (getRank(hand[i]) == getRank(hand[i + 1])) {
            return 1;
        }
    }
    return 0;
}

int getHandRank(const int hand[]) {
    if (isStraightFlush(hand)) return 8;
    if (isFourOfAKind(hand)) return 7;
    if (isFullHouse(hand)) return 6;
    if (isFlush(hand)) return 5;
    if (isStraight(hand)) return 4;
    if (isThreeOfAKind(hand)) return 3;
    if (isTwoPair(hand)) return 2;
    if (isOnePair(hand)) return 1;
    return 0;
}

int comparePokerHands(const int playerA[], const int playerB[]) {
    int handA[5], handB[5];
    for (int i = 0; i < 5; i++) {
        handA[i] = playerA[i];
        handB[i] = playerB[i];
    }

    qsort(handA, 5, sizeof(int), compareRanks);
    qsort(handB, 5, sizeof(int), compareRanks);

    int rankA = getHandRank(handA);
    int rankB = getHandRank(handB);

    if (rankA != rankB) {
        return rankA > rankB ? RES_WIN_A : RES_WIN_B;
    }

    for (int i = 4; i >= 0; i--) {
        if (getRank(handA[i]) != getRank(handB[i])) {
            return getRank(handA[i]) > getRank(handB[i]) ? RES_WIN_A : RES_WIN_B;
        }
    }

    return RES_DRAW;
}

#ifndef __PROGTEST__
int main ()
{
  int x0[] = { SPADES('5'), HEARTS('5'), CLUBS('5'), DIAMONDS('5'), HEARTS('X') };
  int y0[] = { SPADES('6'), SPADES('9'), SPADES('8'), SPADES('X'), SPADES('7') };
  assert ( comparePokerHands ( x0, y0 ) == RES_WIN_B );

  int x1[] = { SPADES('2'), HEARTS('2'), CLUBS('2'), SPADES('A'), DIAMONDS('2') };
  int y1[] = { CLUBS('A'), HEARTS('K'), HEARTS('A'), SPADES('K'), DIAMONDS('A') };
  assert ( comparePokerHands ( x1, y1 ) == RES_WIN_A );

  int x2[] = { CLUBS('3'), HEARTS('2'), HEARTS('3'), SPADES('2'), DIAMONDS('3') };
  int y2[] = { CLUBS('A'), CLUBS('9'), CLUBS('Q'), CLUBS('4'), CLUBS('J') };
  assert ( comparePokerHands ( x2, y2 ) == RES_WIN_A );

  int x3[] = { DIAMONDS('3'), HEARTS('7'), SPADES('5'), DIAMONDS('6'), SPADES('4') };
  int y3[] = { CLUBS('2'), CLUBS('4'), CLUBS('6'), CLUBS('3'), CLUBS('X') };
  assert ( comparePokerHands ( x3, y3 ) == RES_WIN_B );

  int x4[] = { DIAMONDS('3'), HEARTS('7'), SPADES('5'), DIAMONDS('6'), SPADES('4') };
  int y4[] = { CLUBS('2'), DIAMONDS('2'), CLUBS('6'), CLUBS('3'), HEARTS('2') };
  assert ( comparePokerHands ( x4, y4 ) == RES_WIN_A );

  int x5[] = { DIAMONDS('3'), HEARTS('7'), SPADES('3'), DIAMONDS('6'), SPADES('7') };
  int y5[] = { CLUBS('2'), DIAMONDS('2'), CLUBS('6'), CLUBS('3'), HEARTS('2') };
  assert ( comparePokerHands ( x5, y5 ) == RES_WIN_B );

  int x6[] = { DIAMONDS('3'), HEARTS('7'), SPADES('3'), DIAMONDS('6'), SPADES('7') };
  int y6[] = { CLUBS('2'), DIAMONDS('9'), CLUBS('K'), CLUBS('A'), HEARTS('2') };
  assert ( comparePokerHands ( x6, y6 ) == RES_WIN_A );

  int x7[] = { DIAMONDS('A'), HEARTS('J'), SPADES('Q'), DIAMONDS('X'), SPADES('2') };
  int y7[] = { CLUBS('2'), DIAMONDS('9'), CLUBS('K'), CLUBS('A'), HEARTS('2') };
  assert ( comparePokerHands ( x7, y7 ) == RES_WIN_B );

  int x8[] = { DIAMONDS('A'), HEARTS('J'), SPADES('Q'), DIAMONDS('X'), SPADES('2') };
  int y8[] = { CLUBS('Q'), DIAMONDS('K'), CLUBS('2'), CLUBS('A'), HEARTS('3') };
  assert ( comparePokerHands ( x8, y8 ) == RES_WIN_B );

  int x9[] = { DIAMONDS('A'), HEARTS('5'), SPADES('4'), DIAMONDS('5'), CLUBS('4') };
  int y9[] = { DIAMONDS('4'), DIAMONDS('K'), CLUBS('5'), SPADES('5'), HEARTS('4') };
  assert ( comparePokerHands ( x9, y9 ) == RES_WIN_A );

  int x10[] = { CLUBS('A'), CLUBS('2'), CLUBS('3'), CLUBS('4'), CLUBS('5') };
  int y10[] = { HEARTS('J'), CLUBS('J'), SPADES('J'), HEARTS('2'), SPADES('2') };
  assert ( comparePokerHands ( x10, y10 ) == RES_WIN_B );

  int x11[] = { CLUBS('A'), CLUBS('K'), CLUBS('Q'), CLUBS('J'), CLUBS('X') };
  int y11[] = { HEARTS('3'), CLUBS('3'), SPADES('3'), HEARTS('2'), SPADES('2') };
  assert ( comparePokerHands ( x11, y11 ) == RES_WIN_A );

  int x12[] = { CLUBS('A'), HEARTS('A'), CLUBS('Q'), HEARTS('Q'), CLUBS('J') };
  int y12[] = { SPADES('A'), DIAMONDS('A'), SPADES('Q'), DIAMONDS('Q'), SPADES('J') };
  assert ( comparePokerHands ( x12, y12 ) == RES_DRAW );

  int x13[] = { DIAMONDS('A'), HEARTS('5'), SPADES('4'), DIAMONDS('5'), CLUBS('4') };
  int y13[] = { DIAMONDS('4'), DIAMONDS('K'), CLUBS('5'), HEARTS('5'), HEARTS('4') };
  assert ( comparePokerHands ( x13, y13 ) == RES_INVALID );

  int x14[] = { DIAMONDS('A'), HEARTS('Z'), SPADES('4'), DIAMONDS('5'), CLUBS('4') };
  int y14[] = { DIAMONDS('4'), DIAMONDS('K'), CLUBS('5'), SPADES('5'), HEARTS('4') };
  assert ( comparePokerHands ( x14, y14 ) == RES_INVALID );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

