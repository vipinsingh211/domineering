#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

bool turn;
int rows, columns;

void save_to_file(char *board, char *file_name)
{
    std::ofstream save_game;
    save_game.open(file_name);
    if (save_game.is_open())
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                save_game << *((board + i * columns) + j);
                if (j < columns - 1)
                {
                    save_game << ",";
                }
            }
            if (i < rows - 1)
            {
                save_game << "\n";
            }
        }
    }
    else
    {
        std::cout << "ERROR: while opening the file: " << file_name << std::endl;
        exit(2);
    }
    save_game.close();

    std::ofstream save_next_turn;
    std::strcat(file_name, "_next");
    save_next_turn.open(file_name);
    if (save_next_turn.is_open())
    {
        save_next_turn << turn;
    }
    else
    {
        std::cout << "ERROR: while opening the file: " << file_name << std::endl;
        exit(2);
    }
    save_next_turn.close();
}

void size_of_board(char *file_name)
{
    std::string line;
    std::ifstream load_game_meta(file_name);
    if (load_game_meta.is_open())
    {
        while (std::getline(load_game_meta, line))
        {
            rows += 1;
        }
        std::string value;
        std::stringstream line_stream(line);
        while (getline(line_stream, value, ','))
        {
            columns += 1;
        }
        load_game_meta.close();
    }
    else
    {
        std::cout << "ERROR: while opening the file." << file_name << std::endl;
        exit(1);
    }
}

void load_game_from_file(char *board, char *file_name)
{
    std::string line;
    std::ifstream load_game(file_name);
    if (load_game.is_open())
    {
        int j = 0;
        while (std::getline(load_game, line))
        {
            std::string value;
            std::stringstream line_stream(line);
            while (getline(line_stream, value, ','))
            {
                // std::cout << j << ":" << value.c_str()[0];
                *(board + j) = value.c_str()[0];
                j += 1;
            }
        }
        load_game.close();
    }
    else
    {
        std::cout << "ERROR: while opening the file." << file_name << std::endl;
        exit(1);
    }
}

void load_next_turn(char *file_name)
{
    std::string line;
std:
    strcat(file_name, "_next");
    std::ifstream load_turn(file_name);
    if (load_turn.is_open())
    {
        std::getline(load_turn, line);
        turn = line == "0" ? false : true;
    }
    else
    {
        std::cout << "ERROR: while opening the file." << file_name << std::endl;
        exit(1);
    }
}

void if_quitting(char input, char *board = NULL)
{
    if (input == 'q' || input == 'Q')
    {
        if (board)
        {
            char save_flag[2];
            while (true)
            {
                std::cout << "Do you want to save the game? (y/n): ";
                std::cin >> save_flag;
                if (save_flag[0] == 'y')
                {
                    char *file_name = new char[50];
                    std::cout << "Enter a file name: ";
                    std::cin >> file_name;
                    std::cout << "File name: " << file_name << std::endl;
                    save_to_file(board, file_name);
                    break;
                }
                else if (save_flag[0] == 'n')
                {
                    break;
                }
                else
                {
                    std::cout << "ERROR: Invalid selection." << std::endl;
                    continue;
                }
            }
        }

        exit(0);
    }
}

void display_board(char *board)
{
    std::cout << std::endl;

    char space = '\0';
    printf("%3s", &space);
    for (int c = 0; c < columns; c++)
    {
        printf("%4d", c);
    }
    std::cout << std::endl;

    for (int i = 0; i < rows; i++)
    {
        printf("%3d", i);
        for (int j = 0; j < columns; j++)
        {
            std::cout << " | " << *((board + i * columns) + j);
        }
        std::cout << " | " << std::endl;
    }
    std::cout << std::endl;
}

void init_board(char *board)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            *((board + i * columns) + j) = ' ';
        }
    }
}

int *ask_turn(int *selection, char *board)
{
    char c_selection_1[2], c_selection_2[2];
    int row_limit = rows - 1;
    int column_limit = columns - 1;
    if (turn == true)
    {
        row_limit = rows - 2;
    }
    if (turn == false)
    {
        column_limit = columns - 2;
    }

    while (true)
    {
        if (turn == true)
        {
            std::cout << "Player V's turn";
        }
        else
        {
            std::cout << "Player H's turn";
        }

        std::cout << ", select row and column" << std::endl;
        std::cout << "Row (range 0 - " << row_limit << ") OR 'q|Q' to quit: ";
        std::cin >> c_selection_1;
        if_quitting(c_selection_1[0], board);
        selection[0] = atoi(c_selection_1);
        if (selection[0] < 0 || selection[0] > row_limit)
        {
            std::cout << "ERROR: Out of range 'Row' value." << std::endl;
            continue;
        }

        std::cout << "Column (range 0 - " << column_limit << ") OR 'q|Q' to quit: ";
        std::cin >> c_selection_2;
        if_quitting(c_selection_2[0], board);
        selection[1] = atoi(c_selection_2);
        if (selection[1] < 0 || selection[1] > column_limit)
        {
            std::cout << "ERROR: Out of range 'Column' value." << std::endl;
            continue;
        }
        break;
    }

    return selection;
}

void fill_board(int selection[2], char *board)
{
    if (turn == true)
    {
        *((board + selection[0] * columns) + selection[1]) = 'V';
        *((board + (selection[0] + 1) * columns) + selection[1]) = 'V';
        turn = false;
    }
    else
    {
        *((board + selection[0] * columns) + selection[1]) = 'H';
        *((board + selection[0] * columns) + selection[1] + 1) = 'H';
        turn = true;
    }
}

bool verify_selection(int selection[2], char *board)
{
    if (turn == true)
    {
        if (*((board + selection[0] * columns) + selection[1]) == ' ' &&
            *((board + (selection[0] + 1) * columns) + selection[1]) == ' ')
        {
            return true;
        }
    }
    if (turn == false)
    {
        if (*((board + selection[0] * columns) + selection[1]) == ' ' &&
            *((board + selection[0] * columns) + selection[1] + 1) == ' ')
        {
            return true;
        }
    }
    return false;
}

bool is_game_over(char *board)
{
    bool stop_flag = true;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (turn == true)
            {
                if (i + 1 == rows)
                {
                    break;
                }

                if (*((board + i * columns) + j) == ' ' &&
                    *((board + (i + 1) * columns) + j) == ' ')
                {
                    stop_flag = false;
                    break;
                }
            }
            else
            {
                if (j + 1 == columns)
                {
                    break;
                }
                if (*((board + i * columns) + j) == ' ' &&
                    *((board + i * columns) + j + 1) == ' ')
                {
                    stop_flag = false;
                    break;
                }
            }
        }
        if (stop_flag == false)
        {
            break;
        }
    }

    return stop_flag;
}

int main()
{
    int selection[2];

    char c_rows[2], c_columns[2];
    char *board;

    bool load_flag = false;
    while (true)
    {
        std::cout << "Enter the board size (range 2 - 15) OR 'q|Q' to quit OR 'l|L' to load the game from a file: ";
        std::cin >> c_rows;
        if_quitting(c_rows[0]);

        if (c_rows[0] == 'l' || c_rows[0] == 'L')
        {
            char *file_name = new char[50];
            std::cout << "Enter the file name: ";
            std::cin >> file_name;
            size_of_board(file_name);
            // std::cout << "Rows: " << rows << "; Cols: " << columns << std::endl;
            board = (char *)malloc(rows * columns * sizeof(char));
            load_game_from_file(&board[0], file_name);
            load_next_turn(file_name);
            std::cout << turn << std::endl;
            load_flag = true;
            break;
        }
        else
        {
            c_columns[0] = c_rows[0];
            c_columns[1] = c_rows[1];
            rows = atoi(c_rows);
            columns = atoi(c_columns);
            if (rows < 2 || rows > 15)
            {
                std::cout << "ERROR: Out of range board size value." << std::endl;
                continue;
            }
            board = (char *)malloc(rows * columns * sizeof(char));
            init_board(&board[0]);
            break;
        }
    }

    char first_player;
    if (load_flag == false)
    {
        while (true)
        {
            std::cout << "Select who is going to have first turn? (H or V): ";
            std::cin >> first_player;
            if (first_player != 'H' && first_player != 'V')
            {
                std::cout << "ERROR: Must select a valid player (options 'H' and 'V')." << std::endl;
                continue;
            }
            if (first_player == 'V')
            {
                turn = true;
            }
            else
            {
                turn = false;
            }
            break;
        }
    }

    display_board(&board[0]);
    while (true)
    {
        if (is_game_over(&board[0]))
        {
            std::cout << "!!GAME OVER!!" << std::endl;
            if (turn == true)
            {
                std::cout << "** H is the WINNER **" << std::endl;
            }
            else
            {
                std::cout << "** V is the WINNER **" << std::endl;
            }
            break;
        }
        ask_turn(selection, &board[0]);
        if (!verify_selection(selection, &board[0]))
        {
            std::cout << "ERROR: Illegal selection! Try again!" << std::endl;
            continue;
        }
        fill_board(selection, &board[0]);
        display_board(&board[0]);
    }

    free(board);

    return 0;
}