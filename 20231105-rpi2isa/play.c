#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>

#define LA 23
#define IOW 25
#define IOR 11
#define MEMR 9
#define MEMW 10
#define RESET 22

#define D0 27
#define D1 18
#define D2 17
#define D3 15
#define D4 14
#define D5 4
#define D6 3
#define D7 2

#define A0 21
#define A1 20
#define A2 26
#define A3 16
#define A4 19
#define A5 13
#define A6 12
#define A7 6
#define A8 5
#define A9 1
#define A10 0
#define A11 7

void setaddr(unsigned short addr) {
  digitalWrite(A0, addr & 1); addr >>= 1;
  digitalWrite(A1, addr & 1); addr >>= 1;
  digitalWrite(A2, addr & 1); addr >>= 1;
  digitalWrite(A3, addr & 1); addr >>= 1;
  digitalWrite(A4, addr & 1); addr >>= 1;
  digitalWrite(A5, addr & 1); addr >>= 1;
  digitalWrite(A6, addr & 1); addr >>= 1;
  digitalWrite(A7, addr & 1); addr >>= 1;
  digitalWrite(A8, addr & 1); addr >>= 1;
  digitalWrite(A9, addr & 1); addr >>= 1;
  digitalWrite(A10, addr & 1); addr >>= 1;
  digitalWrite(A11, addr & 1);
}

void setdata(unsigned char data) {
  digitalWrite(D0, data & 1); data >>= 1;
  digitalWrite(D1, data & 1); data >>= 1;
  digitalWrite(D2, data & 1); data >>= 1;
  digitalWrite(D3, data & 1); data >>= 1;
  digitalWrite(D4, data & 1); data >>= 1;
  digitalWrite(D5, data & 1); data >>= 1;
  digitalWrite(D6, data & 1); data >>= 1;
  digitalWrite(D7, data & 1);
}

unsigned char getdata() {
  unsigned char data;

  data  = digitalRead(D7); data <<= 1;
  data |= digitalRead(D6); data <<= 1;
  data |= digitalRead(D5); data <<= 1;
  data |= digitalRead(D4); data <<= 1;
  data |= digitalRead(D3); data <<= 1;
  data |= digitalRead(D2); data <<= 1;
  data |= digitalRead(D1); data <<= 1;
  data |= digitalRead(D0);

  return data;
}

/*
void setinput() {
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);
}

void setoutput() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
}
*/

void outb(unsigned short addr, unsigned char data) {
  //setoutput();
  setaddr(addr);
  setdata(data);
  digitalWrite(IOW, 0);
  usleep(1);
  digitalWrite(IOW, 1);
}

void wr_adlib(unsigned char reg, unsigned char value) {
  outb(0x388, reg); usleep(3);
  outb(0x389, value), usleep(22);
}

int main() {
  int i, j, k, x, y, z, song_register, song_data, song_wait, song_offset = 0;
  unsigned char buffer[57306];
  FILE *f;

  f = fopen("lemmings.dat", "rb");
  fread(buffer, 1, 57306, f);
  fclose(f);

  wiringPiSetupGpio();

  pinMode(LA, OUTPUT);
  pinMode(IOW, OUTPUT);
  pinMode(IOR, OUTPUT);
  pinMode(MEMW, OUTPUT);
  pinMode(MEMR, OUTPUT);
  pinMode(RESET, OUTPUT);
  digitalWrite(IOW, 1);
  digitalWrite(IOR, 1);
  digitalWrite(MEMW, 1);
  digitalWrite(MEMR, 1);

  // CONFIGURE ADDRESS PINS
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A7, OUTPUT);
  pinMode(A8, OUTPUT);
  pinMode(A9, OUTPUT);
  pinMode(A10, OUTPUT);
  pinMode(A11, OUTPUT);

  // CONFIGURE DATA PINS
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  // RESET BUS
  digitalWrite(RESET, 1); usleep(100000);
  digitalWrite(RESET, 0); usleep(100000);

  // PLAY THE LEMMINGS SONG USING ADLIB
  for(;;) {
    song_register = buffer[song_offset];
    song_offset++;
    if(song_register==0) {
      song_wait  = (buffer[song_offset+1] << 8) | buffer[song_offset+0];
      song_offset += 2;
      usleep(1786*song_wait);
    } else {
      song_data = buffer[song_offset]; song_offset++;
      song_wait = buffer[song_offset]; song_offset++;
      wr_adlib(song_register, song_data);
      usleep(1786*song_wait);
    }
    if(song_offset>57300) {
      for(i=0; i<256; i++)
        wr_adlib(i, 0);
      exit(0);
    }
  }

  return 0;
}
