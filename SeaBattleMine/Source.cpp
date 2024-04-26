#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SIZE = 10;
const int SHIPS = 10;

const int ShipSize[SHIPS] = { 4, 3, 3, 2, 2, 2, 1, 1 ,1, 1 }; // All ship size;

enum class CellState
{
    Empty,
    Ship,
    Miss,
    Hit,
    DeadZone
};

struct Cell
{
    CellState state;
    bool revealed;
};

typedef vector<vector<Cell>> Board;

void printBoard(const Board& board, bool revealShips)
{
    cout << "  ";
    for (int i = 0; i < SIZE; ++i)
    {
        cout << i << " ";
    }
    cout << endl;

    for (int i = 0; i < SIZE; ++i)
    {
        cout << i << " ";
        for (int j = 0; j < SIZE; ++j)
        {
            if (board[i][j].revealed || revealShips)
            {
                switch (board[i][j].state)
                {
                case CellState::Empty:
                    cout << "- ";
                    break;
                case CellState::Ship:
                    cout << "S ";
                    break;
                case CellState::Miss:
                    cout << "M ";
                    break;
                case CellState::Hit:
                    cout << "X ";
                    break;
                }
            }
            else {
                cout << "- ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void printBoardForBoth(const Board& boardPlayer, const Board& boardComp, bool revealShipsPlayer, bool revealShipsComp, bool boardNamePlayer)
{
    cout << endl;
    if (boardNamePlayer)
        cout << "  PlayerBoard\t\t  CompBoard" << endl;
    else
        cout << "  CompBoard No.1\t  CompBoard No.2" << endl;

    cout << "  ";
    for (int i = 0; i < SIZE; ++i)
    {
        cout << i << " ";
    }
    cout << "\t" << "  ";
    for (int i = 0; i < SIZE; ++i)
    {
        cout << i << " ";
    }
    cout << endl;

    for (int i = 0; i < SIZE; ++i)
    {
        cout << i << " ";
        for (int j = 0; j < SIZE; ++j)
        {
            if (boardPlayer[i][j].revealed || revealShipsPlayer)
            {
                switch (boardPlayer[i][j].state)
                {
                case CellState::Empty:
                    cout << "- ";
                    break;
                case CellState::Ship:
                    cout << "S ";
                    break;
                case CellState::Miss:
                    cout << "M ";
                    break;
                case CellState::Hit:
                    cout << "X ";
                    break;
                case CellState::DeadZone:
                    cout << "# ";
                    break;
                }
            }
            else
            {
                cout << "- ";
            }
        }
        cout << "\t" << i << " ";
        for (int j = 0; j < SIZE; ++j)
        {
            if (boardComp[i][j].revealed || revealShipsComp)
            {
                switch (boardComp[i][j].state)
                {
                case CellState::Empty:
                    cout << "- ";
                    break;
                case CellState::Ship:
                    cout << "S ";
                    break;
                case CellState::Miss:
                    cout << "M ";
                    break;
                case CellState::Hit:
                    cout << "X ";
                    break;
                case CellState::DeadZone:
                    cout << "# ";
                    break;
                }
            }
            else
            {
                cout << "- ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void initializeBoard(Board& board)
{
    board.clear();
    board.resize(SIZE, vector<Cell>(SIZE, { CellState::Empty, false }));
}

bool isValidPlacement(const Board& board, int row, int col, int size, bool vertical)
{
    if (vertical)
    {
        if (row + size > SIZE) // Проверка на вместимость
            return false;
        for (int i = row; i < row + size; ++i) // Проверка на серединку
            if (board[i][col].state != CellState::Empty)
                return false;
        if (row != 0) // Проверка сверху при row != 0 (Проверка слева сверху)
            if (board[row - 1][col].state != CellState::Empty)
                return false;
        if (col != 0) // Проверка сверху при col != 0 (Проверка слева)
            if (board[row][col - 1].state != CellState::Empty)
                return false;
        if (row != 0 && col != 0) // Проверка сверху с учетом выхода за левую границу
            if (board[row - 1][col - 1].state != CellState::Empty)
                return false;
        if (row != 0 && col + 1 != SIZE)
            if (board[row - 1][col + 1].state != CellState::Empty)
                return false;
        if (col != 0) // Проверка левой границы
            for (int i = row; i < row + size; ++i)
                if (board[i][col - 1].state != CellState::Empty)
                    return false;
        if (col + 1 != SIZE) // Проверка правой границы
            for (int i = row; i < row + size; ++i)
                if (board[i][col + 1].state != CellState::Empty)
                    return false;
        if (row + size != SIZE)
        {
            if (col + 1 != SIZE) //Проверка снизу при col + 1 != SIZE
                if (board[row + size][col + 1].state != CellState::Empty)
                    return false;
            if (row + 1 != SIZE) //Проверка снизу при row + 1 != SIZE
                if (board[row + size][col].state != CellState::Empty)
                    return false;
            if (col != 0) // Проверка снизу с учетом выхода за границу SIZE
                if (board[row + size][col - 1].state != CellState::Empty)
                    return false;
        }
    }
    else
    {
        if (col + size > SIZE) // Проверка на вместимость
            return false;
        for (int i = col; i < col + size; ++i) // Проверка на серединку
            if (board[row][i].state != CellState::Empty)
                return false;
        if (row != 0 && col != 0) //Проверка слева с учетом выхода за левую границу
            if (board[row - 1][col - 1].state != CellState::Empty)
                return false;
        if (row != 0) // Проверка сверху при row != 0 (левое верхнее)
            if (board[row - 1][col].state != CellState::Empty)
                return false;
        if (col != 0) // Проверка сверху при col != 0 (крайнее левое)
            if (board[row][col - 1].state != CellState::Empty)
                return false;
        if (row + 1 != SIZE && col != 0) // Проверка сверху левого нижнего
            if (board[row + 1][col - 1].state != CellState::Empty)
                return false;
        if (row != 0) // Проверка по верху
            for (int i = col; i < col + size; ++i)
                if (board[row - 1][i].state != CellState::Empty)
                    return false;
        if (row + 1 != SIZE) //Проверка по низу
            for (int i = col; i < col + size; ++i)
                if (board[row + 1][i].state != CellState::Empty)
                    return false;
        if (col + size != SIZE)
        {
            if (row + 1 != SIZE) // Проверка при row + 1 != SIZE (Правое крайнее нижнее)
                if (board[row + 1][col + size].state != CellState::Empty)
                    return false;
            if (col + 1 != SIZE) // Проверка при col + 1 != SIZE (Правое крайнее)
                if (board[row][col + size].state != CellState::Empty)
                    return false;
            if (row != 0) // Проверка правого крайнего верхнего
                if (board[row - 1][col + size].state != CellState::Empty)
                    return false;
            if (row + 1 != SIZE && row != 0) // Проверка справа с учетом выхода за границы SIZE
                if (board[row - 1][col + size].state != CellState::Empty)
                    return false;
        }
    }
    return true;
}

bool isValidMove(const Board& board, int row, int col)
{
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE && !board[row][col].revealed;
}

void placeShip(Board& board, int row, int col, int size, bool vertical) {
    if (vertical) {
        for (int i = row; i < row + size; ++i) {
            board[i][col].state = CellState::Ship;
        }
    }
    else {
        for (int j = col; j < col + size; ++j) {
            board[row][j].state = CellState::Ship;
        }
    }
}

void randomlyPlaceShips(Board& board)
{
    int shipsPlaced = 0;
    while (shipsPlaced < SHIPS)
    {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        bool vertical = rand() % 2 == 0;

        if (isValidPlacement(board, row, col, ShipSize[shipsPlaced], vertical))
        {
            placeShip(board, row, col, ShipSize[shipsPlaced], vertical);
            shipsPlaced++;
        }
    }
}

void manuallyPlaceShips(Board& board)
{
    int shipsPlaced = 0;
    while (shipsPlaced < SHIPS)
    {
        printBoard(board, true);
        cout << "Enter ship #" << shipsPlaced + 1 << " (Size = " << ShipSize[shipsPlaced] << ") location and orientation (row col vertical): ";
        int row, col, vertical;
        cin >> row >> col >> vertical;

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || (vertical != 0 && vertical != 1))
        {
            cout << "Invalid input. Try again." << endl;
            continue;
        }

        if (isValidPlacement(board, row, col, ShipSize[shipsPlaced], vertical))
        {
            placeShip(board, row, col, ShipSize[shipsPlaced], vertical);
            shipsPlaced++;
        }
        else
        {
            cout << "Invalid placement. Try again." << endl;
        }
    }
}

bool isGameOver(const Board& board)
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (board[i][j].state == CellState::Ship)
            {
                return false;
            }
        }
    }
    return true;
}

void boardChangeIfDestroyed(Board& board, int row, int col, int size, bool vertical)
{

    if (vertical)
    {
        if (row != 0)  //Проверка сверху
        {
            board[row - 1][col].state = CellState::DeadZone;
            board[row - 1][col].revealed = true;
        }
        if (col != 0) //Проверка слева
        {
            for (int i = row; i < row + size; i++) // боковушки слева 
            {
                board[i][col - 1].state = CellState::DeadZone;
                board[i][col - 1].revealed = true;
            }
        }
        if (row != 0 && col != 0) //Проверка слева, сверху
        {
            board[row - 1][col - 1].state = CellState::DeadZone;
            board[row - 1][col - 1].revealed = true;
        }
        if (row != 0 && col + 1 != SIZE) //Проверка сверху и справа
        {
            board[row - 1][col + 1].state = CellState::DeadZone;
            board[row - 1][col + 1].revealed = true;
        }
        if (col + 1 != SIZE) // Проверка правой боковушки
            for (int i = row; i < row + size; i++)
            {
                board[i][col + 1].state = CellState::DeadZone;
                board[i][col + 1].revealed = true;
            }
        if (row + size != SIZE) // Проверки нижних границ
        {
            if (col + 1 != SIZE)
            {
                board[row + size][col + 1].state = CellState::DeadZone;
                board[row + size][col + 1].revealed = true;
            }
            if (row + 1 != SIZE)
            {
                board[row + size][col].state = CellState::DeadZone;
                board[row + size][col].revealed = true;
            }
            if (col != 0)
            {
                board[row + size][col - 1].state = CellState::DeadZone;
                board[row + size][col - 1].revealed = true;
            }
        }
    }
    else
    {
        if (row != 0 && col != 0) // Проверка слева сверху
        {
            board[row - 1][col - 1].state = CellState::DeadZone;
            board[row - 1][col - 1].revealed = true;
        }
        if (row != 0) // Проверка по верху
        {
            board[row - 1][col].state = CellState::DeadZone;
            board[row - 1][col].revealed = true;
            for (int i = col; i < col + size; i++)
            {
                board[row - 1][i].state = CellState::DeadZone;
                board[row - 1][i].revealed = true;
            }
        }
        if (col != 0)
        {
            board[row][col - 1].state = CellState::DeadZone;
            board[row][col - 1].revealed = true;
        }
        if (row + 1 != SIZE && col != 0)
        {
            board[row + 1][col - 1].state = CellState::DeadZone;
            board[row + 1][col - 1].revealed = true;
        }
        if (row + 1 != SIZE) // Проверка по низу
        {
            for (int i = col; i < col + size; i++)
            {
                board[row + 1][i].state = CellState::DeadZone;
                board[row + 1][i].revealed = true;
            }
        }
        if (col + size != SIZE)
        {
            if (row + 1 != SIZE)
            {
                board[row + 1][col + size].state = CellState::DeadZone;
                board[row + 1][col + size].revealed = true;
            }
            if (col + 1 != SIZE)
            {
                board[row][col + size].state = CellState::DeadZone;
                board[row][col + size].revealed = true;
            }
            if (row != 0)
            {
                board[row - 1][col + size].state = CellState::DeadZone;
                board[row - 1][col + size].revealed = true;
            }
        }
    }
}

void checkShipHealth(Board& board, int row, int col)
{
    bool vertical = true;
    int size = 1;
    if (row != 0) // Проверка верхнего значения коробля
        if (board[row - 1][col].state == CellState::Ship || board[row - 1][col].state == CellState::Hit)
            vertical = true;
    if (row + 1 != SIZE)
        if (board[row + 1][col].state == CellState::Ship || board[row + 1][col].state == CellState::Hit)
            vertical = true;
    if (col != 0) // Проверка левого значения корабля
        if (board[row][col - 1].state == CellState::Ship || board[row][col - 1].state == CellState::Hit)
            vertical = false;
    if (col + 1 != SIZE)
        if (board[row][col + 1].state == CellState::Ship || board[row][col + 1].state == CellState::Hit)
            vertical = false;


    if (vertical)
    {
        while (row != 0)
            if (board[row - 1][col].state == CellState::Ship || board[row - 1][col].state == CellState::Hit)
                row--;
            else
                break;
        int rowMax = row;
        while (row != SIZE - 1)
            if (board[row + 1][col].state == CellState::Ship || board[row + 1][col].state == CellState::Hit)
            {
                row++;
                size++;
            }
            else
                break;

        for (int i = rowMax; i < rowMax + size; i++)
            if (board[i][col].state != CellState::Hit)
                return;
        cout << "Ship is down" << endl;
        boardChangeIfDestroyed(board, rowMax, col, size, vertical);
    }
    else
    {
        while (col != 0)
            if (board[row][col - 1].state == CellState::Ship || board[row][col - 1].state == CellState::Hit)
                col--;
            else
                break;
        int colMax = col;
        while (col != SIZE - 1)
            if (board[row][col + 1].state == CellState::Ship || board[row][col + 1].state == CellState::Hit)
            {
                col++;
                size++;
            }
            else
                break;
        for (int i = colMax; i < colMax + size; i++)
            if (board[row][i].state != CellState::Hit) // Заменить границы убитых кораблей
                return;
        cout << "Ship is down" << endl;
        boardChangeIfDestroyed(board, row, colMax, size, vertical);
    }
}

void smartPC(Board& enemyBoard, int& row, int& col)
{
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (enemyBoard[i][j].state == CellState::Hit) //Находим что будем чекать
            {
                if (i != 0 && !enemyBoard[i - 1][j].revealed)
                {
                    row = i-1; col = j;
                    return;
                }
                else if (j != 0 && !enemyBoard[i][j - 1].revealed)
                {
                    row = i; col = j - 1;
                    return;
                }
                else if (i != SIZE - 1 && !enemyBoard[i + 1][j].revealed)
                {
                    row = i + 1; col = j;
                    return;
                }
                else if (j != SIZE - 1 && !enemyBoard[i][j + 1].revealed)
                {
                    row = i; col = j + 1;
                    return;
                }
            }
    return;
}

void playerTurn(Board& playerBoard, Board& computerBoard, bool superAI, bool boardNamePlayer)
{
    bool flagPlayer = false;
    do
    {
        cout << endl;
        printBoardForBoth(playerBoard, computerBoard, true, false, boardNamePlayer);
        cout << "Enter your move (row col): ";
        int row, col;
        cin >> row >> col;
        if (!isValidMove(computerBoard, row, col))
        {
            cout << "Invalid move. Try again." << endl;
            playerTurn(playerBoard, computerBoard, superAI, boardNamePlayer);
        }
        switch (computerBoard[row][col].state)
        {
        case CellState::Empty:
            cout << "You missed" << endl;
            computerBoard[row][col].state = CellState::Miss;
            flagPlayer = false;
            break;
        case CellState::Ship:
            cout << "You hit a ship" << endl;
            computerBoard[row][col].state = CellState::Hit;
            flagPlayer = true;
            break;
        }
        computerBoard[row][col].revealed = true;
        if (flagPlayer)
            checkShipHealth(computerBoard, row, col);

        if (isGameOver(computerBoard))
        {
            cout << "Congratulations! You sank all the enemy ships. You win!" << endl;
            return;
        }
    } while (flagPlayer);

    bool flagComp = false;
    do
    {
        int computerRow, computerCol;
        do
        {
            computerRow = rand() % SIZE;
            computerCol = rand() % SIZE;
        } while (!isValidMove(playerBoard, computerRow, computerCol));
        if (superAI)
            smartPC(playerBoard, computerRow, computerCol); // Включили ИИ лол

        cout << "Computer's turn:";
        switch (playerBoard[computerRow][computerCol].state)
        {
        case CellState::Empty:
            cout << " missed";
            playerBoard[computerRow][computerCol].state = CellState::Miss;
            flagComp = false;
            break;
        case CellState::Ship:
            cout << " hit your ship";
            playerBoard[computerRow][computerCol].state = CellState::Hit;
            flagComp = true;
            break;
        }
        cout << " (x = " << computerRow << ", y = " << computerCol << ')' << endl;
        if (flagComp)
            checkShipHealth(playerBoard, computerRow, computerCol);

        if (isGameOver(playerBoard))
        {
            printBoardForBoth(playerBoard, computerBoard, false, false, boardNamePlayer);
            cout << "Congratulations! Computer win!" << endl;
            return;
        }
    } while (flagComp);

    int choice;
    static bool NeverAsk = true;

    while (NeverAsk)
    {
        printBoardForBoth(playerBoard, computerBoard, true, false, boardNamePlayer);
        cout << "Do you wanna continue? 0. Yes, never ask again, 1. Yes, 2. No, let me out" << endl;
        cout << "Make a choice => ";
        cin >> choice;
        if (choice == 0)
            NeverAsk = false;
        if (choice == 1)
            break;
        if (choice == 2)
            return;
        else
            cout << "Invalid input, try again" << endl;
    }

    playerTurn(playerBoard, computerBoard, superAI, boardNamePlayer);
} // Игрок против ПК

void CompTurn(Board& playerBoard, Board& computerBoard, bool superAI, bool boardNamePlayer)
{
    bool flagPlayer = false;
    do
    {
        printBoardForBoth(playerBoard, computerBoard, false, false, boardNamePlayer);
        int row, col;
        do
        {
            row = rand() % SIZE;
            col = rand() % SIZE;
        } while (!isValidMove(computerBoard, row, col));
        if (superAI)
        smartPC(computerBoard, row, col); // Включили ИИ лол

        cout << "Computer No.1 turn:";

        switch (computerBoard[row][col].state)
        {
        case CellState::Empty:
            cout << " missed" << endl;
            computerBoard[row][col].state = CellState::Miss;
            flagPlayer = false;
            break;
        case CellState::Ship:
            cout << " hit ship" << endl;
            computerBoard[row][col].state = CellState::Hit;

            flagPlayer = true;
            break;
        }
        computerBoard[row][col].revealed = true;
        cout << " (x = " << row << ", y = " << col << ')' << endl;
        if (flagPlayer)
            checkShipHealth(computerBoard, row, col);

        if (isGameOver(computerBoard))
        {
            printBoardForBoth(playerBoard, computerBoard, false, false, boardNamePlayer);
            cout << "Congratulations! Computer No.1 win!" << endl;
            return;
        }
    } while (flagPlayer);

    bool flagComp = false;
    do
    {
        printBoardForBoth(playerBoard, computerBoard, false, false, boardNamePlayer);
        int computerRow, computerCol;
        do
        {
            computerRow = rand() % SIZE;
            computerCol = rand() % SIZE;
        } while (!isValidMove(playerBoard, computerRow, computerCol));
        if (superAI)
            smartPC(playerBoard, computerRow, computerCol); // Включили ИИ лол

        cout << "Computer No.2 turn:";
        switch (playerBoard[computerRow][computerCol].state)
        {
        case CellState::Empty:
            cout << " missed" << endl;
            playerBoard[computerRow][computerCol].state = CellState::Miss;
            flagComp = false;
            break;
        case CellState::Ship:
            cout << " hit ship" << endl;
            playerBoard[computerRow][computerCol].state = CellState::Hit;
            flagComp = true;
            break;
        }
        playerBoard[computerRow][computerCol].revealed = true;

        cout << " (x = " << computerRow << ", y = " << computerCol << ')' << endl;
        if (flagComp)
            checkShipHealth(playerBoard, computerRow, computerCol);

        if (isGameOver(playerBoard))
        {
            printBoardForBoth(playerBoard, computerBoard, false, false, boardNamePlayer);
            cout << "Congratulations! Computer No.2 win!" << endl;
            return;
        }
    } while (flagComp);

    CompTurn(playerBoard, computerBoard, superAI, boardNamePlayer);
} // ПК против ПК



void main()
{
    srand(time(0));
    while (true)
    {
        Board playerBoard;
        Board computerBoard;

        cout << "Hello, this is sea battle" << endl;
        cout << "We have 2 mods: \n0. Player vs PC \n1. PC vs PC" << endl;
        int mod;
        while (true)
        {
            cout << "Mod selection => ";
            cin >> mod;
            if (mod > 1)
                cout << "Invalid input, try again" << endl;
            else
                break;
        }
        int placing;
        cout << "We can place ship's by random or manual:" << endl;
        cout << "0. By random \n1. By yourself" << endl;
        while (true)
        {
            cout << "Placing selection => ";
            cin >> placing;
            if (placing > 1)
                cout << "Invalid input, try again" << endl;
            else
                break;
        }
        int statAI;
        cout << "Turn in our super AI?" << endl;
        cout << "0. YES!!! 0_o \n1. no :(" << endl;
        while (true)
        {
            cout << "AI selection => ";
            cin >> statAI;
            if (statAI > 1)
                cout << "Invalid input, try again" << endl;
            else
                break;
        }
        cout << "Almost ready, press any key to start the game" << endl;
        system("pause");

        initializeBoard(playerBoard);
        initializeBoard(computerBoard);

        if (placing == 0)
        {
            randomlyPlaceShips(playerBoard);
            randomlyPlaceShips(computerBoard);
        }
        else
        {
            manuallyPlaceShips(playerBoard);
            randomlyPlaceShips(computerBoard);
        }
        if (mod == 0 && statAI == 0) // ПК против игрока с ИИ
            playerTurn(playerBoard, computerBoard, true, true);
        else if (mod == 0 && statAI == 1) // ПК против игрока без ИИ
            playerTurn(playerBoard, computerBoard, false, true);
        else if (mod == 1 && statAI == 0) // ПК против ПК с ИИ
        {
            CompTurn(playerBoard, computerBoard, true, false);
        }
        else // ПК против ПК без ИИ
        {
            CompTurn(playerBoard, computerBoard, false, false);
        }

        cout << "I hope you have a great time, do you wanna restart? \n0.Yes\n1.No" << endl;
        int end;
        while (true)
        {
            cout << "Restart => ";
            cin >> end;
            if (end > 1)
                cout << "Invalid input, try again" << endl;
            else
                break;
        }
        if (end == 1)
        {
            cout << "Goodbye, world!";
            return;
        }
    }
    //randomlyPlaceShips(playerBoard);

    //randomlyPlaceShips(computerBoard);

    //CompTurn(playerBoard, computerBoard, true);
    system("pause");
}