#include "BingrowSquare.h"
#include "std_lib_facilities.h"
#include <algorithm>
#include <cassert>

class BingrowBoard {
    public:
        BingrowBoard(int n); // Constructor for board, takes an int for intXint board size
        
        int GetSize();
        
        void GrowBoard(ArrowType direction);

        bool UpdateBoard(Symbol symbol); // Returns true if symbol is valid for board size
        
        bool WinningBoard(void); // Returns true and covers symbol if symbol called is on board

        void SetSquare(int row, int col, BingrowSquare square); // For Autograder

        Symbol GenerateSymbol(int col);

        ~BingrowBoard() {}

        deque<deque<BingrowSquare>> getBoard();

    private:
        deque<deque<BingrowSquare>> Board; 
        int size;
        int rightboundcol;
        int leftboundcol; 
        int origsize;
};  