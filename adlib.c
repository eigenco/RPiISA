// g++ -O3 -c opl.cpp
// gcc -O0 adlib.c opl.o -lpthread -lm
// sudo modprobe snd-pcm-oss
// ./a.out
#define _GNU_SOURCE

#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/resource.h>
#include <linux/soundcard.h>

unsigned char BA[256], BB[256];
unsigned char idx = 0;

void adlib(void) {
  short buf[2*512];
  int a;

  int fd = open("/dev/dsp", O_WRONLY);
  a = 16; ioctl(fd, SOUND_PCM_WRITE_BITS, &a);
  a = 2; ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &a);
  a = 44100; ioctl(fd, SOUND_PCM_WRITE_RATE, &a);
  for(;;) {
    /* dump all the buffered commands to the adlib emulator */
    for(int i=0; i<idx; i++)
      adlib_write(BA[i], BB[i]);

    /* buffer is empty now */
    idx = 0;

    /* have the adlib emulator create the waveform */
    adlib_getsample(buf, 512);

    /* write the waveform to a file */
    write(fd, buf, sizeof(buf));
  }
}

/* buffer all incoming commands to the adlib */
void adlib_wr(unsigned char a, unsigned char b) {
  BA[idx] = a;
  BB[idx] = b;
  idx++;
}

int main() {
  unsigned int *gpio;
  unsigned char portb, portc = 0, data;
  unsigned char ai, ad;
  unsigned int i = 0;
  unsigned int porta;
  pthread_t th0;

  adlib_init(44100);

  int fdgpio = open("/dev/gpiomem", O_RDWR);
  gpio = (unsigned int *)mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fdgpio, 0);

  gpio[0] = 0;                  // pins 00-09 are input
  gpio[1] = 0;                  // pins 10-19 are input
  gpio[2] = 0;                  // pins 20-29 are input
  gpio[3] = gpio[3]&0xFFFFFFFC; // pins 30-31 are input

  pthread_create(&th0, NULL, (void*)adlib, NULL);

  for(;;) {
    porta = gpio[13];
    portb = porta&1;
    if(portb<portc) { // falling edge of ~IOW
      switch(((porta>>12)&0x3FF)) {
        case 0x388:
          ai = (porta>>4)&0xFF;
        break;
        case 0x389:
          adlib_wr(ai, (porta>>4)&0xFF);
        break;
      }
    }
    portc = portb;
  }

  return 0;
}
