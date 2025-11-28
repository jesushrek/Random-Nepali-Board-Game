#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define INIT_MAX_DEPTH 9 
#define MOVE_MAX_DEPTH 5

static const char g_symbolX = 'x';
static const char g_symbolO = 'o';
static const char g_empty = '\0';
static const int g_size = 3;

typedef struct 
{ 
    int row;
    int column;
}Spot;

typedef struct 
{ 
    Spot from; 
    Spot to;
}Move;

/* 00 01 02 
 * 10 11 12 
 * 20 21 22 */

bool isInit(char c[g_size][g_size])
{ 
    int countX = 0;
    int countO = 0;
    int row = 0;
    for(; row < g_size; ++row)
    { 
        int column = 0;
        for(; column < g_size; ++column)
        { 
            if(c[row][column] == g_symbolX)
                ++countX;
            if(c[row][column] == g_symbolO)
                ++countO;
        }
    }
    return (countO == g_size) && (countX == g_size);
}


void printBoard(char board[g_size][g_size])
{ 
    int row = 0;
    for(; row < g_size; ++row)
    { 
        int column = 0;
        for(; column < g_size; ++column)
        { 
            printf("[%c]", (board[row][column] == g_empty)? ' ' : board[row][column]);
        }
        printf("\n");
    }
}

int evaluate(char board[g_size][g_size])
{ 
    if( board[0][0] != g_empty && board[0][0] == board[1][1] && board[1][1] == board[2][2] )
        return (board[0][0] == g_symbolX)? +10 : -10;
    if( board[0][2] != g_empty && board[0][2] == board[1][1] && board[1][1] == board[2][0] )
        return (board[0][2] == g_symbolX)? +10 : -10;

    int i = 0;
    for(; i < g_size; ++i )
    { 
        if( board[0][i] != g_empty && board[0][i] == board[1][i] && board[1][i] == board[2][i] )
            return (board[0][i] == g_symbolX)? +10 : -10;
        if( board[i][0] != g_empty && board[i][0] == board[i][1] && board[i][1] == board[i][2] )
            return (board[i][0] == g_symbolX)? +10 : -10;
    }
    return 0;
}

bool isValidMove(Spot to, Spot from)
{
    if (from.row < 0 || from.row > 2 || from.column < 0 || from.column > 2 || 
            to.row < 0 || to.row > 2 || to.column < 0 || to.column > 2)
        return false;

    int rowDiff = abs(from.row - to.row);
    int colDiff = abs(from.column - to.column);

    if (rowDiff == 0 && colDiff == 0) 
        return false; 

    if ((rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1))
        return true;

    if (rowDiff == 1 && colDiff == 1)
    {
        bool isStartCenter = (from.row == 1 && from.column == 1);
        bool isEndCenter = (to.row == 1 && to.column == 1);

        if (isStartCenter || isEndCenter)
            return true;
    }

    return false;
}

Spot getSpot() 
{ 
    int r = 0;
    int c = 0;

    while(true)
    {
        printf("Enter the row: ");
        scanf("%d", &r);
        printf("Enter the column: ");
        scanf("%d", &c);
        if( r > 2 || c > 2 )
        {
            printf("\nThe coordinate (%d, %d) is out of bound.\n", r, c);
            continue; 
        }
        break;
    }
    Spot s = {r, c};
    return s;
}

bool move(char c[g_size][g_size], bool isTurnX) 
{ 
    char symbol = isTurnX? g_symbolX : g_symbolO;
    printf("\nMove from:\n");
    Spot from = getSpot();

    if( c[from.row][from.column] == g_empty)
    {
        printf("\nEmpty spaces may not be move!\n");
        return false;
    }
    if(symbol != c[from.row][from.column])
    { 
        printf("\nThat spot isn't yours!\n");
        return false;
    }
    printf("\nMove To:\n");
    Spot to = getSpot();

    if(c[to.row][to.column] != g_empty)
    {
        printf("\nThat place is not empty!\n");
        return false;
    }
    if(!isValidMove(to, from))
    { 
        printf("That move isn't valid\n");
        return false;
    }


    c[to.row][to.column] = c[from.row][from.column];
    c[from.row][from.column] = g_empty;
    return true;
}

void check(char c[3][3])
{ 
    int score = evaluate(c);
    if(score == 10)
        printf("\nx won!\n");
    if(score == -10)
        printf("\no won!\n");
    if(score == 0)
        printf("Its a draw");
}


void multiPlayer() 
{ 
    bool isTurnX = false;
    char c[3][3] = {
        { g_empty, g_empty, g_empty },
        { g_empty, g_empty, g_empty }, 
        { g_empty, g_empty, g_empty } 
    };

    while(!isInit(c))
    {

        printf("\n%c's turn \n", isTurnX? g_symbolX : g_symbolO);
        Spot get = getSpot();
        if(c[get.row][get.column] != g_empty)
        { 
            printf("\nThat place is not empty:\n");
            continue;
        }
        if(evaluate(c) != 0)
        { 
            printf("\n%c's won! \n", isTurnX? g_symbolX : g_symbolO);
            return;
        }        
        c[get.row][get.column] = isTurnX? g_symbolX : g_symbolO;
        isTurnX = !isTurnX;
        printBoard(c);
    }

    while(evaluate(c) == 0)
    { 
        printf("\n%c's turn \n", isTurnX? g_symbolX : g_symbolO);
        if(!move(c, isTurnX))
        { 
            printf("\n Please try again. \n");
            continue;
        }
        isTurnX = !isTurnX;
        printBoard(c);
    }
    printf("\n Results: ");
    printBoard(c);
    check(c);
}

bool isMoveAva(char c[g_size][g_size])
{
    int r = 0; 
    for(; r < g_size; ++r)
    { 
        int col = 0; 
        for(; col < g_size; ++col)
        { 
            if(c[r][col] == g_empty)
                return true;
        }
    }
    return false;
}

int moveMiniMax( char c[g_size][g_size], int depth, bool isMax)
{ 
    int score = evaluate(c);
    if(score != 0 || depth == 0)
        return score;
    if(!isMoveAva(c))
        return 0;
    if(isMax)
    { 
        int best = -100000;
        int row = 0;
        for(; row < g_size; ++row)
        { 
            int column = 0;
            for(; column < g_size; ++column)
            { 
                if(c[row][column] == g_symbolX)
                { 
                    int innerRow = 0;
                    for(; innerRow < g_size; ++innerRow)
                    { 
                        int innerColumn = 0;
                        for(; innerColumn  < g_size; ++innerColumn)
                        { 
                            Move s = {};
                            s.from.row = row;
                            s.from.column = column;
                            s.to.row = innerRow;
                            s.to.column = innerColumn;
                            if( isValidMove(s.to, s.from) && c[s.to.row][s.to.column] == g_empty)
                            { 
                                c[s.to.row][s.to.column] = c[s.from.row][s.from.column];
                                c[s.from.row][s.from.column] = g_empty;
                                int newValue = moveMiniMax(c, depth - 1, !isMax);
                                best = (best > newValue)? best : newValue;
                                c[s.from.row][s.from.column] = g_symbolX;
                                c[s.to.row][s.to.column] = g_empty;
                            }
                        }
                    }
                }
            }
        }
        return best;
    }
    else 
    { 
        int best = 100000;
        int row = 0;
        for(; row < g_size; ++row)
        { 
            int column = 0;
            for(; column < g_size; ++column)
            { 
                if(c[row][column] == g_symbolO) 
                {
                    int innerRow = 0;
                    for(; innerRow < g_size; ++innerRow)
                    { 
                        int innerColumn = 0;
                        for(; innerColumn < g_size; ++innerColumn)
                        { 
                            Move s = {};
                            s.from.row = row; 
                            s.from.column = column;
                            s.to.row = innerRow;
                            s.to.column = innerColumn;
                            if( isValidMove(s.to, s.from) && c[s.to.row][s.to.column] == g_empty)
                            { 
                                c[s.to.row][s.to.column] = c[s.from.row][s.from.column];
                                c[s.from.row][s.from.column] = g_empty;
                                int newValue = moveMiniMax(c, depth -1, !isMax);
                                best = (best < newValue)? best : newValue;
                                c[s.from.row][s.from.column] = g_symbolO;
                                c[s.to.row][s.to.column] = g_empty;
                            }
                        }
                    }
                }
            }
        }
        return best;
    }
}

int initMiniMax( char c[g_size][g_size], int depth, bool isMax )
{ 
    int score = evaluate(c);

    if(score != 0 || depth == 0)
        return score;

    if(!isMoveAva(c))
        return 0;

    if(isMax)
    { 
        int best = -100000;
        int row = 0;
        for(; row < g_size; ++row) 
        { 
            int column = 0;
            for(; column < g_size; ++column)
            { 
                if(c[row][column] == g_empty)
                { 
                    c[row][column] = g_symbolX;
                    int value = initMiniMax(c, depth - 1, !isMax);
                    best = (best > value)? best : value;
                    c[row][column] = g_empty;
                }
            }
        }
        return best;
    }
    else  
    { 
        int best = 100000;
        int row = 0;
        for(; row < g_size; ++row)
        { 
            int column = 0;
            for(; column < g_size; ++column)
            { 
                if(c[row][column] == g_empty)
                {
                    c[row][column] = g_symbolO;
                    int value = initMiniMax(c, depth - 1, !isMax);
                    best = (best < value)? best : value;
                    c[row][column] = g_empty;
                }
            }
        }
        return best;
    }
    return 0;
}

Move moveBestMove(char c[g_size][g_size], bool isTurnX)
{ 
    Move x = { { -1, -1 }, { -1, -1} };
    int bestScore = isTurnX? -100000 : 100000;
    char symbol = isTurnX? g_symbolX : g_symbolO;
    int row = 0;
    for(; row < g_size; ++row)
    { 
        int column = 0;
        for(; column < g_size; ++column)
        { 
            if(c[row][column] == symbol)
            {
                int innerRow = 0;
                for(; innerRow < g_size; ++innerRow)
                { 
                    int innerColumn = 0;
                    for(; innerColumn < g_size; ++innerColumn)
                    { 
                        Move s = {};
                        s.from.row = row;
                        s.from.column = column;
                        s.to.row = innerRow;
                        s.to.column = innerColumn;
                        if(isValidMove(s.to, s.from) && c[s.to.row][s.to.column] == g_empty)
                        {
                            c[s.to.row][s.to.column] = c[s.from.row][s.from.column];
                            c[s.from.row][s.from.column] = g_empty;
                            int newValue = moveMiniMax(c, MOVE_MAX_DEPTH, !isTurnX);
                            c[s.from.row][s.from.column] = symbol;
                            c[s.to.row][s.to.column] = g_empty;

                            if(isTurnX) 
                            { 
                                if(newValue > bestScore)
                                {
                                    bestScore = newValue;
                                    x.from.row = s.from.row;
                                    x.from.column = s.from.column;
                                    x.to.row = s.to.row;
                                    x.to.column = s.to.column;
                                }
                            }
                            else 
                            {
                                if(newValue < bestScore)
                                {
                                    bestScore = newValue;
                                    x.from.row = s.from.row;
                                    x.from.column = s.from.column;
                                    x.to.row = s.to.row;
                                    x.to.column = s.to.column;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return x;
}

Spot initbestMove(char c[g_size][g_size], bool isTurnX) 
{ 
    Spot x = { -1, -1 };
    int bestScore = isTurnX? -100000 : 100000;
    int row = 0;
    for(; row < g_size; ++row)
    { 
        int column = 0;
        for(; column < g_size; ++column)
        { 
            if(c[row][column] == g_empty)
            { 
                c[row][column] = isTurnX? g_symbolX : g_symbolO;
                int newScore = initMiniMax(c, INIT_MAX_DEPTH, !isTurnX);
                c[row][column] = g_empty;
                if(isTurnX)
                { 
                    if(newScore > bestScore)
                    { 
                        bestScore = newScore;
                        x.row = row;
                        x.column = column;
                    }
                }
                else 
                { 
                    if(newScore < bestScore)
                    { 
                        bestScore = newScore;
                        x.row = row;
                        x.column = column;
                    }
                }
            }
        }
    }
    return x;
}

void computerPlace(char c[3][3], bool isTurnX) 
{ 
    Spot selected = initbestMove(c, isTurnX);
    if( selected.row == -1 && selected.column == -1)
    { 
        return;
    }
    c[selected.row][selected.column] = isTurnX? g_symbolX : g_symbolO;
}

void computerMove(char c[3][3], bool isTurnX)
{ 
    Move selected = moveBestMove(c, isTurnX);
    if(selected.from.row == -1)
    {
        return;
    }
    c[selected.from.row][selected.from.column] = g_empty;
    c[selected.to.row][selected.to.column] = isTurnX? g_symbolX : g_symbolO;
}

void singlePlayer() 
{ 
    char board[3][3] = { 
        { g_empty, g_empty, g_empty },
        { g_empty, g_empty, g_empty },
        { g_empty, g_empty, g_empty }
    };
    bool isTurnX = false;

    while(!isInit(board))
    { 
        printf("\no's turn\n");
        Spot s = getSpot();
        if(board[s.row][s.column] != g_empty)
        { 
            printf("\nThat spot is not empty:\n");
            continue;
        }
        board[s.row][s.column] = g_symbolO;
        computerPlace(board, true);
        printBoard(board);
        if(evaluate(board) != 0)
        { 
            check(board);
            return;
        }
        isTurnX = !isTurnX;
    }
    while(true) 
    { 
        if(evaluate(board))
        { 
            check(board);
            return;
        }       
        printf("\no's turn\n");
        if(!move(board, false))
        {
            printf("Try again:");
            continue;
        }
        computerMove(board, true);
        printBoard(board);
    }
}

void botVsBot() 
{ 
    bool isTurnX = true;
    char board[3][3] = {};
    while(!isInit(board))
    { 
        computerPlace(board, isTurnX);
        printBoard(board);
        isTurnX = !isTurnX;
    }
}

int main() 
{ 
    singlePlayer();
    return 0;
}
