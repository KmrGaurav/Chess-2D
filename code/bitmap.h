#if !defined(BITMAP_H)

#pragma pack(push, 1)
struct bitmap_header
{
    uint16 FileType;
    uint32 FileSize;
    uint16 Reserved1;
    uint16 Reserved2;
    uint32 BitmapOffset;
    uint32 Size;
     int32 Width;
     int32 Height;
    uint16 Planes;
    uint16 BitsPerPixel;
    uint32 Compression;
    uint32 SizeOfBitmap;
     int32 HorzResolution;
     int32 VertResolution;
    uint32 ColorsUsed;
    uint32 ColorsImportant;
    
    uint32 RedMask;
    uint32 GreenMask;
    uint32 BlueMask;
};
#pragma pack(pop)

struct bitmap
{
    bitmap_header *Header;
    void* Pixels;
};

#define BITMAP_H
#endif
