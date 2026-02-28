#include <IRremote.hpp>  // to work with IR remote
#include <toneAC.h>  // uses tone() without Timer2 and won't block IR 

#define PREV_BUTTON 534839040 // 0xBA45FF00
#define NEXT_BUTTON 1871773440 // 0xB946FF00
#define PLAY_PAUSE_BUTTON 1470693120 // 0xB847FF00

//#define VOL_MIN_BTN 0xBB44FF00
//#define VOL_PLUS_BTN 0xBF40FF00
//#define EQ_BTN 0xF609FF00

const int A2_ = 110;
const int B2 = 123;
const int C3 = 131;
const int E3 = 165;
const int F3 = 174;
const int A3_ = 220;
const int B3 = 247;
const int C4 = 262;
const int D4 = 294;
const int bE4 = 311;
const int E4 = 330;
const int F4 = 349;
const int Fs4 = 370;
const int G4 = 392;
const int Gs4 = 415;
const int bA4 = Gs4;
const int A4_ = 440;
const int bB4 = 466;
const int B4 = 494;
const int C5 = 523;
const int Cs5 = 554;
const int bD5 = Cs5;
const int D5 = 587;
const int Ds5 = 622;
const int bE5 = Ds5;
const int E5 = 659;
const int F5 = 698;
const int Fs5 = 740;
const int bG5 = Fs5;
const int G5 = 784;
const int Gs5 = 831;
const int bA5 = Gs5;
const int A5_ = 880;
const int bB5 = 932;
const int B5 = 988;
const int C6 = 1047;
const int bD6 = 1109;
const int D6 = 1175;
const int B6 = 1975;
const int pause = 0;

const int t32 = 50;
const int t16 = 2 * t32;
const int eighth = 2 * t16;
const int quarter = 2 * eighth;
const int half = 2 * quarter;
const int three_forth = 3 * quarter;
const int whole = 2 * half;
const int whole_and_half = 3 * half;

typedef struct {
  int frequency;
  int duration;
} Note;

const Note lyshVona[] PROGMEM = {
// lysh vona
  {A3_, half},
  {C5, eighth},
  {A4_, eighth},
  {E4, eighth},
  {F3, half},
  {B4, eighth},
  {A4_, eighth},
  {E4, eighth},

  {A3_, half},
  {E4, eighth},
  {B4, t16},
  {C5, t16},
  {A4_, eighth},
  {F3, half},
  {E4, eighth},
  {B4, eighth},
  {A4_, eighth},

  // ?? {, t32}
  {A3_, half},
  {C5, eighth},
  {A4_, eighth},
  {E4, eighth},
  {F3, half},
  {B4, eighth},
  {A4_, eighth},
  {E4, eighth},

  {E3, whole},
  {E4, eighth},
  {E5, eighth},
  {Gs4, eighth},
  {D5, eighth},
  {G4, eighth},
  {E5, quarter},

  {A3_, half},
  {C5, eighth},
  {A4_, eighth},
  {E4, eighth},
  {F3, half},
  {B4, eighth},
  {A4_, eighth},
  {E4, eighth},

  {A3_, half},
  {E4, eighth},
  {B4, t16},
  {C5, t16},
  {A4_, t16},
  {E4, t16},
  {F3, half},
  {E4, eighth},
  {B4, eighth},
  {A4_, eighth},

  {A3_, half},
  {C5, eighth},
  {A4_, eighth},
  {E4, eighth},
  {F3, half},
  {E3, eighth},
  {B4, eighth},
  {A4_, eighth},

  {E3, whole},  // if sucks, change this to quarter and add arpeggio
  {E4, eighth},
  {E5, eighth},
  {Gs4, eighth},
  {D5, eighth},
  {G4, eighth},
  {E5, eighth},

  {A3_, whole},
  {A4_, eighth},
  {C5, eighth},
  {E4, eighth},
  {F3, half},
  {E4, eighth},
  {B4, eighth},
  {A4_, eighth},

  {E3, whole},
  {E4, eighth},
  {E5, eighth},
  {Gs4, eighth},
  {D5, eighth},
  {G4, eighth},
  {E5, eighth}
};

const Note HedwigTheme[] PROGMEM = {
  {B4, quarter},

  {E5, quarter + eighth},
  {G5, eighth},
  {Fs5, quarter},

  {E5, half},
  {B5, quarter},

  {A5_, half+quarter},

  {Fs5, half+quarter},

  {E5, quarter},
  {G5, eighth},
  {Fs5, quarter},

  {bE5, half},
  {F5, quarter},

  {B4, half+quarter},

  {pause, half},
  //{B4, half},
  {B4, quarter},

  {E5, quarter+eighth},
  {G5, eighth},
  {Fs5, quarter},  // 18 740 200
// 10 ^^, 19 notes

  {E5, half},
  {B5, quarter},

  {D6, half},
  {bD6, quarter},

  {C6, half},
  {bA5, quarter},

  {C6, quarter+eighth},
  {B5, eighth},
  {bB5, quarter},

  {bB4, half},
  {G5, quarter},

  {E5, half+quarter},

  {E5, half},
  {G5, quarter},  // 33 784 200
// 17 ^^, 19+14=33 notes

  {B5, half},
  {G5, quarter},

  {B5, half},
  {G5, quarter},

  {C6, half},
  {B5, quarter},

  {bB5, half},
  {bG5, quarter},

  {G5, quarter+eighth},
  {B5, eighth},
  {bB5, quarter},

  {bB4, half},
  {B4, quarter},

  {B5, half+quarter},
  
  {B5, half},
  {G5, quarter}, // 48 784 200
  // 25 ^^, 33+16=49 notes

  {B5, half},
  {G5, quarter},

  {B5, half},
  {G5, quarter},

  {D6, half},
  {bD6, quarter},

  {C6, half},
  {bA5, quarter},

  {C6, quarter+eighth},
  {B5, eighth},
  {bB5, quarter},

  {bB4, half},
  {G5, quarter},

  {E5, half+eighth},
  
  {E5, half+eighth}, // 63 659 500
// 33 ^^ 49+15=64 notes
  // 4/4
  {pause, whole},  // 64th

  {B5, eighth},
  {B5, eighth},
  {B5, eighth},
  {B5, eighth},
  {B5, eighth},
  {B5, eighth},
  {bB5, eighth},
  {B5, eighth},

  {C6, quarter},
  {B5, eighth},
  {bB5, eighth},
  {B5, quarter},
  {G5, quarter},

  {A5_, quarter},
  {A5_, eighth},
  {bA5, eighth},
  {A5_, quarter},
  {C5, quarter},

  {B4, eighth},
  {pause, eighth},
  {E5, eighth},
  {G5, eighth},
  {bB5, half},

  {B5, eighth},
  {B5, eighth},
  {B5, eighth},
  {B5, eighth},
  {B5, eighth},
  {B5, eighth},
  {bB5, eighth},
  {B5, eighth},

  {D6, quarter},
  {C6, eighth},
  {B5, eighth},
  {C6, quarter},
  {F5, quarter},

  {C6, quarter},
  {B5, eighth},
  {bB5, eighth},
  {B5, quarter},
  {E5, quarter},

  {G5, half},
  {Ds5, quarter}  // 107 622 200
// 42^^ 64+44=108 notes
};

const Note Ksenya[] PROGMEM = {
  {F4, eighth},
  {pause, eighth},
  {C4, quarter},
  {D4, quarter},
  {C4, quarter},
  
  {C5, half},
  {C5, half},

  // 6 quarters?
  {C5, quarter},
  {bB4, quarter},
  {C5, quarter},
  {bD5, quarter},
  {C5, quarter},
  {bB4, quarter},

  {bA4, half},
  {bA4, half},

  {bA4, quarter},
  {G4, quarter},
  {bA4, quarter},
  {bB4, quarter},
  {bA4, quarter},  // 20
  {G4, quarter},

  {bB4, half},
  {bB4, half},

  {bB4, eighth},
  {pause, eighth},
  {C5, eighth},
  {bD5, eighth},
  {C5, quarter},
  {G4, eighth},
  {bA4, eighth},

  {F4, eighth},
  {pause, eighth},
  {C4, quarter},
  {D4, quarter},
  {C4, quarter},

  {C5, half},
  {C5, half},

  // 6 quarters?
  {C5, quarter},
  {bB4, quarter},
  {C5, quarter},  // 40
  {bD5, quarter},
  {C5, quarter},
  {bB4, quarter},

  {bA4, half},
  {bA4, half},

  {bA4, quarter},
  {G4, quarter},
  {bA4, quarter},
  {bB4, quarter},
  {bA4, quarter},
  {G4, quarter},

  {bB4, half},
  {bB4, half},

  {bB4, eighth},
  {pause, eighth},
  {C5, eighth},
  {bD5, eighth},
  {C5, quarter},
  {G4, eighth},
  {bA4, eighth},  // 60

  {F4, eighth},
  {pause, eighth},
  {C4, quarter},
  {D4, quarter},
  {C4, quarter},

  {F4, quarter}  //  65 349 200
};  // 66 notes

const Note badGuy[] PROGMEM = {
  {A4_, quarter+eighth},
  {A4_, eighth},
  {C5, eighth},
  {A4_, quarter},
  {A4_, eighth},

  {A4_, eighth},
  {A4_, quarter},
  {A4_, eighth},
  {C5, eighth},
  {A4_, eighth},
  {G4, quarter},

  {A4_, quarter+eighth},
  {A4_, eighth},
  {C5, eighth},
  {A4_, quarter},
  {A4_, eighth},

  {A4_, eighth},
  {A4_, quarter},
  {A4_, eighth},
  {C5, eighth},  // 20
  {A4_, eighth},
  {G4, quarter},

  {D5, quarter+eighth},
  {D5, eighth},
  {F5, eighth},
  {D5, quarter},
  {D5, eighth},

  {D5, eighth},
  {D5, quarter},
  {D5, eighth},
  {F5, eighth},
  {D5, eighth},
  {A5_, quarter},

  {E5, quarter+eighth},
  {E5, eighth},
  {Gs5, eighth},
  {E5, quarter},
  {E4, eighth},

  {E4, eighth},
  {E4, quarter},  // 40
  {pause, quarter},
  {pause, half},

  // lyrics start
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},

  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {A4_, eighth},
  {C5, t16},
  {D5, eighth+t16},

  {C5, eighth},
  {pause, eighth},  // 60
  {C5, eighth},
  {pause, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},

  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {A4_, eighth},
  {C5, t16},
  {D5, eighth+t16},

  {C5, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {pause, eighth},
  {C5, eighth},  // 80
  {pause, eighth},
  {C5, eighth},

  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {A4_, eighth},
  {E5, quarter},

  {B4, eighth},
  {pause, eighth},
  {B4, eighth},
  {pause, eighth},
  {pause, eighth},
  {B4, eighth},
  {pause, eighth},
  {B4, eighth},
  
  {pause, eighth},
  {A4_, eighth},
  {Gs4, eighth},  // 100
  {pause, eighth},
  {pause, half},
  
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},

  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {A4_, eighth},
  {C5, t16},
  {D5, eighth+t16},

  {C5, eighth},
  {pause, eighth},  // 120
  {C5, eighth},
  {pause, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},

  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {A4_, eighth},
  {C5, t16},
  {D5, eighth+t16},

  {C5, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {pause, eighth},
  {C5, eighth},  // 140
  {pause, eighth},
  {C5, eighth},

  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},
  {pause, eighth},
  {C5, eighth},
  {E5, quarter},

  {B4, eighth},
  {pause, eighth},
  {B4, eighth},
  {pause, eighth},
  {pause, eighth},
  {B4, eighth},
  {pause, eighth},
  {B4, eighth},
  
  {pause, eighth},
  {A4_, eighth},
  {G4, eighth},  // 160
  {pause, eighth},
  {pause, eighth},
  {C5, eighth},
  {C5, eighth},
  {D5, eighth},

  {D5, quarter},
  {A4_, quarter},
  {C5, eighth},
  {D5, eighth},
  {D5, eighth},
  {C5, eighth},

  {D5, quarter},
  {A4_, quarter},
  {C5, eighth},
  {D5, eighth},
  {D5, eighth},
  {C5, eighth},

  {D5, quarter},
  {A4_, quarter},
  {C5, eighth},  // 180
  {D5, eighth},
  {D5, eighth},
  {C5, eighth},

  {D5, quarter},
  {A4_, quarter},
  {pause, quarter},
  {C5, eighth},
  {D5, eighth},

  {E5, eighth},
  {G5, eighth},
  {E5, quarter},
  {D5, eighth},
  {D5, eighth},
  {D5, eighth},
  {C5, eighth},

  {E5, eighth},
  {G5, eighth},
  {E5, quarter},
  {D5, eighth},
  {D5, eighth},  // 200
  {D5, eighth},
  {C5, eighth},

  {E5, eighth},
  {G5, eighth},
  {E5, quarter},
  {D5, eighth},
  {D5, eighth},
  {D5, eighth},
  {C5, eighth},

  {E5, eighth},
  {G5, eighth},
  {E5, quarter},
  {pause, quarter},
  {D5, eighth},
  {C5, eighth},

  {B4, half},
  {A4_, half},

  {pause, half},
  {G4, quarter},
  {pause, quarter},  // 220

  {A4_, eighth},
  {E5, quarter},
  {A4_, eighth},
  {Ds5, eighth},
  {E5, eighth},
  {D5, eighth},
  {C5, eighth},

  {A4_, eighth},
  {E5, quarter},
  {A4_, eighth},
  {Ds5, eighth},
  {E5, eighth},
  {D5, eighth},
  {C5, eighth},

  {A4_, eighth},
  {E5, quarter},
  {A4_, eighth},
  {Ds5, eighth},
  {E5, eighth},
  {D5, eighth},  // 240
  {C5, eighth},

  {A4_, eighth},
  {E5, quarter},
  {A4_, eighth},
  {Ds5, eighth},
  {E5, eighth},
  {D5, eighth},
  {C5, eighth},

  {D5, eighth},
  {A5_, quarter},
  {D5, eighth},
  {Gs5, eighth},
  {A5_, eighth},
  {G5, eighth},
  {F5, eighth},

  {D5, eighth},
  {A5_, quarter},
  {D5, eighth},
  {Gs5, eighth},
  {A5_, eighth},  // 260
  {G5, eighth},
  {F5, eighth},

  {E5, eighth},
  {B5, quarter},
  {E5, eighth},
  {Gs5, eighth},
  {E5, quarter},
  {D5, eighth},

  {D5, half},  // 268 587 400
  {pause, half}  //  269 
};  // 270 notes

const Note starWars[] PROGMEM = {
  {D4, eighth},
  {D4, eighth},
  {D4, eighth},

  {G4, half},
  {D5, half},

  {C5, eighth},
  {B4, eighth},
  {A4_, eighth},
  {G5, half},
  {D5, eighth},

  {C5, eighth},
  {B4, eighth},
  {A4_, eighth},
  {G5, half},
  {D5, eighth},

  {C5, eighth},
  {B4, eighth},
  {C5, eighth},
  {A4_, half},
  {D4, quarter},
  {D4, eighth},

  {G4, half},
  {D5, half},

  {C5, eighth},
  {B4, eighth},
  {A4_, eighth},
  {G5, half},
  {D5, eighth},
  
  {C5, eighth},
  {B4, eighth},
  {A4_, eighth},
  {G5, half},
  {D5, eighth},
  
  {C5, eighth},
  {B4, eighth},
  {C5, eighth},
  {A4_, half},
  {D4, quarter},
  {D4, eighth},

  {E4, quarter+eighth},
  {E4, eighth},
  {C5, eighth},
  {B4, eighth},
  {A4_, eighth},
  {G4, eighth},  //  42 392 100

  {G4, eighth},
  {A4_, eighth},
  {B4, eighth},
  {A4_, quarter},
  {E4, eighth},
  {Fs4, quarter},
  {D4, quarter},
  {D4, quarter},

  {E4, quarter+eighth},
  {E4, eighth},
  {C5, eighth},
  {B4, eighth},
  {A4_, eighth},
  {G4, eighth},

  {D5, quarter},
  {A4_, half},
  {D4, quarter},
  {D4, eighth}  // 60 294 200
};  // 43+18 = 61 notes

const Note numb[] PROGMEM = {
  {A4_, eighth},
  {C5, eighth},
  {A4_, eighth},

  {D5, quarter+eighth},
  {F5, quarter+eighth},
  {E5, quarter},
  
  {E5, half},
  {pause, eighth},
  {A4_, eighth},
  {C5, eighth},
  {A4_, eighth},

  {F5, quarter+eighth},
  {E5, quarter+eighth},
  {C5, quarter},

  {C5, half},
  {pause, eighth},
  {A4_, eighth},
  {C5, eighth},
  {A4_, eighth},

  {D5, quarter+eighth},
  {F5, quarter+eighth},
  {E5, quarter},
  
  {E5, half},
  {pause, eighth},
  {A4_, eighth},
  {C5, eighth},
  {A4_, eighth},

  {F5, quarter+eighth},
  {E5, quarter+eighth},
  {C5, quarter},

  {C5, whole},

  {pause, eighth},
  {D4, eighth},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, eighth},
  {bB4, eighth},
  {A4_, eighth},

  {A4_, eighth},
  {A4_, eighth},
  {G4, eighth},
  {F4, quarter+eighth},
  {pause, quarter},

  {A4_, eighth},
  {A4_, eighth},
  {G4, eighth},
  {F4, quarter},
  {G4, quarter},
  {C4, eighth},

  {A4_, eighth},
  {A4_, eighth},
  {G4, eighth},
  {F4, quarter},
  {G4, quarter+eighth},

  {A4_, eighth},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, eighth},
  {bB4, eighth},
  {A4_, eighth},
 
  {A4_, eighth},
  {A4_, eighth},
  {G4, eighth},
  {F4, quarter+eighth},
  {pause, eighth},
  {F4, eighth},

  {A4_, eighth},
  {A4_, eighth},
  {G4, eighth},
  {F4, quarter},
  {G4, quarter},
  {C4, eighth},

  {A4_, eighth},
  {G4, quarter},
  {F4, quarter},
  {G4, quarter},
  {G4, eighth},

  {A4_, whole},
  
  {pause, half},
  {pause, quarter},
  {A4_, eighth},
  {A4_, eighth},
  
  {D5, eighth},
  {D5, eighth},
  {C5, eighth},
  {A4_, quarter},
  {A4_, eighth},
  {A4_, eighth},
  {D5, eighth},

  {D5, eighth},
  {C5, eighth},
  {C5, eighth},
  {A4_, quarter},
  {bB4, quarter},
  {bB4, eighth},

  {C5, whole},
  
  {pause, whole},

  {D5, quarter+eighth},
  {D5, eighth},
  {D5, quarter},
  {C5, quarter},

  {D5, quarter},
  {D5, eighth},
  {D5, eighth},
  {D5, quarter},
  {E5, quarter},

  {C5, quarter+eighth},
  {D5, eighth},
  {D5, quarter},
  {C5, quarter},

  {D5, quarter},
  {D5, eighth},
  {D5, eighth},
  {D5, quarter},
  {C5, quarter},

  {D5, quarter},
  {D5, eighth},
  {D5, eighth},
  {D5, quarter},
  {C5, quarter},

  {D5, quarter},
  {D5, eighth},
  {D5, eighth},
  {D5, quarter},
  {E5, quarter},

  {C5, quarter},
  {D5, eighth},
  {D5, eighth},
  {D5, quarter},
  {C5, quarter},

  {D5, quarter},
  {D5, eighth},
  {D5, eighth},
  {D5, quarter},
  {C5, quarter},
  
  // verse
  {D5, quarter},
  {D5, eighth},
  {D5, eighth},
  {D5, eighth},
  {D5, eighth},
  {D5, eighth},
  {D5, eighth},
  
  {D5, eighth},
  {D5, eighth},
  {C5, eighth},
  {C5, quarter+eighth},
  {pause, quarter},

  {D5, eighth},
  {C5, eighth},
  {C5, eighth},
  {C5, quarter},
  {A4_, quarter},
  {A4_, eighth},

  {bB4, eighth},
  {A4_, quarter},
  {G4, quarter},
  {A4_, quarter},
  {A4_, eighth},

  {pause, eighth},
  {F4, eighth},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, eighth},
  
  {A4_, eighth},
  {A4_, eighth},
  {G4, eighth},
  {A4_, quarter+eighth},
  {pause, eighth},
  {F4, eighth},

  {A4_, eighth},
  {A4_, eighth},
  {G4, eighth},
  {F4, half},
  {bB4, eighth},

  {A4_, eighth},
  {G4, quarter},
  {F4, quarter},
  {G4, quarter},
  {A4_, eighth} 
};

const Note GoTTheme[] PROGMEM = {
  {G4, half+quarter},

  {C4, half+quarter},

  {E4, eighth},
  {F4, eighth},
  {G4, half},

  {C4, half},
  {E4, eighth},
  {F4, eighth},

  {D4, half+quarter},

  {D4, half+quarter},

  {D4, half+quarter},

  {D4, half+quarter},

  {F4, half+quarter},

  {B4, half+quarter},

  {E4, eighth},
  {D4, eighth},
  {F4, half},

  {B4, half+quarter},

  {E4, eighth},
  {D4, eighth},
  {C4, half},

  {C4, half+quarter},

  {C4, half+quarter},
 
  {C4, half+quarter},

  {G5, half+quarter},

  {C5, half+quarter},

  {bE5, eighth},
  {F5, eighth},
  {G5, half},

  {C5, half},
  {bE5, eighth},
  {F5, eighth},

  {D5, half+quarter},

  {D5, half+quarter},

  {D5, half+quarter},

  {D5, half+quarter},

  {F5, half+quarter},

  {bB4, half+quarter},

  {bE5, eighth},
  {D5, eighth},
  {F5, half},

  {bB4, half+quarter},

  {bE5, eighth},
  {D5, eighth},
  {C5, half},

  {C5, half+quarter},

  {C5, half+quarter},

  {C5, half+quarter},

  {G5, half+quarter},

  {C5, half+quarter},

  {bE5, eighth},
  {F5, eighth},
  {G5, half},

  {C5, half},
  {bE5, eighth},
  {F5, eighth},

  {D5, quarter},
  {G4, quarter},
  {bB4, eighth},
  {C5, eighth},

  {D5, quarter},
  {G4, quarter},
  {bB4, eighth},
  {C5, eighth},

  {D5, quarter},
  {G4, quarter},
  {bB4, eighth},
  {C5, eighth},

  {D5, quarter},
  {G4, quarter},
  {bB4, eighth},
  {C5, eighth},

  {F5, half+quarter},

  {bB4, half+quarter},

  {D5, quarter+eighth},
  {bE5, quarter+eighth},

  {D5, quarter+eighth},
  {bB5, quarter+eighth},

  {C5, quarter},
  {G4, quarter},
  {bA4, eighth},
  {bB4, eighth},

  // 46
  {C5, quarter},
  {G4, quarter},
  {bA4, eighth},
  {bB4, eighth},

  {C5, quarter},
  {G4, quarter},
  {bA4, eighth},
  {bB4, eighth},

  {C5, quarter},
  {G4, quarter},
  {bA4, eighth},
  {bB4, eighth},

  {C6, half+quarter},

  {C6, half+quarter},

  {B6, half+quarter},

  {B6, half+quarter},

  {C5, half+quarter},

  {C5, half+quarter},

  {G5, half+quarter},

  {G5, half+quarter},

  {bA4, half+quarter},

  {A4_, half+quarter},

  {bE5, half+quarter},

  {F5, half+quarter},

  {G5, half+quarter},

  {G5, half+quarter},

  {G5, half+quarter},

  // 64
  {G5, half+quarter},

  {C6, half+quarter},

  {C6, half+quarter},

  {B5, half+quarter},

  {B5, half+quarter},

  {C5, half+quarter},

  {C5, half+quarter},

  {G5, half+quarter},

  {G5, half+quarter},

  // 73
  {bA4, half+quarter},

  {bA4, half+quarter},

  {bE5, half+quarter},

  {D5, half+quarter},

  {C5, half+quarter},

  {C5, half+quarter},

  {C5, half+quarter},

  {C5, half+quarter}
};

const Note hero[] PROGMEM = {
  {C4, quarter},
  {E4, quarter},
  {A4_, quarter},

  {C5, whole},

  {pause, quarter},
  {C4, quarter},
  {E4, quarter},
  {A4_, quarter},

  {C5, whole},

  {pause, quarter},
  {C4, quarter},
  {E4, quarter},
  {A4_, quarter},

  {C5, whole},

  {pause, quarter},
  {C5, half+quarter},

  {pause, quarter},
  {B4, half+quarter},

  // 9
  {pause, quarter},
  {E4, eighth},
  {E4, eighth},
  {E4, quarter},
  {E4, eighth},
  {F4, eighth},

  {F4, eighth},
  {E4, quarter},
  {F4, eighth},
  {F4, quarter},
  {E4, quarter},

  {E4, quarter},
  {E4, eighth},
  {E4, eighth},
  {E4, quarter},
  {D4, eighth},
  {E4, eighth},

  {E4, quarter},
  {pause, half+quarter},

  {G4, quarter},
  {F4, eighth},
  {F4, eighth},
  {F4, eighth},
  {pause, eighth},
  {F4, eighth},
  {G4, eighth},

  // 14
  {G4, quarter},
  {F4, eighth},
  {F4, eighth},
  {F4, quarter},
  {F4, eighth},
  {E4, eighth},
  
  {E4, quarter},
  {E4, eighth},
  {E4, eighth},
  {E4, quarter},
  {D4, eighth},
  {E4, eighth},

  {E4, quarter},
  {pause, half+quarter},

  {pause, quarter},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, quarter},
  {A4_, eighth},
  {A4_, eighth},

  {A4_, eighth},
  {B4, half},
  {B4, eighth},
  {A4_, eighth},
  {A4_, eighth},
  
  {A4_, quarter},
  {G4, eighth},
  {G4, eighth},
  {G4, quarter},
  {E4, eighth},
  {G4, eighth},

  // 20
  {G4, quarter},
  {pause, half+quarter},

  {pause, quarter},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, quarter},
  {A4_, eighth},
  {A4_, eighth},
  
  {A4_, eighth},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, quarter},
  {B4, eighth},
  {C5, eighth},
  {C5, eighth},
  
  {C5, quarter},
  {B4, eighth},
  {B4, quarter},
  {B4, eighth},
  {A4_, eighth},
  {B4, eighth},

  {B4, half},
  {pause, half},

  {A4_, eighth},
  {A4_, eighth},
  {G4, eighth},
  {A4_, quarter},
  {A4_, quarter},
  {pause, eighth},

  // 26
  {pause, half},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, eighth},
  
  {A4_, eighth},
  {A4_, eighth},
  {A4_, quarter},
  {A4_, eighth},
  {G4, eighth},
  {G4, eighth},
  {G4, eighth},
  
  {G4, eighth},
  {G4, eighth},
  {E4, eighth},
  {G4, quarter},
  {A4_, eighth},
  {pause, quarter},
  
  {pause, quarter},
  {G4, eighth},
  {G4, quarter},
  {F4, eighth},
  {F4, eighth},
  
  {F4, quarter},
  {F4, eighth},
  {F4, eighth},
  {G4, quarter},
  {G4, eighth},
  {F4, eighth},

  // 31
  {F4, eighth},
  {F4, eighth},
  {F4, eighth},
  {F4, eighth},
  {F4, eighth},
  {E4, quarter},
  {E4, eighth},

  {E4, eighth},
  {E4, eighth},
  {E4, quarter},
  {D4, eighth},
  {C4, eighth},
  {D4, quarter},

  {D4, eighth},
  {pause, half},
  {C5, eighth},
  {C5, eighth},
  {B4, eighth},

  {A4_, quarter},
  {C5, quarter},
  {pause, half},

  {pause, eighth},
  {C5, eighth},
  {C5, eighth},
  {C5, eighth},
  {C5, eighth},
  {B4, eighth},
  {A4_, eighth},
  {A4_, eighth},
  
  // 36
  {A4_, quarter},
  {G4, eighth},
  {G4, eighth},
  {G4, eighth},
  {G4, eighth},
  {G4, eighth},
  {E4, eighth},
  
  {G4, quarter},
  {A4_, eighth},
  {pause, quarter+eighth},
  {G4, eighth},
  {G4, eighth},
  
  {G4, quarter},
  {F4, eighth},
  {F4, eighth},
  {F4, quarter},
  {C4, eighth},
  {C4, eighth},
  
  {G4, quarter},
  {G4, eighth},
  {F4, eighth},
  {F4, eighth},
  {F4, quarter},
  {F4, eighth},

  {F4, eighth},
  {E4, quarter},
  {E4, eighth},
  {E4, eighth},
  {E4, eighth},
  {E4, quarter},

  {D4, eighth},
  {C4, eighth},
  {D4, half},
  {D4, quarter},

  // 42
  {C4, eighth},
  {B3, eighth},
  {C4, eighth},
  {B3, t16},
  {A3_, eighth},
  {pause, t16+eighth+quarter},

  {pause, quarter},
  {E4, eighth},
  {E4, eighth},
  {E4, quarter},
  {E4, eighth},
  {F4, eighth},

  {F4, eighth},
  {E4, quarter},
  {F4, eighth},
  {F4, quarter},
  {E4, quarter},

  {E4, quarter},
  {E4, eighth},
  {E4, eighth},
  {E4, quarter},
  {D4, eighth},
  {E4, eighth},

  {E4, quarter},
  {pause, half+quarter},

  // 47
  {G4, quarter},
  {F4, eighth},
  {F4, eighth},
  {F4, eighth},
  {pause, eighth},
  {F4, eighth},
  {G4, eighth},

  {G4, quarter},
  {F4, eighth},
  {F4, eighth},
  {F4, quarter},
  {F4, eighth},
  {E4, eighth},

  {E4, quarter},
  {E4, eighth},
  {E4, eighth},
  {E4, quarter},
  {D4, eighth},
  {E4, eighth},

  {E4, quarter},
  {pause, half+quarter},

  {pause, quarter+eighth},
  {A4_, eighth},
  {A4_, quarter},
  {A4_, eighth},
  {A4_, eighth},
  
  // 52 
  {A4_, eighth},
  {B4, half},
  {B4, eighth},
  {A4_, eighth},
  {A4_, eighth},

  {A4_, quarter},
  {G4, eighth},
  {G4, eighth},
  {G4, quarter},
  {E4, eighth},
  {G4, eighth},

  {G4, quarter},
  {pause, half+quarter},

  {pause, quarter},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, quarter},
  {A4_, eighth},
  {A4_, eighth},
  
  {A4_, eighth},
  {A4_, eighth},
  {A4_, eighth},
  {A4_, quarter},
  {B4, eighth},
  {C5, eighth},
  {C5, eighth},

  {C5, quarter},
  {B4, eighth},
  {B4, quarter},
  {B4, eighth},
  {A4_, eighth},
  {B4, eighth}
};

const byte RECV_PIN = 4;
const byte leds[] = {5, 6, 7};  // green, red, yellow

byte i = 0;  // melody index
const Note* const melodies[] PROGMEM = {
  lyshVona, HedwigTheme, Ksenya, badGuy, starWars, numb, GoTTheme,
  hero};
const int melodySizes[] = { // PROGMEM = {
  sizeof(lyshVona)/sizeof(Note),
  sizeof(HedwigTheme)/sizeof(Note),
  sizeof(Ksenya)/sizeof(Note),
  sizeof(badGuy)/sizeof(Note),
  sizeof(starWars)/sizeof(Note),
  sizeof(numb)/sizeof(Note),
  sizeof(GoTTheme)/sizeof(Note),
  sizeof(hero)/sizeof(Note)
};
String titles[] = {
  "Lysh Vona - Plach Yeremii",
  "Hedwig Theme - John Williams",
  "Hutsulka Ksenia",
  "Bad Guy - Billie Eilish",
  "Star Wars Theme",
  "Numb - Linkin Park",
  "Game of Thrones Theme",
  "Holding Out For A Hero - Bonny Tyler"
};
const byte listLen = 8;

// Function to access a Note from PROGMEM
// because dynamic memory can't hold all the melodies
Note getNote(int melodyIndex, int noteIndex) {
  // Get pointer to the melody from PROGMEM
  const Note* melodyPtr = (const Note*)pgm_read_ptr(&melodies[melodyIndex]);
  // Read Note struct from PROGMEM
  Note note;
  memcpy_P(&note, &melodyPtr[noteIndex], sizeof(Note));
  return note;
}

void print_title(int i) {
  Serial.print(F("Now playing "));
  Serial.print(F(" - "));
  Serial.print(i);
  Serial.print(F(" - "));
  Serial.println(titles[i]);
}

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);  // start the receiver
  for (int j = 0; j < 3; j++) {
    pinMode(leds[j], OUTPUT);
  }
  print_title(i); // initial
}

// Tracking melody progress
int noteIndex = 0;
unsigned long noteStartTime = 0;
unsigned long currentTime;

unsigned long lastIRTime = 0;
const unsigned long debounceDelay = 300;  // ms

bool onPause = false;

void loop() {
  if (IrReceiver.decode()) {
    currentTime = millis();
    if (currentTime - lastIRTime > debounceDelay) {
      lastIRTime = currentTime;
      unsigned long res = IrReceiver.decodedIRData.decodedRawData;
      
      if (res == PREV_BUTTON) {
        i = i == 0 ? listLen - 1 : i - 1;
        Serial.print(F("PREV "));
        print_title(i);
        noteIndex = 0;
      }
      if (res == NEXT_BUTTON) {
        i = i == listLen - 1 ? 0 : i + 1;
        Serial.print(F("NEXT "));
        print_title(i);
        noteIndex = 0;
      }
      if (res == PLAY_PAUSE_BUTTON) {
        if (onPause) {
          Serial.println(F("PLAY"));
        } else {
          Serial.println(F("PAUSE"));
        }
        onPause = onPause ? false : true;
        noToneAC();
        for (int j = 0; j < 3; j++) {
          digitalWrite(leds[j], LOW);
        }
      }
      // TODO: add 'loop one song'
      // TODO: change volume (-/+)
      // TODO: lights on/off (what button? maybe EQ?)
    }
    IrReceiver.resume();  // receive the next value
  }
  
  if (onPause) {
    return;
  }

  currentTime = millis();
  Note note = getNote(i, noteIndex);

  // If we finished the previous note
  if (currentTime - noteStartTime >= note.duration) {
    // Advance to next note
    noteIndex++;
    if (noteIndex >= melodySizes[i]) {
      // go to next song
      i = i == listLen - 1 ? 0 : i + 1;
      print_title(i);
      noteIndex = 0;
    }

    // Play next note
    note = getNote(i, noteIndex);
    if (note.frequency != 0) {
      noToneAC();  // adds a little pause between notes
      toneAC(note.frequency); //, 10, 0, true);
    } else {
      noToneAC();
    }
    noteStartTime = currentTime;

    // LED reaction: cycle through LEDs
    for (int j = 0; j < 3; j++) {
      digitalWrite(leds[j], noteIndex % 3 == j ? HIGH : LOW);
    }
  }
}

// uses 13422 bytes (41%) program storage
// 3031 bytes (147%) dynamic memory
// adding PROGMEM -> 49% and 37%
