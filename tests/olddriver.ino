#define NUMPIXELS 10

static const uint32_t crc_table[16] = {
        0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
        0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};


uint32_t crc_update(uint32_t crc, const void *data, size_t data_len) {
    const unsigned char *d = (const unsigned char *) data;
    unsigned int tbl_idx;

    while (data_len--) {
        tbl_idx = crc ^ *d;
        crc = crc_table[tbl_idx & 0x0f] ^ (crc >> 4);
        tbl_idx = crc ^ (*d >> 4);
        crc = crc_table[tbl_idx & 0x0f] ^ (crc >> 4);
        d++;
    }
    return crc & 0xffffffff;
}

void setup() {
  Serial.begin(2000000L);
}

typedef struct fh {
  int8_t magic[4];
  uint8_t channel;
  uint8_t rectype; //setchan_ws2812 1, draw_all 2
} fh_t;

typedef struct ch {
  uint8_t numelem; // 0 disables, 3 RGB, 4 RGBW
  uint8_t redindex : 2;
  uint8_t greenindex : 2;
  uint8_t blueindex : 2;
  uint8_t whiteindex : 2;
  uint16_t pixels;
} ch_t;

#define BASE {0x18,0x06,0x01}

float c = 0;

void loop() {
  // send data
  fh_t fh {'U','P','X','L',0,1};
  ch_t ch {3,1,0,2,0,10};
  byte data[10][3];
  float r,g,b;
  for(int i = 0;i < NUMPIXELS;i++) {
    r = 0x148 * pow(sin(c + 0.314 * i) / 2 + 0.5, 2);
    g = 0x43 * pow(sin(c + 0.314 * i) / 2 + 0.5, 2);
    b = 0x197 * pow(sin(c + 0.314 * i) / 2 + 0.5, 2);
    data[i][0] = (uint8_t) r;
    data[i][1] = (uint8_t) g;
    data[i][2] = (uint8_t) b;
  }
  c += 0.1;
  uint32_t crc = 0xffffffff;
  Serial.write((uint8_t *)&fh, sizeof fh);
  crc = crc_update(crc, &fh, sizeof fh);
  Serial.write((uint8_t *)&ch, sizeof ch);
  crc = crc_update(crc, &ch, sizeof ch);
  for(int i = 0;i < NUMPIXELS;i++) {
    Serial.write(data[i], 3);
    crc = crc_update(crc, data[i], 3);
  }
  crc = crc ^ 0xffffffff;
  Serial.write((uint8_t *)&crc, 4);
  // draw
  fh.channel = 0xff;
  fh.rectype = 2;
  crc = 0xffffffff;
  Serial.write((uint8_t *)&fh, sizeof fh);
  crc = crc_update(crc, &fh, sizeof fh);
  crc = crc ^ 0xffffffff;
  Serial.write((uint8_t *)&crc, 4);
  Serial.flush();

  delay(100);
}
