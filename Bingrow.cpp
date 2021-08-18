#include "BingrowBoard.h"
#include <iostream>

int BingrowBoard::GetSize() {return size;}

deque<deque<BingrowSquare>> BingrowBoard::getBoard() {return Board;}

void BingrowBoard::SetSquare(int row, int col, BingrowSquare square) // Set a square in ith row, jth column to square, indexing starts at [1,1]
{
   for (int i = 1; i <= size; i++)
   {
      for (int j = 1; j <= size; j++)
      {
         if (i == row && j == col)
         {
            Board[i-1][j-1] = square;
            break;
         }
      }
   }
}

bool BingrowBoard::UpdateBoard(Symbol symbol) // Covers symbol called, grows Board when arrows are covered
{
   int instances = 0;
   bool symbolFound = false;

   if (symbol.getSymbolType() == Number) // If symbol is a number, cover all instances of that number
   {
      for (int i = 0; i < size; i++)
      {
         for (int j = 0; j < size; j++)
         {
            if (Board[i][j].getSymbol().getNumber() == symbol.getNumber())
            {
               Board[i][j].setCovered(true);
               bool symbolFound = true;
            }
         }
      }
   }
   if (symbol.getSymbolType() == Arrow) // If symbol type is arrow, iterate through board, GrowBoard(Arrow) up to twice, and cover all arrows of that type
   {
      for (int i = 0; i < size; i++)
      {
         for (int j = 0; j < GetSize(); j++)
         {
            if (Board[i][j].getSymbol().getArrowType() == symbol.getArrowType() && Board[i][j].getSymbol().getSymbolType() == Arrow)
            {
               bool symbolFound = true;
               if (instances < 2) {instances += 1;}; // Limits expansion to twice per UpdateBoard(Arrow) 
               Board[i][j].setCovered(true);
            }
         }
      }
      for (int k = 1; k <= instances; k++) // Grow board (max of twice) in the arrow's direction
      {
         GrowBoard(symbol.getArrowType());
      }
   }
   return symbolFound;
}

bool BingrowBoard::WinningBoard(void) // Checks for winning condition of the Bingrow Board, returns true if met, false if not
{
   bool Winner = false;

   int i = 0, rowcovers = 0; // # of covered squares in a single row
   
   while (i < size && Winner == false) // Checks for 5 or more covered squares in a row
   {
      if (rowcovers >= 5) {Winner = true;}
      else {rowcovers = 0;}
      int j = 0;
      while (j < size && rowcovers < 5)
      {            
         if (Board[i][j].getCovered() == true)
         {
            rowcovers += 1;
            j++;
         }
         else 
         {
            j++;
         }
      }
      i++;
   }
   int k, colcovers = 0; // # of covered squares in a single column
   
   while (k < size && Winner == false) // Checks for 5 or more covered squares in a column
   {
      if (colcovers >= 5) {Winner = true;}
      else {colcovers = 0;}
      int l = 0;
      while (l < size && colcovers < 5)
      {            
         if (Board[l][k].getCovered() == true)
         {
            colcovers += 1;
            l++;
         }
         else 
         {
            l++;
         }
      }
      k++;
   }
   int diagcovers = 0; // # of covered squares in a main diagonal
   int m = 0; 
   int n = size-1;
   for (;m < size; m++) // Checks top right -> bottom left main diagonal
   {
      if (diagcovers >= 5) {Winner = true;} 
      if (Board[m][n-m].getCovered() == true)
      {
         diagcovers += 1;
      }
   }
   for (;n >= 0; n--) // Checks top left -> bottom right main diagonal
   {
      if (diagcovers >= 5) {Winner = true;}
      if (Board[n][n].getCovered() == true)
      {
         diagcovers += 1;
      }
   }
   return Winner;
}


BingrowBoard::BingrowBoard(int n) 
{  
   size = n; 
   leftboundcol = 1; 
   rightboundcol = n;
   origsize = n;
  
   bool validSize = 1;
   
   if ((n < 5) or (n % 2 == 0)) // Checks for even # of rows/colums or initSize < 5 (ILLEGAL)
   {
      bool validSize = 0;
      cout << "The entered board size of: " << n << " by " << n << " was invalid. Please enter an odd number greater than or equal to 5." << endl;
   }
   assert(validSize);

   for (int rowNum = 0; rowNum < n; rowNum++) // Fills constructed Board with new random squares
   {
      deque<BingrowSquare> row;

      for (int colNum = 1; colNum <= n; colNum++) 
      {
         BingrowSquare square;
         square.setSymbol(GenerateSymbol(colNum)); // Generate a symbol within this new square and add it to a row, then push_back to Board
         square.setCovered(false);
         row.push_back(square);
      }
      Board.push_back(row);
   }
   Symbol freesymb; // Creates square with symbol of type Free (pre-covered), places it in center of board
   freesymb.setSymbolType(Free);
   BingrowSquare freeSquare;
   freeSquare.setSymbol(freesymb);
   freeSquare.setCovered(true);
   Board[n/2][n/2] = freeSquare;
}

void BingrowBoard::GrowBoard(ArrowType direction) // Grows board in direction specified by ArrowType direction
{
   size += 1;
   deque<BingrowSquare> col; 
   deque<BingrowSquare> row; 
   
   if (direction == 0 || direction == 2) // Dec/Increment left/rightmost column according to arrow direction
   {
      leftboundcol -= 1;
   }
   if (direction == 1 || direction == 3) 
   {
      rightboundcol += 1;
   }
   for (int i = 0; i < size-1; i++)  
   {
      BingrowSquare square;
      
      if (direction == 0 || direction == 2) // Appropriate symbols for for new leftbound or rightbound column index are generated
      {
         square.setSymbol(GenerateSymbol(leftboundcol)); 
      }
      if (direction == 1 || direction == 3)
      {
         square.setSymbol(GenerateSymbol(rightboundcol)); 
      }
      square.setCovered(false);
      col.push_back(square);
   }

   cout << "New column added with elements: " << endl; // TESTING GROWTH
   for (auto &elem: col)
   {
      cout << elem.getSymbol().getNumber() << " ";
   }
   cout << endl;

   for (int i = 0; i < size-1; i++) // Column is added in at the "front" (left) or "back" (right) of board according to direction of arrow
   {
      if (direction == 0 | direction == 2)
      {
         Board[i].push_front(col[i]);
      }
      else
      {
         Board[i].push_back(col[i]);
      }
   }
   for (int i = leftboundcol; i <= rightboundcol; i++) // A row is generated with appropriate values for every column it spans
   {
      BingrowSquare square;
      square.setSymbol(GenerateSymbol(i));
      square.setCovered(false);
      row.push_back(square);

   }
   
   cout << "New row added with elements: " << endl; // TESTING GROWTH
   for (auto &elem: row)
   {
      cout << elem.getSymbol().getNumber() << ", ";
   }
   cout << endl;

   if (direction == 0 || direction == 1)  // Adds row to top for Upper arrows
   {
      Board.push_front(row);
   }
   if (direction == 2 || direction == 3) // Adds row to bottom for Lower arrows
   {
      Board.push_back(row);
   }
}


// Generates a symbol for column col on the board
Symbol BingrowBoard::GenerateSymbol(int col) // n is the current size of the board, col is index of column being generated for
{
   
   Symbol tempSymbol;
   int randNum = (int) (((float) rand()/RAND_MAX*(3*origsize*size+4)));
   if (randNum >= 3*origsize*size)
   {
      tempSymbol.setSymbolType(Arrow);   
      if (randNum == 3*origsize*size)
      {
         tempSymbol.setArrowType(UpperLeft);
      }
      else if (randNum == 3*origsize*size+1)
      {
         tempSymbol.setArrowType(UpperRight);
      }
      else if (randNum == 3*origsize*size+2)
      {
         tempSymbol.setArrowType(LowerLeft);
      }
      else
      {
         tempSymbol.setArrowType(LowerRight);
      }      
   }
   else
   {
      randNum = (int) (((float) rand()/RAND_MAX*(3*origsize)));
      tempSymbol.setSymbolType(Number);
      tempSymbol.setNumber(randNum+1+(col-1)*3*origsize);
   }   
   return tempSymbol;
}


// int main() { // For testing of implementation
//    BingrowBoard test(5);
 
//    BingrowSquare testsquare;
//    Symbol testsymbol;
//    testsymbol.setSymbolType(Number);
//    testsymbol.setNumber(10);

//    testsquare.setSymbol(testsymbol);
   
//    test.SetSquare(5,3,testsquare);
//    test.SetSquare(4,3,testsquare);
//    test.SetSquare(2,3,testsquare);
//    test.SetSquare(1,3,testsquare); 

//    test.UpdateBoard(testsymbol);
   

//    int elemCount = 0;
//    deque<deque<BingrowSquare>> testboard = test.getBoard();
//    for (int i = 0; i < test.GetSize(); i++) 
//    {
//       for (int j = 0; j < test.GetSize(); j++) 
//       {
//          if (elemCount % test.GetSize() == 0) 
//          {
//             std::cout << '\n';
//          }
//          if (testboard[i][j].getSymbol().getSymbolType() == Number) 
//          {
//             std::cout << testboard[i][j].getSymbol().getNumber() << " ";
//             elemCount++;
//          }
//          else if (testboard[i][j].getSymbol().getSymbolType() == Free) 
//          {
//             std::cout << "FR" << " ";
//             elemCount++;
//          }
//          else if (testboard[i][j].getSymbol().getSymbolType() == Arrow) 
//          {
//             std::cout << "A" << testboard[i][j].getSymbol().getArrowType() << " ";
//             elemCount++;
//          }
//          else 
//          {
//             elemCount++;
//          }
//       }
//    }
//    std::cout << "\nnumber of elements: " << elemCount << std::endl;
//    for (int i = 0; i < test.GetSize(); i++) 
//    {
//       for (int j = 0; j < test.GetSize(); j++) 
//       {            
//          if (testboard[i][j].getCovered() == true && testboard[i][j].getSymbol().getSymbolType() != Free)
//          {
//             if (testboard[i][j].getSymbol().getSymbolType() == Number) 
//             {
//                cout << testboard[i][j].getSymbol().getNumber() << " has been covered." << endl;
//             }
//             else 
//             {
//                cout << "An arrow of type: " << testboard[i][j].getSymbol().getArrowType() << " has been covered." << endl;
//             }
//          }
//       }
//    }
//    if (test.WinningBoard() == true)
//    {
//       cout << "Bingo!" << endl;
//    }
//    return 0;
// } 