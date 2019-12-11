#include "bitmap.h"

internal void
FlipBitmapVertically(bitmap* Bitmap)
{
    uint32 *Memory       = (uint32*)Bitmap->Pixels;
    int32  Width         = Bitmap->Header->Width;
    int32  Height        = Bitmap->Header->Height;
    int32  BytesPerPixel = Bitmap->Header->BitsPerPixel/8;
    int32  Pitch         = BytesPerPixel*Width;
    
    for(int32 Top = 0, Bottom = (Height - 1)*Width;
        Top < Bottom;
        Top += Width, Bottom -= Width)
    {
        for(int Column = 0; Column < Width; Column++)
        {
            int32 Temp = Memory[Top + Column];
            Memory[Top + Column] = Memory[Bottom + Column];
            Memory[Bottom + Column] = Temp;
        }
    }
}

internal bitmap
LoadBMPFile(void* Contents)
{
    bitmap AssetBitmap;
    
    AssetBitmap.Header = (bitmap_header*)Contents;
    AssetBitmap.Pixels = (void*)((uint8*)Contents + AssetBitmap.Header->BitmapOffset);
    FlipBitmapVertically(&AssetBitmap);

    return(AssetBitmap);
}
