// 05.c

/* --- Day 5: Supply Stacks ---

The expedition can depart as soon as the final supplies have been unloaded
from the ships. Supplies are stored in stacks of marked crates, but because
the needed supplies are buried under many other crates, the crates need to
be rearranged.

The ship has a giant cargo crane capable of moving crates between stacks.
To ensure none of the crates get crushed or fall over, the crane operator
will rearrange them in a series of carefully-planned steps. After the
crates are rearranged, the desired crates will be at the top of each stack.

The Elves don't want to interrupt the crane operator during this delicate
procedure, but they forgot to ask her which crate will end up where, and
they want to be ready to unload them as soon as possible so they can
embark.

They do, however, have a drawing of the starting stacks of crates and the
rearrangement procedure (your puzzle input). For example:
-------------------
    [D]    
[N] [C]    
[Z] [M] [P]
 1   2   3

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2
-------------------

In this example, there are three stacks of crates. Stack 1 contains two
crates: crate Z is on the bottom, and crate N is on top. Stack 2 contains
three crates; from bottom to top, they are crates M, C, and D. Finally,
stack 3 contains a single crate, P.

Then, the rearrangement procedure is given. In each step of the procedure,
a quantity of crates is moved from one stack to a different stack. In the
first step of the above rearrangement procedure, one crate is moved from
stack 2 to stack 1, resulting in this configuration:
------------
[D]        
[N] [C]    
[Z] [M] [P]
 1   2   3
------------

In the second step, three crates are moved from stack 1 to stack 3. Crates
are moved one at a time, so the first crate to be moved (D) ends up below
the second and third crates:
------------
        [Z]
        [N]
    [C] [D]
    [M] [P]
 1   2   3
------------

Then, both crates are moved from stack 2 to stack 1. Again, because crates
are moved one at a time, crate C ends up below crate M:
------------
        [Z]
        [N]
[M]     [D]
[C]     [P]
 1   2   3
------------

Finally, one crate is moved from stack 1 to stack 2:
------------
        [Z]
        [N]
        [D]
[C] [M] [P]
 1   2   3
------------

The Elves just need to know which crate will end up on top of each stack;
in this example, the top crates are C in stack 1, M in stack 2, and Z in
stack 3, so you should combine these together and give the Elves the
message CMZ.

> After the rearrangement procedure completes, what crate ends up on top of
  each stack?

--- Part Two ---

As you watch the crane operator expertly rearrange the crates, you notice
the process isn't following your prediction.

Some mud was covering the writing on the side of the crane, and you quickly
wipe it away. The crane isn't a CrateMover 9000 - it's a CrateMover 9001.

The CrateMover 9001 is notable for many new and exciting features: air
conditioning, leather seats, an extra cup holder, and the ability to pick
up and move multiple crates at once.

Again considering the example above, the crates begin in the same
configuration:
------------
    [D]    
[N] [C]    
[Z] [M] [P]
 1   2   3
------------

Moving a single crate from stack 2 to stack 1 behaves the same as before:
------------
[D]        
[N] [C]    
[Z] [M] [P]
 1   2   3
------------

However, the action of moving three crates from stack 1 to stack 3 means
that those three moved crates stay in the same order, resulting in this new
configuration:
------------
        [D]
        [N]
    [C] [Z]
    [M] [P]
 1   2   3
------------

Next, as both crates are moved from stack 2 to stack 1, they retain their
order as well:
------------
        [D]
        [N]
[C]     [Z]
[M]     [P]
 1   2   3
------------

Finally, a single crate is still moved from stack 1 to stack 2, but now
it's crate C that gets moved:
------------
        [D]
        [N]
        [Z]
[M] [C] [P]
 1   2   3
------------

In this example, the CrateMover 9001 has put the crates in a totally
different order: MCD.

Before the rearrangement process finishes, update your simulation so that
the Elves know where they should stand to be ready to unload the final
supplies.

> After the rearrangement procedure completes, what crate ends up
  on top of each stack?

$ make 05 && ./05 1 < in/05
BWNCQRMDB

$ ./05 2 < in/05
NHWZCBNBF
*/

/*
This was suprisingly annoying, especially the diagram parsing.
Doubly-linked lists are also very cumbersome.
The code here is pretty ugly I think, but hey, we don't have to
allocate or free any more memory after parsing the diagram!
*/

#include <stdlib.h> // malloc()
#include <stdio.h>  // fscanf() fgetc() printf() FILE stdin

#define CRATE_COUNT 9

typedef struct list {
	char data;
	struct list* prev;
	struct list* next;
} list;

list** crates;

void AllocateCrates(size_t count)
{
	crates = malloc(count * sizeof (list*));
	for (int i = 0; i < CRATE_COUNT; i++) crates[i] = NULL;
}

list* ConstructCrate(char data)
{
	list* c = malloc(sizeof (list));
	if (c != NULL) {
		c->prev = NULL;
		c->next = NULL;
		c->data = data;
	}
	return c;
}

list* End(list* c)
{
	if (c == NULL) return c;
	c->prev = NULL;
	while (c->next) {
		list* old = c;
		c = c->next;
		c->prev = old;
	}
	return c;
}

list* Prepend(list* l, char data)
{
	list* c = ConstructCrate(data);
	if (c != NULL) c->next = l;
	return c;
}

void ParseDiagram(FILE* input)
{
	char ch;
	int data = 0;
	int i = 0;
	size_t count = 0;
	for (;;) {
		ch = fgetc(input);
		if (ch == 'm' || ch == EOF) break;
		if (ch == '\n') { count = 0; i = 0; continue; }
		if (data) {
			crates[count] = Prepend(crates[count], ch);
			data = 0;
		}
		if (ch == '[') data = 1;

		i++;
		if (i == 4) {
			count++;
			i = 0;
		}
	}
	ungetc('m', input);
}

void Solve(FILE* input, char part)
{
	AllocateCrates(CRATE_COUNT);
	ParseDiagram(input);

	int src;
	int dst;
	int count;
	while (fscanf(input, "move %d from %d to %d\n", &count, &src, &dst) != EOF) {
		src--;
		dst--;
		if (part == '1') {
			for (int i = 0; i < count; i++) {
				list* sEnd = End(crates[src]);
				list* dEnd = End(crates[dst]);
				if (dEnd != NULL) dEnd->next = sEnd;
				else crates[dst] = sEnd;
				if (sEnd != NULL && sEnd->prev != NULL) sEnd->prev->next = NULL;
				else crates[src] = NULL;
			}
		} else {
			list* sEnd = End(crates[src]);
		  list* dEnd = End(crates[dst]);
			list* s = sEnd;
			for (int i = 1; i < count; i++) {
				if (s != NULL && s->prev != NULL) s = s->prev;
			}
			if (dEnd != NULL) dEnd->next = s;
			else crates[dst] = s;
			if (s != NULL && s->prev != NULL) s->prev->next = NULL;
			else crates[src] = NULL;
		}
	}

	for (int i = 0; i < CRATE_COUNT; i++) putchar(End(crates[i])->data);
	putchar('\n');
}

int main(int argc, char* argv[])
{
	Solve(stdin, argv[1][0]);
	return 0;
}
