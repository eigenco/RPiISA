void init_vga(void) {
  // ET4000: set key
  outb(0x3bf, 3);
  outb(0x3d8, 0xa0);
  usleep(100000);

  // ET4000: set clocks
  outb(0x3c4, 0x07);
  outb(0x3c5, 0xfc);
  usleep(100000);

  // MISC register at port 3c2
  outb(0x3c2, 0x63);

  // ATC registers at port 3c0
  for(int i=0; i<0x10; i++) {
    inb(0x3da);
    outb(0x3c0, i);
    outb(0x3c0, i);
  }
  outb(0x3c0, 0x30); outb(0x3c0, 0x41);
  outb(0x3c0, 0x31); outb(0x3c0, 0x00);
  outb(0x3c0, 0x32); outb(0x3c0, 0x0f);
  outb(0x3c0, 0x33); outb(0x3c0, 0x00);
  outb(0x3c0, 0x34); outb(0x3c0, 0x00);

  // SEQ registers at port 3c4
  outb(0x3c4, 0x00); outb(0x3c5, 0x03);
  outb(0x3c4, 0x01); outb(0x3c5, 0x01);
  outb(0x3c4, 0x02); outb(0x3c5, 0x0f);
  outb(0x3c4, 0x03); outb(0x3c5, 0x00);
  outb(0x3c4, 0x04); outb(0x3c5, 0x0e);

  // GDC registers at port 3ce
  outb(0x3ce, 0x00); outb(0x3cf, 0x00);
  outb(0x3ce, 0x01); outb(0x3cf, 0x00);
  outb(0x3ce, 0x02); outb(0x3cf, 0x00);
  outb(0x3ce, 0x03); outb(0x3cf, 0x00);
  outb(0x3ce, 0x04); outb(0x3cf, 0x00);
  outb(0x3ce, 0x05); outb(0x3cf, 0x40);
  outb(0x3ce, 0x06); outb(0x3cf, 0x05);
  outb(0x3ce, 0x07); outb(0x3cf, 0x0f);
  outb(0x3ce, 0x08); outb(0x3cf, 0xff);

  // CRTC registers at port 3d4
  outb(0x3d4, 0x11); outb(0x3d5, 0x00);
  outb(0x3d4, 0x00); outb(0x3d5, 0x5f);
  outb(0x3d4, 0x01); outb(0x3d5, 0x4f);
  outb(0x3d4, 0x02); outb(0x3d5, 0x50);
  outb(0x3d4, 0x03); outb(0x3d5, 0x82);
  outb(0x3d4, 0x04); outb(0x3d5, 0x54);
  outb(0x3d4, 0x05); outb(0x3d5, 0x80);
  outb(0x3d4, 0x06); outb(0x3d5, 0xbf);
  outb(0x3d4, 0x07); outb(0x3d5, 0x1f);
  outb(0x3d4, 0x08); outb(0x3d5, 0x00);
  outb(0x3d4, 0x09); outb(0x3d5, 0x41);
  outb(0x3d4, 0x0a); outb(0x3d5, 0x00);
  outb(0x3d4, 0x0b); outb(0x3d5, 0x00);
  outb(0x3d4, 0x0c); outb(0x3d5, 0x00);
  outb(0x3d4, 0x0d); outb(0x3d5, 0x00);
  outb(0x3d4, 0x0e); outb(0x3d5, 0x00);
  outb(0x3d4, 0x0f); outb(0x3d5, 0x00);
  outb(0x3d4, 0x10); outb(0x3d5, 0x9c);
  outb(0x3d4, 0x11); outb(0x3d5, 0x8e);
  outb(0x3d4, 0x12); outb(0x3d5, 0x8f);
  outb(0x3d4, 0x13); outb(0x3d5, 0x28);
  outb(0x3d4, 0x14); outb(0x3d5, 0x40);
  outb(0x3d4, 0x15); outb(0x3d5, 0x96);
  outb(0x3d4, 0x16); outb(0x3d5, 0xb9);
  outb(0x3d4, 0x17); outb(0x3d5, 0xa3);
  outb(0x3d4, 0x18); outb(0x3d5, 0xff);

  /* Enable video */
  outb(0x3c0, 0x20);
  inb(0x3da);

  /* Set palette */
  outb(0x3c6, 0xff); // PEL mask
  outb(0x3c8, 0);    // Palette register
  for(int i=0; i<64; i++) {
    outb(0x3c9, i & 63);
    outb(0x3c9, 0);
    outb(0x3c9, 0);
  }
  for(int i=0; i<64; i++) {
    outb(0x3c9, 0);
    outb(0x3c9, i & 63);
    outb(0x3c9, 0);
  }
  for(int i=0; i<64; i++) {
    outb(0x3c9, 0);
    outb(0x3c9, 0);
    outb(0x3c9, i & 63);
  }
  for(int i=0; i<64; i++) {
    outb(0x3c9, i & 63);
    outb(0x3c9, i & 63);
    outb(0x3c9, i & 63);
  }
}