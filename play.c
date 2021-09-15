#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>

#define IOW 0
#define IOR 1
#define AEN 22
#define RESET 27

#define ATA0 0x300
#define ATA1 0x302
#define ATA2 0x304
#define ATA3 0x306
#define ATA4 0x308
#define ATA5 0x30A
#define ATA6 0x30C
#define ATA7 0x30E

void setaddr(unsigned short addr) {
  digitalWrite(0x0C, addr & 1); addr >>= 1;
  digitalWrite(0x0D, addr & 1); addr >>= 1;
  digitalWrite(0x0E, addr & 1); addr >>= 1;
  digitalWrite(0x0F, addr & 1); addr >>= 1;
  digitalWrite(0x10, addr & 1); addr >>= 1;
  digitalWrite(0x11, addr & 1); addr >>= 1;
  digitalWrite(0x12, addr & 1); addr >>= 1;
  digitalWrite(0x13, addr & 1); addr >>= 1;
  digitalWrite(0x14, addr & 1); addr >>= 1;
  digitalWrite(0x15, addr & 1);
}

void setdata(unsigned char data) {
  digitalWrite(0x04, data & 1); data >>= 1;
  digitalWrite(0x05, data & 1); data >>= 1;
  digitalWrite(0x06, data & 1); data >>= 1;
  digitalWrite(0x07, data & 1); data >>= 1;
  digitalWrite(0x08, data & 1); data >>= 1;
  digitalWrite(0x09, data & 1); data >>= 1;
  digitalWrite(0x0A, data & 1); data >>= 1;
  digitalWrite(0x0B, data & 1);
}

unsigned char getdata() {
  unsigned char data;

  data  = digitalRead(0x0B); data <<= 1;
  data |= digitalRead(0x0A); data <<= 1;
  data |= digitalRead(0x09); data <<= 1;
  data |= digitalRead(0x08); data <<= 1;
  data |= digitalRead(0x07); data <<= 1;
  data |= digitalRead(0x06); data <<= 1;
  data |= digitalRead(0x05); data <<= 1;
  data |= digitalRead(0x04);

  return data;
}

void setinput() {
  unsigned char i;
  for(i=0; i<8; i++)
    pinMode(i+4, INPUT);
}

void setoutput() {
  unsigned char i;
  for(i=0; i<8; i++)
    pinMode(i+4, OUTPUT);
}

unsigned char inb(unsigned short addr) {
  unsigned char data;

  setinput();
  setaddr(addr);
  digitalWrite(IOR, 0);
  usleep(10);
  data = getdata();
  digitalWrite(IOR, 1);
  usleep(10);

  return data;
}

void outb(unsigned short addr, unsigned char data) {
  setoutput();
  setaddr(addr);
  setdata(data);
  digitalWrite(IOW, 0);
  usleep(10);
  digitalWrite(IOW, 1);
  usleep(10);
}

void wr_adlib(unsigned char reg, unsigned char value) {
  outb(0x388, reg); usleep(4);
  outb(0x389, value), usleep(23);
}

int main() {
  unsigned char buffer[57306];
  int song_register, song_data, song_offset = 0, song_wait;
  FILE *f;
  int i;

  f = fopen("le.adl", "rb");
  fread(buffer, 1, 57306, f);
  fclose(f);

  wiringPiSetupGpio();

  pinMode(IOW, OUTPUT);
  pinMode(IOR, OUTPUT);
  pinMode(AEN, OUTPUT);
  pinMode(RESET, OUTPUT);
  digitalWrite(IOW, 1);
  digitalWrite(IOR, 1);
  digitalWrite(AEN, 0);
  digitalWrite(RESET, 1); usleep(1000);
  digitalWrite(RESET, 0); usleep(1000);

  // CONFIGURE ADDRESS PINS
  for(i=0; i<0x0A; i++) pinMode(i+12, OUTPUT);
  for(i=0; i<0x0A; i++) pullUpDnControl(i+12, PUD_DOWN);

  // CONFIGURE DATA PINS
  for(i=0; i<0x08; i++) pinMode(i+4, INPUT);
  for(i=0; i<0x08; i++) pullUpDnControl(i+4, PUD_DOWN);

  // PLAY THE LEMMINGS SONG USING ADLIB
  for(;;)
  {
    song_register = buffer[song_offset];
    song_offset++;
    if(song_register==0)
    {
      song_wait  = (buffer[song_offset+1] << 8) | buffer[song_offset+0];
      song_offset += 2;
      usleep(1786*song_wait);
    }
    else
    {
      song_data = buffer[song_offset]; song_offset++;
      song_wait = buffer[song_offset]; song_offset++;
      wr_adlib(song_register, song_data);
      usleep(1786*song_wait);
    }
    if(song_offset>57300)
    {
      for(i=0; i<256; i++)
        wr_adlib(i, 0);
      exit(0);
    }
  }

  return 0;
}
