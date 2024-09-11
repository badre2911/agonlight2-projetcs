/*
 Title:		    cpmkey.c
 Author:	    Badre
 Created:	    28/07/2024 
 Last Updated:  11/09/2024

 Modinfo:
 11/09/2024     Added key Up, Down, Left and Right
 */



#include "agconio.h"
#include "ckey.h"

#define TIMEOUT 1000
#define END 0

typedef enum ctrlkey
{
  CTRL_A = 65,
  CTRL_B,
  CTRL_C,
  CTRL_D,
  CTRL_E,
  CTRL_F,
  CTRL_G,
  CTRL_H,
  CTRL_I,
  CTRL_J,
  CTRL_K,
  CTRL_L,
  CTRL_M,
  CTRL_N,
  CTRL_O,
  CTRL_P,
  CTRL_Q,
  CTRL_R,
  CTRL_S,
  CTRL_T,
  CTRL_U,
  CTRL_V,
  CTRL_W,
  CTRL_X,
  CTRL_Y,
  CTRL_Z
}  CtrlKey;

static unsigned char t_getch(int timeout) {
  char ch = END;
  int t = timeout;
  while (t && !_kbhit()) {
    t--;
  }
  if (_kbhit()) {
    ch = _readKbd();
  }
  return ch;
}

int cpmReadKey(void)
{
    unsigned char i, c;
    unsigned char c1[10];

    while (1) {
        i = 0;
        c1[i++] = _getch();
        do {
            c = t_getch(TIMEOUT);
            c1[i++] = c;
        } while (c != END);
        
        if (c1[0] == 0) {
            return 0;
        } else if (c1[0] == 27 && c1[1] == 0) {
            return ESC_KEY;                 
        } else if(c1[0] == 27 && c1[1] == 79 && c1[2] == 65 && c1[3] == 0) {
            return ARROW_UP;
        } else if(c1[0] == 27 && c1[1] == 79 && c1[2] == 66 && c1[3] == 0) {
            return ARROW_DOWN;
        } else if(c1[0] == 27 && c1[1] == 79 && c1[2] == 68 && c1[3] == 0) {
            return ARROW_LEFT;
        } else if(c1[0] == 27 && c1[1] == 79 && c1[2] == 67 && c1[3] == 0) {
            return ARROW_RIGHT;  
        } else if (c1[0] == 13) {
            return ENTER_KEY;        
        } else if (c1[0] == 9) {
            return TAB_KEY;
        } else if(c1[0] == 8) {
            return BACKSPACE;
        } else if (c1[0] == 32) {
            return SPACE_KEY;
        } else if (c1[0] < 32) {
            switch(c1[0]+64) {
                case CTRL_A: return HOME_KEY;
                case CTRL_C: return CTRL_C_KEY;
                case CTRL_D: return ARROW_RIGHT;
                case CTRL_E: return ARROW_UP;
                case CTRL_F: return END_KEY;
                case CTRL_G: return BACKSPACE;
                case CTRL_I: return TAB_KEY;
                case CTRL_S: return ARROW_LEFT;
                case CTRL_W: return PAGE_UP;
                case CTRL_X: return ARROW_DOWN;
                case CTRL_Y: return DELETE_LINE;
                case CTRL_Z: return PAGE_DOWN;
                default: return 0;
            }
        } else if (c1[0] > 127) {
            return 0;
        } else {
            break;
        }
        /*for (i=0; c1[i]!=END; i++) {
            describe(c1[i]);
        }
        printf("\n");
        if(c1[0] == 3) {
        printf("Exiting...\n");
        break;
        }*/
    }
    return c1[0];
}