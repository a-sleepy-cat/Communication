#ifndef ACTION_H
#define ACTION_H

#define TURNLEFT 1
#define TURNRIGHT 2

extern u8 turn_direction;

void CheckTurn(void);
void Turn(void);

void turn_left45(void);           //use delay_ms()
void turn_left45_return(void);
void turn_right45(void);
void turn_right45_return(void);
void turn_right_withdelay(void);  //use delay_ms()
void turn_left_withdelay(void);

#endif
