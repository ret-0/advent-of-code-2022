// 02.c

/* --- Day 2: Rock Paper Scissors ---
The Elves begin to set up camp on the beach. To decide whose tent gets to be closest to
the snack storage, a giant Rock Paper Scissors tournament is
already in progress.

Rock Paper Scissors is a game between two players. Each game contains many
rounds; in each round, the players each simultaneously choose one of Rock,
Paper, or Scissors using a hand shape. Then, a winner for that round is
selected: Rock defeats Scissors, Scissors defeats Paper, and Paper defeats
Rock. If both players choose the same shape, the round instead ends in a
draw.

Appreciative of your help yesterday, one Elf gives you an encrypted
strategy guide (your puzzle input) that they say will be sure to help you
win. "The first column is what your opponent is going to play: A for Rock,
B for Paper, and C for Scissors. The second column--" Suddenly, the Elf is
called away to help with someone's tent.

The second column, you reason, must be what you should play in response: X
for Rock, Y for Paper, and Z for Scissors. Winning every time would be
suspicious, so the responses must have been carefully chosen.

The winner of the whole tournament is the player with the highest score.
Your total score is the sum of your scores for each round. The score for a
single round is the score for the shape you selected (1 for Rock, 2 for
Paper, and 3 for Scissors) plus the score for the outcome of the round (0
if you lost, 3 if the round was a draw, and 6 if you won).

Since you can't be sure if the Elf is trying to help you or trick you, you
should calculate the score you would get if you were to follow the strategy
guide.

For example, suppose you were given the following strategy guide:
----
A Y
B X
C Z
----

This strategy guide predicts and recommends the following:
 - In the first round, your opponent will choose Rock (A), and you should
   choose Paper (Y). This ends in a win for you with a score of 8 (2
   because you chose Paper + 6 because you won).
 - In the second round, your opponent will choose Paper (B), and you
   should choose Rock (X). This ends in a loss for you with a score of 1
   (1 + 0).
 - The third round is a draw with both players choosing Scissors, giving
   you a score of 3 + 3 = 6.

In this example, if you were to follow the strategy guide, you would get a
total score of 15 (8 + 1 + 6).

> What would your total score be if everything goes exactly according to your
  strategy guide?

--- Part Two ---

The Elf finishes helping with the tent and sneaks back over to you.
"Anyway, the second column says how the round needs to end: X means you
need to lose, Y means you need to end the round in a draw, and Z means you
need to win. Good luck!"

The total score is still calculated in the same way, but now you need to
figure out what shape to choose so the round ends as indicated. The example
above now goes like this:
 - In the first round, your opponent will choose Rock (A), and you need
   the round to end in a draw (Y), so you also choose Rock. This gives
   you a score of 1 + 3 = 4.
 - In the second round, your opponent will choose Paper (B), and you
   choose Rock so you lose (X) with a score of 1 + 0 = 1.
 - In the third round, you will defeat your opponent's Scissors with Rock
   for a score of 1 + 6 = 7.

Now that you're correctly decrypting the ultra top secret strategy guide,
you would get a total score of 12.

> Following the Elf's instructions for the second column, what would your
  total score be if everything goes exactly according to your strategy guide?

$ make 02 && ./02 < in/02
15691 12989
*/

#include <stdio.h> // fgets() printf() FILE stdin

typedef struct {
	int a;
	int b;
} solutions;

enum { _, ROCK, PAPER, SCISSORS };
#define LOSE 0
#define DRAW 3
#define WIN  6

// There is probably a better way to do this than 9 switch statements but :^).

int Decrypt(char c)
{
	switch (c) {
	case 'A': case 'X': return ROCK;
	case 'B': case 'Y': return PAPER;
	case 'C': case 'Z': return SCISSORS;
	}
	__builtin_unreachable();
}

int DecryptOutcome(char c)
{
	switch (c) {
	case 'X': return LOSE;
	case 'Y': return DRAW;
	case 'Z': return WIN;
	}
	__builtin_unreachable();
}

int CorrectMove(int opponent, int outcome)
{
	switch (outcome) {
	case LOSE:
		switch (opponent) {
		case ROCK:     return SCISSORS;
		case PAPER:    return ROCK;
		case SCISSORS: return PAPER;
		}
	case DRAW: return opponent;
	case WIN:
		switch (opponent) {
		case ROCK:     return PAPER;
		case PAPER:    return SCISSORS;
		case SCISSORS: return ROCK;
		}
	}
	__builtin_unreachable();
}

int Outcome(int opponent, int ourselves)
{
	int score = 0;
	score += ourselves;
	switch (ourselves) {
	case ROCK:
		switch (opponent) {
		case ROCK:     return score += DRAW;
		case PAPER:    return score += LOSE;
		case SCISSORS: return score += WIN;
		}
	case PAPER:
		switch (opponent) {
		case ROCK:     return score += WIN;
		case PAPER:    return score += DRAW;
		case SCISSORS: return score += LOSE;
		}
	case SCISSORS:
		switch (opponent) {
		case ROCK:     return score += LOSE;
		case PAPER:    return score += WIN;
		case SCISSORS: return score += DRAW;
		}
	}
	return score;
}

solutions Solve(FILE* input)
{
	int incorrectScore = 0;
	int correctScore = 0;

	static char line[5]; // (3 Characters) + \n + \0
	while (fgets(line, sizeof (line), input) != NULL) {
		int opponent = Decrypt(line[0]);
		incorrectScore += Outcome(opponent, Decrypt(line[2]));
		correctScore += Outcome(opponent, CorrectMove(opponent, DecryptOutcome(line[2])));
	}

	solutions r = {incorrectScore, correctScore};
	return r;
}

int main(void)
{
	solutions s = Solve(stdin);
	printf("%d %d\n", s.a, s.b);
	return 0;
}
