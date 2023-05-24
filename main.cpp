#include <iostream>
#include <vector>

class Ship {
protected:
    int size, row, col;
    bool vertical;
public:
    Ship(int size) : size(size), row(0), col(0), vertical(std::rand() % 2) { }

    bool getVertical() const {
        return vertical;
    }
    int getSize() const {
        return size;
    }
    int getRow() const {
        return row;
    }
    int getCol() const {
        return col;
    }
    void setRow(int newRow) {
        row = newRow;
    }
    void setCol(int newCol) {
        col = newCol;
    }
};

class Carrier : public Ship {
public:
    Carrier() : Ship(5) { }
};

class Battleship : public Ship {
public:
    Battleship() : Ship(4) { }
};

class Cruiser : public Ship {
public:
    Cruiser() : Ship(3) { }
};

class Submarine : public Ship {
public:
    Submarine() : Ship(3) { }
};

class Destroyer : public Ship {
public:
    Destroyer() : Ship(2) { }
};

class Board {
protected:
    int rowscols;
    std::vector<std::vector<char> > board, markBoard;
public:
    Board(int rowscols) : rowscols(rowscols + 1), board(rowscols + 1, std::vector<char>(rowscols + 1, '.')), markBoard(rowscols + 1, std::vector<char>(rowscols + 1, '.')) {}

    void displayBoard() const {
        for (int i = 0; i < rowscols; i++) {
            for (int j = 0; j < rowscols; j++) {
                if (i == 0 && j == 0) {
                    std::cout << "  ";
                } else if (i == 0 && j != 0) {
                    std::cout << j << ' ';
                } else if (j == 0 && i != 0) {
                    std::cout << i << ' ';
                } else {
                    std::cout << board[i][j] << ' ';
                }
            }

            std::cout << "   ";

            for (int j = 0; j < rowscols; j++) {
                if (i == 0 && j == 0) {
                    std::cout << "  ";
                } else if (i == 0 && j != 0) {
                    std::cout << j << ' ';
                } else if (j == 0 && i != 0) {
                    std::cout << i << ' ';
                } else {
                    std::cout << markBoard[i][j] << ' ';
                }
            }

            std::cout << '\n';
        }
        std::cout << '\n';
    }

    bool isValidPlacement(const Ship& ship) const {
        int startRow = ship.getRow();
        int startCol = ship.getCol();
        int size = ship.getSize();
        bool vertical = ship.getVertical();

        if (vertical) {
            if (startRow + size > rowscols) {
                return false;
            }
            for (int i = 0; i < size; i++) {
                if (board[startRow + i][startCol] != '.') {
                    return false;
                }
            }
        } else {
            if (startCol + size > rowscols) {
                return false;
            }
            for (int i = 0; i < size; i++) {
                if (board[startRow][startCol + i] != '.') {
                    return false;
                }
            }
        }

        return true;
    }

    void placeShipRandomly(Ship& ship) {
        do {
            ship.setRow(std::rand() % (rowscols - 1) + 1);
            ship.setCol(std::rand() % (rowscols - 1) + 1);
        } while (!isValidPlacement(ship));

        int startRow = ship.getRow();
        int startCol = ship.getCol();
        int size = ship.getSize();
        bool vertical = ship.getVertical();

        for (int i = 0; i < size; i++) {
            if (vertical) {
                board[startRow + i][startCol] = 'S';
            } else {
                board[startRow][startCol + i] = 'S';
            }
        }
    }

    const std::vector<std::vector<char> >& getBoard() const {
        return board;
    }

    const std::vector<std::vector<char> >& getMarkBoard() const {
        return markBoard;
    }

    void setBoard(int row, int col, char symbol) {
        board[row][col] = symbol;
    }

    void setMarkBoard(int row, int col, char symbol) {
        markBoard[row][col] = symbol;
    }

    int getRowsCols() const {
        return rowscols;
    }
};

class Player : public Board {
public:
    Player(int rowscols) : Board(rowscols) {}
};

class Game {
private:
    std::vector<std::unique_ptr<Ship> > playerShips;
    std::vector<std::unique_ptr<Ship> > botShips;
    Player player;
    Player bot;
public:
    Game(int rowscols) : player(rowscols), bot(rowscols) {
        playerShips.push_back(std::make_unique<Carrier>());
        playerShips.push_back(std::make_unique<Battleship>());
        playerShips.push_back(std::make_unique<Cruiser>());
        playerShips.push_back(std::make_unique<Submarine>());
        playerShips.push_back(std::make_unique<Destroyer>());

        botShips.push_back(std::make_unique<Carrier>());
        botShips.push_back(std::make_unique<Battleship>());
        botShips.push_back(std::make_unique<Cruiser>());
        botShips.push_back(std::make_unique<Submarine>());
        botShips.push_back(std::make_unique<Destroyer>());

        for (auto& ship : playerShips) {
            player.placeShipRandomly(*ship);
        }
        for (auto& ship : botShips) {
            bot.placeShipRandomly(*ship);
        }
    }

    void displayBoard() const {
        system("clear");
        player.displayBoard();
    }

    void play() {
        displayBoard();
        
        while (true) {
            const std::vector<std::vector<char> >& botBoard = bot.getBoard();
            const std::vector<std::vector<char> >& playerBoard = player.getBoard();
            const std::vector<std::vector<char> >& markBoard = player.getMarkBoard();
            int boardSize = player.getRowsCols();
            int row, col;

            if (end(botBoard, boardSize)) {
                system("clear");
                player.displayBoard();
                std::cout << "Player wins\n";
                break;
            } else if (end(playerBoard, boardSize)) {
                system("clear");
                player.displayBoard();
                std::cout << '\n' << "Bot wins\n";
                break;
            }

            std::cout << "Enter target coordinates (x, y): ";
            std::cin >> row >> col;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (row <= 0 || row >= boardSize || col <= 0 || col >= boardSize) {
                displayBoard();
                std::cout << "Invalid target coordinates. Try again.\n";
                continue;
            }

            if (botBoard[row][col] == 'H' || botBoard[row][col] == 'M') {
                displayBoard();
                std::cout << "Target already hit. Try again.\n";
                continue;
            } else if (botBoard[row][col] == 'S') {
                bot.setBoard(row, col, 'H');
                player.setMarkBoard(row, col, 'H');
                displayBoard();
                std::cout << "Bot ship hit.\n";
            } else {
                bot.setBoard(row, col, 'M');
                player.setMarkBoard(row, col, 'M');
                displayBoard();
                std::cout << "Bot ship missed.\n";
            }

            while (true) {
                row = std::rand() % (boardSize - 1) + 1;
                col = std::rand() % (boardSize - 1) + 1;

                if (playerBoard[row][col] == 'H' || playerBoard[row][col] == 'M') {
                    continue;
                } else if (playerBoard[row][col] == 'S') {
                    player.setBoard(row, col, 'H');
                    break;
                } else {
                    player.setBoard(row, col, 'M');
                    break;
                }
            }
        }
    }

    bool end(const std::vector<std::vector<char> >& board, int boardSize) const {
        for (const auto& row : board) {
            for (const auto& cell : row) {
                if (cell == 'S') {
                    return false;
                }
            }
        }
        return true;
    }
};

int main() {
    system("clear");
    int boardSize;

    while (true) {
        try {
            std::cout << "Enter board size (6x6 - 9x9): ";
            std::cin >> boardSize;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (boardSize < 6 || boardSize > 9) {
                throw boardSize;
            } else {
                break;
            }
        } catch (int exception) {
            std::cout << "Caught exception, Invalid board size, entered: " << exception << '\n';
        }
    }

    srand(static_cast<unsigned int>(time(nullptr)));
    Game game(boardSize);
    game.play();

    return 0;
}