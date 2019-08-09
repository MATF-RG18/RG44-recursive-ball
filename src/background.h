typedef struct
{
    unsigned short type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offsetbits;
} BITMAPFILEHEADER;

typedef struct
{
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitcount;
    unsigned int compression;
    unsigned int sizeimage;
    int xpelspermeter;
    int ypelspermeter;
    unsigned int colorsused;
    unsigned int colorsimportant;
} BITMAPINFOHEADER;

typedef struct Background
{
    int width, height;
    char *pixels;
} Background;

Background *background_init(int width, int height);

void background_done(Background *background);
void background_read(Background *background, char *filename);