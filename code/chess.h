#if !defined(CHESS_H)

inline uint32
SafeTruncateUInt64(uint64 Value)
{
    uint32 Result = (uint32)Value;
    return(Result);
}

struct read_file_result
{
    uint32 ContentsSize;
    void *Contents;
};
internal read_file_result ReadEntireFile(char *Filename);
internal void* AllocateMemory(uint32 Size);
internal void FreeMemory(void *Memory);
internal bool32 WriteEntireFile(char *Filename, uint32 MemorySize, void *Memory);
internal void CopyMemoryBlock(void* Memory1, void* Memory2, uint32 Length);
struct game_offscreen_buffer
{
    void *Memory;
    int Width;
    int Height;
    int Pitch;
};

struct game_asset
{
    uint32 FileSize;
    uint32 BitmapOffset;
    uint32 Size;
    int32 Width;
    int32 Height;
    uint16 BitsPerPixel;
    uint32 SizeOfBitmap;
    void* Memory;
};

enum rank
{
    NoRank,
    King,
    Queen,
    Bishop,
    Knight,
    Rook,
    Pawn
};

struct player_asset
{
    int32 Width;
    int32 Height;
    int32 BitsPerPixel;
    int32 Pitch;
    int32 BitmapSize;
    void* Pixels;
};

struct player_asset_set
{
    union
    {
        player_asset PlayerAsset[6];
        struct
        {
            player_asset King;
            player_asset Queen;
            player_asset Bishop;
            player_asset Knight;
            player_asset Rook;
            player_asset Pawn;
        };
    };
};

struct both_players_asset
{
    player_asset_set PlayerAssetSet[2];
};

enum player
{
    NoPlayer,
    Player1,
    Player2
};

struct player_info
{
    int32  Index;
    int32  X;
    int32  Y;
    rank   Rank;
    bool32 Highlight;
    player_asset* PlayerAsset;
};

struct position
{
    int32  X;
    int32  Y;
    player Player;
    bool32 IsBeingTargeted;
    player_info *PlayerInfo;
};

struct point
{
    int32 X;
    int32 Y;
};

struct game_input
{
    point  MousePointer;
    bool32 IsMousesLeftButtonDown;
};

struct game_state
{
    int32 NewIndex;
    int32 OldIndex;
    
    player Turn;
    
    player_info *DefeatedPlayer1Info[16];
    int32 DefeatedPlayer1sCount;
    
    player_info *DefeatedPlayer2Info[16];
    int32 DefeatedPlayer2sCount;
};

#define CHESS_H
#endif
