#if 0
int32 Pos[64] = {
    2,  3,  4,  5,  6,  4,  3,  2,
    1,  1,  1,  1,  1,  1,  1,  1,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, 11,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    12, 12, 12, 12, 12, 12, 12, 12,
    11, 10,  9,  8,  7,  9, 10, 11 
};
#endif

internal player
OpponentTurn(player Turn)
{
    return Turn == Player1 ? Player2 : Player1;
}

internal void
PawnActions(position* Positions, int32 Index, bool32 AssignValue, player Turn)
{
    if(Positions[Index].Player == Player1)
    {
        if(Positions[Index + 8].Player == NoPlayer)
        {
            Positions[Index + 8].IsBeingTargeted = AssignValue;
            if((Index < 16)&&
               (Positions[Index + 16].Player == NoPlayer))
            {
                Positions[Index + 16].IsBeingTargeted = AssignValue;
            }
        }
        if(Positions[Index + 7].Player == OpponentTurn(Turn))
        {
            Positions[Index + 7].IsBeingTargeted = AssignValue;
        }
        if(Positions[Index + 9].Player == OpponentTurn(Turn))
        {
            Positions[Index + 9].IsBeingTargeted = AssignValue;
        }
    }
    else if(Positions[Index].Player == Player2)
    {
        if(Positions[Index - 8].Player == NoPlayer)
        {
            Positions[Index - 8].IsBeingTargeted = AssignValue;
            if((47 < Index)&&
               (Positions[Index - 16].Player == NoPlayer))
            {
                Positions[Index - 16].IsBeingTargeted = AssignValue;
            }
        }
        if(Positions[Index - 7].Player == OpponentTurn(Turn))
        {
            Positions[Index - 7].IsBeingTargeted = AssignValue;
        }
        if(Positions[Index - 9].Player == OpponentTurn(Turn))
        {
            Positions[Index - 9].IsBeingTargeted = AssignValue;
        }
    }
}

internal void
RookActions(position* Positions, int32 Index, bool32 AssignValue, player Turn)
{
    for(int32 I = Index; 
        I <= 55;
        I += 8)
    {
        if(Positions[I + 8].Player == NoPlayer)
        {
            Positions[I + 8].IsBeingTargeted = AssignValue;
        }
        else if(Positions[I + 8].Player == Turn)
        {
            break;
        }
        else 
        {
            Positions[I + 8].IsBeingTargeted = AssignValue;
            break;
        }
    }
    for(int32 I = Index; 
        8 <= I;
        I -= 8)
    {
        if(Positions[I - 8].Player == NoPlayer)
        {
            Positions[I - 8].IsBeingTargeted = AssignValue;
        }
        else if(Positions[I - 8].Player == Turn)
        {
            break;
        }
        else 
        {
            Positions[I - 8].IsBeingTargeted = AssignValue;
            break;
        }
    }
    for(int32 I = Index;
        I <= Index + 6 - Index%8;
        I += 1)
    {
        if(Positions[I + 1].Player == NoPlayer)
        {
            Positions[I + 1].IsBeingTargeted = AssignValue;
        }
        else if(Positions[I + 1].Player == Turn)
        {
            break;
        }
        else 
        {
            Positions[I + 1].IsBeingTargeted = AssignValue;
            break;
        }
    }
    for(int32 I = Index;
        I >= Index - Index%8 + 1;
        I -= 1)
    {
        if(Positions[I - 1].Player == NoPlayer)
        {
            Positions[I - 1].IsBeingTargeted = AssignValue;
        }
        else if(Positions[I - 1].Player == Turn)
        {
            break;
        }
        else 
        {
            Positions[I - 1].IsBeingTargeted = AssignValue;
            break;
        }
    }
}

internal void
BishopActions(position* Positions, int32 Index, bool32 AssignValue, player Turn)
{
    int32 Rem, Div, Comparator;
    Rem = Index%8;
    Div = 7 - Index/8;
    
    Comparator = Div > Rem ? Index%7 : (Index%8 + Index/8 - 7)*8 + 7;
    for(int32 I = Index;
        I > Comparator;
        I -= 7)
    {
        if(Positions[I - 7].Player == NoPlayer)
        {
            Positions[I - 7].IsBeingTargeted = AssignValue;
        }
        else if(Positions[I - 7].Player == Turn)
        {
            break;
        }
        else
        {
            Positions[I - 7].IsBeingTargeted = AssignValue;
            break;
        }
    }
    
    Comparator = Div > Rem ? (Index%8 + Index/8)*8 : 56 + Index/8 - (7 - Index%8);
    for(int32 I = Index;
        I < Comparator;
        I += 7)
    {
        if(Positions[I + 7].Player == NoPlayer)
        {
            Positions[I + 7].IsBeingTargeted = AssignValue;
        }
        else if(Positions[I + 7].Player == Turn)
        {
            break;
        }
        else
        {
            Positions[I + 7].IsBeingTargeted = AssignValue;
            break;
        }
    }
    
    Rem = Index%8;
    Div = Index/8;
    
    Comparator = Div > Rem ? 
        56 - (Index%8)*8 - (7 - (Index/8))*8 : 7 - Index/8 - (7 - Index%8);
    for(int32 I = Index;
        I > Comparator;
        I -= 9)
    {
        if(Positions[I - 9].Player == NoPlayer)
        {
            Positions[I - 9].IsBeingTargeted = AssignValue;
        }
        else if(Positions[I - 9].Player == Turn)
        {
            break;
        }
        else
        {
            Positions[I - 9].IsBeingTargeted = AssignValue;
            break;
        }
    }
    
    Comparator = Rem > Div ?
        7 + (7 - Index%8)*8 + (Index/8)*8 : 63 - Index/8 + Index%8;
    for(int32 I = Index;
        I < Comparator;
        I += 9)
    {
        if(Positions[I + 9].Player == NoPlayer)
        {
            Positions[I + 9].IsBeingTargeted = AssignValue;
        }
        else if(Positions[I + 9].Player == Turn)
        {
            break;
        }
        else
        {
            Positions[I + 9].IsBeingTargeted = AssignValue;
            break;
        }
    }
}

internal void
KnightActions(position* Positions, int32 Index, bool32 AssignValue, player Turn)
{
    if((2 <= Index/8 && 1 <= Index%8)&&
       (Positions[Index - 17].Player != Turn))
    {
        Positions[Index - 17].IsBeingTargeted = AssignValue;
    }
    if((2 <= Index/8 && Index%8 <= 6)&&
       (Positions[Index - 15].Player != Turn))
    {
        Positions[Index - 15].IsBeingTargeted = AssignValue;
    }
    if((Index%8 <= 6 && Index/8 <= 5)&&
       (Positions[Index + 17].Player != Turn))
    {
        Positions[Index + 17].IsBeingTargeted = AssignValue;
    }
    if((1 <= Index%8 && Index/8 <= 5)&&
       (Positions[Index + 15].Player != Turn))
    {
        Positions[Index + 15].IsBeingTargeted = AssignValue;
    }
    if((2 <= Index%8 && 1 <= Index/8)&&
       (Positions[Index - 10].Player != Turn))
    {
        Positions[Index - 10].IsBeingTargeted = AssignValue;
    }
    if((2 <= Index%8 && Index/8 <= 6)&&
       (Positions[Index + 6].Player != Turn))
    {
        Positions[Index + 6].IsBeingTargeted = AssignValue;
    }
    if((Index%8 <= 5 && 1 <= Index/8)&&
       (Positions[Index - 6].Player != Turn))
    {
        Positions[Index - 6].IsBeingTargeted = AssignValue;
    }
    if((Index%8 <= 5 && Index/8 <= 6)&&
       (Positions[Index + 10].Player != Turn))
    {
        Positions[Index + 10].IsBeingTargeted = AssignValue;
    }
}

internal void
KingActions(position* Positions, int32 Index, bool32 AssignValue, player Turn)
{
    if((1 <= Index/8)&&
       (Positions[Index - 8].Player != Turn))
    {
        Positions[Index - 8].IsBeingTargeted = AssignValue;
    }
    if((Index/8 <= 6)&&
       (Positions[Index + 8].Player != Turn))
    {
        Positions[Index + 8].IsBeingTargeted = AssignValue;
    }
    if((Index%8 <= 6)&&
       (Positions[Index + 1].Player != Turn))
    {
        Positions[Index + 1].IsBeingTargeted = AssignValue;
    }
    if((1 <= Index%8)&&
       (Positions[Index - 1].Player != Turn))
    {
        Positions[Index - 1].IsBeingTargeted = AssignValue;
    }
    if((1 <= Index%8 && 1 <= Index/8)&&
       (Positions[Index - 9].Player != Turn))
    {
        Positions[Index - 9].IsBeingTargeted = AssignValue;
    }
    if((Index%8 <= 6 && 1 <= Index/8)&&
       (Positions[Index - 7].Player != Turn))
    {
        Positions[Index - 7].IsBeingTargeted = AssignValue;
    }
    if((1 <= Index%8 && Index/8 <= 6)&&
       (Positions[Index + 7].Player != Turn))
    {
        Positions[Index + 7].IsBeingTargeted = AssignValue;
    }
    if((Index%8 <= 6 && Index/8 <= 6)&&
       (Positions[Index + 9].Player != Turn))
    {
        Positions[Index + 9].IsBeingTargeted = AssignValue;
    }
}
