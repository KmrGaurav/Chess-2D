#include "bitmap.cpp"
#include <stdio.h>
#include <windows.h>
#include "chess.h"
#include "moves.cpp"

internal void
ExtractBothPlayersAssets(bitmap* CompleteAsset, both_players_asset* BothPlayersAsset)
{
    for(int32 Index = 0;
        Index < 6;
        Index++)
    {
        player_asset *PlayerAsset = &BothPlayersAsset->PlayerAssetSet[0].PlayerAsset[Index];
        
        PlayerAsset->Width        = CompleteAsset->Header->Width/6;
        PlayerAsset->Height       = CompleteAsset->Header->Height/2;
        PlayerAsset->BitsPerPixel = CompleteAsset->Header->BitsPerPixel;
        PlayerAsset->Pitch        = PlayerAsset->Width*PlayerAsset->BitsPerPixel/8;
        PlayerAsset->BitmapSize   = PlayerAsset->Width*PlayerAsset->Height*PlayerAsset->BitsPerPixel/8;
        PlayerAsset->Pixels       = (void*)AllocateMemory(PlayerAsset->BitmapSize);
        
        uint32* Src = (uint32*)CompleteAsset->Pixels + 80*(5-Index);
        uint32* Des = (uint32*)  PlayerAsset->Pixels;
        for(int32 Y = 0;
            Y < PlayerAsset->Height;
            ++Y)
        {
            uint32*  In = (uint32*)Src;
            uint32* Out = (uint32*)Des;
            for(int32 X = 0;
                X < PlayerAsset->Width;
                ++X)
            {
                *Out++ = *In++;
            }
            Src += CompleteAsset->Header->Width;
            Des += PlayerAsset->Width;
        }
    }
    
    for(int32 Index = 0;
        Index < 6;
        Index++)
    {
        player_asset *PlayerAsset = &BothPlayersAsset->PlayerAssetSet[1].PlayerAsset[Index];
        
        PlayerAsset->Width        = CompleteAsset->Header->Width/6;
        PlayerAsset->Height       = CompleteAsset->Header->Height/2;
        PlayerAsset->BitsPerPixel = CompleteAsset->Header->BitsPerPixel;
        PlayerAsset->Pitch        = PlayerAsset->Width*PlayerAsset->BitsPerPixel/8;
        PlayerAsset->BitmapSize   = PlayerAsset->Width*PlayerAsset->Height*PlayerAsset->BitsPerPixel/8;
        PlayerAsset->Pixels       = (void*)AllocateMemory(PlayerAsset->BitmapSize);
        
        uint32* Src = (uint32*)CompleteAsset->Pixels + 80*Index + 80*480;
        uint32* Des = (uint32*)  PlayerAsset->Pixels;
        for(int32 Y = 0;
            Y < PlayerAsset->Height;
            ++Y)
        {
            uint32*  In = (uint32*)Src;
            uint32* Out = (uint32*)Des;
            for(int32 X = 0;
                X < PlayerAsset->Width;
                ++X)
            {
                *Out++ = *In++;
            }
            Src += CompleteAsset->Header->Width;
            Des += PlayerAsset->Width;
        }
    }
}

internal void
Highlighter(game_offscreen_buffer* Buffer, int32 Index, uint32 Color)
{
    uint32* Pixels = (uint32*)Buffer->Memory + 320 + 80*(Index%8) + 80*Buffer->Width*(Index/8);
    for(int32 Y = 0; 
        Y < 80 && Index < 64; 
        ++Y)
    {
        uint32* Row = (uint32*)Pixels;
        for(int32 X = 0; X < 80; ++X)
        {
            *Row++ = Color;
        }
        Pixels += Buffer->Width;
    }
}

internal void
RenderGameBoard(game_offscreen_buffer *Buffer, int32 HighlightedBlockIndex)
{
    int MarginLeft   = 320;
    int MarginRight  = 960;
    int MarginTop    = 0;
    int MarginBottom = 640;
    
    uint8 *Row = (uint8*)Buffer->Memory;
    for(int32 Y = 0; Y < Buffer->Height; ++Y)
    {
        uint32 *Pixel = (uint32 *)Row;
        for(int32 X = 0; X < Buffer->Width; ++X)
        {
            //uint32 Color1 = 0x00DCDCDC; //Gainsboro
            //uint32 Color1 = 0x00FFFFE0; //Light Yellow
            //uint32 Color1 = 0x00E5E4E2; //Platinum
            uint32 Color1 = 0x00FCE883; //Clover-Lime (Crayola)
            //uint32 Color1 = 0x00FFFACD; //Lemon Chiffon
            //uint32 Color2 = 0x00367588;
            uint32 Color2 = 0x00826644; //Raw Umber
            
            if((MarginLeft <= X && X < MarginRight)&&
               (MarginTop  <= Y && Y < MarginBottom))
            {
                if((Y/80)%2)
                {
                    *Pixel = (X/80)%2 ? Color1 : Color2;
                }
                else
                {
                    *Pixel = (X/80)%2 ? Color2 : Color1;
                }
            }
            else
            {
                *Pixel = 0x004C5866;
            }
            *Pixel++;
        } // X
        Row += Buffer->Pitch;
    } // Y
    
    //Here block is being highlighted on which mouse is hover
    Highlighter(Buffer, HighlightedBlockIndex, 0x00BFA763);
}

internal rank
InitializeRank(int32 Index)
{
    if(Index == 0 || Index == 7 || Index == 56 || Index == 63)
    {
        return(Rook);
    }
    else if(Index == 1 || Index == 6 || Index == 57 || Index == 62)
    {
        return(Knight);
    }
    else if(Index == 2 || Index == 5 || Index == 58 || Index == 61)
    {
        return(Bishop);
    }
    else if(Index == 3 || Index == 59)
    {
        return(Queen);
    }
    else if(Index == 4 || Index == 60)
    {
        return(King);
    }
    else if(( 7 < Index && Index < 16)||
            (47 < Index && Index < 56))
    {
        return(Pawn);
    }
    else
    {
        return(NoRank);
    }
#if 0
    for(int32 Index = 16; Index <= 47; ++Index)
    {
        if(Pos[Index] == 11)
        {
            return(Rook);
        }
    }
#endif
}

internal void
InitializePlayer(player_info* Player, int32 Index, both_players_asset* BothPlayersAssets)
{
    Player->Index           = Index;
    Player->X               = 320 + 80*(Index%8);
    Player->Y               =   0 + 80*(Index/8);
    Player->Rank            = InitializeRank(Index);
    Player->Highlight       = false;
    
    if(Index == 0 || Index == 7)
    {
        Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[0].Rook;
    }
    else if(Index == 56 || Index == 63)
    {
        Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[1].Rook;
    }
    else if(Index == 1 || Index == 6)
    {
        Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[0].Knight;
    }
    else if(Index == 57 || Index == 62)
    {
        Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[1].Knight;
    }
    else if(Index == 2 || Index == 5)
    {
        Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[0].Bishop;
    }
    else if(Index == 58 || Index == 61)
    {
        Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[1].Bishop;
    }
    else if(Index == 3)
    {
        Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[0].Queen;
    }
    else if(Index == 59)
    {
        Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[1].Queen;
    }
    else if(Index == 4)
    {
        Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[0].King;
    }
    else if(Index == 60)
    {
        Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[1].King;
    }
    else if(7 < Index && Index < 16)
    {
        Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[0].Pawn;
    }
    else if(47 < Index && Index < 56)
    {
        Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[1].Pawn;
    }
    else if(16 <= Index && Index <= 47)
    {
        Player->PlayerAsset = NULL;
    }
#if 0
    for(int32 Index = 16; Index <= 47; ++Index)
    {
        if(Pos[Index] == 11)
        {
            Player->PlayerAsset = &BothPlayersAssets->PlayerAssetSet[1].Rook;
        }
    }
#endif
    
}

internal void
InitializePositions(position* Positions, both_players_asset* BothPlayersAssets)
{
    for(int32 Index = 0;
        Index < 64;
        Index++)
    {
        Positions[Index].X     = 320 + 80*(Index%8);
        Positions[Index].Y     =   0 + 80*(Index/8);
        Positions[Index].IsBeingTargeted = false;
        
        if(0 <= Index && Index <= 15)
        {
            Positions[Index].Player = Player1;
            Positions[Index].PlayerInfo = new player_info;
            InitializePlayer(Positions[Index].PlayerInfo, Index, BothPlayersAssets);
        }
        else if(48 <= Index && Index <= 63)
        {
            Positions[Index].Player = Player2;
            Positions[Index].PlayerInfo = new player_info;
            InitializePlayer(Positions[Index].PlayerInfo, Index, BothPlayersAssets);
        }
        else
        {
            Positions[Index].Player = NoPlayer;
            Positions[Index].PlayerInfo = 0;
        }
    }
#if 0
    for(int32 Index = 16; Index <= 47; ++Index)
    {
        if(Pos[Index] == 11)
        {
            Positions[Index].Player = Player2;
            Positions[Index].PlayerInfo = new player_info;
            InitializePlayer(Positions[Index].PlayerInfo, Index, BothPlayersAssets);
        }
    }
#endif
}

internal void
RenderPlayer(game_offscreen_buffer* Buffer, player_info* PlayerInfo)
{
    uint8* Src = (uint8*)PlayerInfo->PlayerAsset->Pixels;
    uint8* Des = (uint8*)Buffer->Memory;
    
    Des += PlayerInfo->X*4;
    Des += PlayerInfo->Y*Buffer->Pitch;
    
    for(int32 Y = 0;
        Y < PlayerInfo->PlayerAsset->Height;
        ++Y)
    {
        uint8 *Out = (uint8*)Des;
        uint8  *In  = (uint8*)Src;
        for(int32 X  = 0;
            X < PlayerInfo->PlayerAsset->Width;
            ++X)
        {
            uint8 BlueSrc  = *In;
            uint8 GreenSrc = *(In+1);
            uint8 RedSrc   = *(In+2);
            uint8 Alpha    = *(In+3);
            
            *Out     = (BlueSrc*Alpha + (*Out*(255-Alpha)))/255;
            *(Out+1) = (GreenSrc*Alpha + (*(Out+1)*(255-Alpha)))/255;
            *(Out+2) = (RedSrc*Alpha + (*(Out+2)*(255-Alpha)))/255;
            
            In+=4;
            Out+=4;
        }
        Src += PlayerInfo->PlayerAsset->Pitch;
        Des += Buffer->Pitch;
    }
}

internal void
RenderPlayers(game_offscreen_buffer* Buffer, position* Positions, 
              bitmap* AllPlayers, game_state* GameState)
{
    for(int32 Index = 0;
        Index < 64;
        Index++)
    {
        if(Positions[Index].Player != NoPlayer)
        {
            if(Positions[Index].PlayerInfo->Highlight)
            {
                Highlighter(Buffer, Index, 0x00BFA763);
            }
            
            
            if(Positions[Index].IsBeingTargeted)
            {
                Highlighter(Buffer, Index, 0x00FD7F7F);
            }
            
            //Alive Players are being drawn here
            RenderPlayer(Buffer, Positions[Index].PlayerInfo);
        }
        else
        {
            if(Positions[Index].IsBeingTargeted)
            {
                //0x00FCE883 //Lighter
                //0x00826644 //Darker
                //Highlighter(Buffer, Index, 0x007FF37F);//Lighter
                Highlighter(Buffer, Index, 0x007FB27F);//Darker
                //Highlighter(Buffer, Index, 0x00008080);
            }
        }
    } // for(int32 Index = 0; Index < 64; Index++)
    
    //Defeated Players are being drawn here
    for(int32 DefeatedPlayer1sCount = 0;
        DefeatedPlayer1sCount < GameState->DefeatedPlayer1sCount;
        DefeatedPlayer1sCount++)
    {
        RenderPlayer(Buffer, GameState->DefeatedPlayer1Info[DefeatedPlayer1sCount]);
    }
    for(int32 DefeatedPlayer2sCount = 0;
        DefeatedPlayer2sCount < GameState->DefeatedPlayer2sCount;
        DefeatedPlayer2sCount++)
    {
        RenderPlayer(Buffer, GameState->DefeatedPlayer2Info[DefeatedPlayer2sCount]);
    }
}

internal void
ToggleTurn(enum player* Turn)
{
    if(*Turn == Player1)
        *Turn = Player2;
    else
        *Turn = Player1;
}

internal void
GameUpdateAndRender(game_state* GameState, game_input* Input, game_offscreen_buffer *Buffer, bitmap* AllPlayers, position* Positions)
{
    int32  MouseX = Input->MousePointer.X;
    int32  MouseY = Input->MousePointer.Y;
    bool32 ButtonDown = Input->IsMousesLeftButtonDown;
    
    if((320 <= MouseX && MouseX < 960)&&
       (  0 <= MouseY && MouseY < 640))
    {
        GameState->NewIndex = MouseX/80 + 8*(MouseY/80) - 4;
        int32 NewIndex = GameState->NewIndex;
        int32 OldIndex = GameState->OldIndex;
        player Turn = GameState->Turn;
        
        if(ButtonDown)
        {
            if((Positions[NewIndex].Player == Player1 && Turn == Player1)||
               (Positions[NewIndex].Player == Player2 && Turn == Player2))
            {
                // Here Player and target positions are dehighlighted
                Positions[OldIndex].PlayerInfo->Highlight = false;
                if(Positions[OldIndex].PlayerInfo->Rank == Pawn)
                {
                    PawnActions(Positions, OldIndex, false, Turn);
                }
                else if(Positions[OldIndex].PlayerInfo->Rank == Rook)
                {
                    RookActions(Positions, OldIndex, false, Turn);
                }
                else if(Positions[OldIndex].PlayerInfo->Rank == Bishop)
                {
                    BishopActions(Positions, OldIndex, false, Turn);
                }
                else if(Positions[OldIndex].PlayerInfo->Rank == Queen)
                {
                    RookActions(Positions, OldIndex, false, Turn);
                    BishopActions(Positions, OldIndex, false, Turn);
                }
                else if(Positions[OldIndex].PlayerInfo->Rank == Knight)
                {
                    KnightActions(Positions, OldIndex, false, Turn);
                }
                else if(Positions[OldIndex].PlayerInfo->Rank == King)
                {
                    KingActions(Positions, OldIndex, false, Turn);
                }
                
                //Here Player and target positions are highlighted 
                Positions[NewIndex].PlayerInfo->Highlight = true;
                if(Positions[NewIndex].PlayerInfo->Rank == Pawn)
                {
                    PawnActions(Positions, NewIndex, true, Turn);
                }
                else if(Positions[NewIndex].PlayerInfo->Rank == Rook)
                {
                    RookActions(Positions, NewIndex, true, Turn);
                }
                else if(Positions[NewIndex].PlayerInfo->Rank == Bishop)
                {
                    BishopActions(Positions, NewIndex, true, Turn);
                }
                else if(Positions[NewIndex].PlayerInfo->Rank == Queen)
                {
                    RookActions(Positions, NewIndex, true, Turn);
                    BishopActions(Positions, NewIndex, true, Turn);
                }
                else if(Positions[NewIndex].PlayerInfo->Rank == Knight)
                {
                    KnightActions(Positions, NewIndex, true, Turn);
                }
                else if(Positions[NewIndex].PlayerInfo->Rank == King)
                {
                    KingActions(Positions, NewIndex, true, Turn);
                }
                
                GameState->OldIndex = GameState->NewIndex;
            }
            else
            {
                // Here player moves from its position
                if(Positions[NewIndex].IsBeingTargeted)
                {
                    if(Positions[OldIndex].PlayerInfo->Rank == Pawn)
                    {
                        PawnActions(Positions, OldIndex, false, Turn);
                    }
                    else if(Positions[OldIndex].PlayerInfo->Rank == Rook)
                    {
                        RookActions(Positions, OldIndex, false, Turn);
                    }
                    else if(Positions[OldIndex].PlayerInfo->Rank == Bishop)
                    {
                        BishopActions(Positions, OldIndex, false, Turn);
                    }
                    else if(Positions[OldIndex].PlayerInfo->Rank == Queen)
                    {
                        RookActions(Positions, OldIndex, false, Turn);
                        BishopActions(Positions, OldIndex, false, Turn);
                    }
                    else if(Positions[OldIndex].PlayerInfo->Rank == Knight)
                    {
                        KnightActions(Positions, OldIndex, false, Turn);
                    }
                    else if(Positions[OldIndex].PlayerInfo->Rank == King)
                    {
                        KingActions(Positions, OldIndex, false, Turn);
                    }
                    
                    Positions[OldIndex].PlayerInfo->Highlight = false;
                    
                    //Here Defeated players get out of the game/board
                    if(Positions[NewIndex].PlayerInfo)
                    {
                        if(Positions[NewIndex].Player == Player1)
                        {
                            Positions[NewIndex].PlayerInfo->X = 240 - 80*(GameState->DefeatedPlayer1sCount/8);
                            Positions[NewIndex].PlayerInfo->Y = 80*(GameState->DefeatedPlayer1sCount%8);
                            
                            GameState->DefeatedPlayer1Info[GameState->DefeatedPlayer1sCount] = Positions[NewIndex].PlayerInfo;
                            GameState->DefeatedPlayer1sCount++;
                        }
                        else if(Positions[NewIndex].Player == Player2)
                        {
                            Positions[NewIndex].PlayerInfo->X = 960 + 80*(GameState->DefeatedPlayer2sCount/8);
                            Positions[NewIndex].PlayerInfo->Y = 80*(GameState->DefeatedPlayer2sCount%8);
                            
                            GameState->DefeatedPlayer2Info[GameState->DefeatedPlayer2sCount] = Positions[NewIndex].PlayerInfo;
                            GameState->DefeatedPlayer2sCount++;
                        }
                    }
                    Positions[NewIndex].PlayerInfo = Positions[OldIndex].PlayerInfo;
                    Positions[NewIndex].Player = Positions[OldIndex].Player;
                    
                    Positions[NewIndex].PlayerInfo->X = Positions[NewIndex].X;
                    Positions[NewIndex].PlayerInfo->Y = Positions[NewIndex].Y;
                    
                    Positions[OldIndex].PlayerInfo = 0;
                    Positions[OldIndex].Player = NoPlayer;
                    
                    GameState->OldIndex = GameState->NewIndex;
                    
                    ToggleTurn(&GameState->Turn);
                }
            }
        } //if(ButtonDown)
    }
    
    RenderGameBoard(Buffer, GameState->NewIndex);
    RenderPlayers(Buffer, Positions, AllPlayers, GameState);
    
    for(int Index = 0; Index < 64; Index++)
    {
        if(Positions[Index].PlayerInfo->Rank); 
    }
}
