#if !defined(WIN32_CHESS_H)

struct win32_offscreen_buffer
{
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
};

#define WIN32_CHESS_H
#endif
