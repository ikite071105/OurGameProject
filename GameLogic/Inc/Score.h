#ifndef SCORE_H
#define SCORE_H

extern volatile int score;

void score_init(void);
void score_update(int value);
void score_draw(void);

#endif