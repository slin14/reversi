#include <iostream>
#include "cmpt_error.h"
#include <vector>

using namespace std;

struct Location {
	int row;
	int column;

	// converting board coordinate
	// to the actual index in 2D vector
	Location(int n, int c);

	Location(char n, char c);
}; // struct Location

// overwrite operator << to print location to cout
ostream& operator<<(ostream& os, const Location& l);

// overwrite operator == to compare if 2 locations are the same
bool operator==(const Location& a, const Location& b);

// Global variables of letters indicates what is in one specific location
namespace Grid{
	const char human = 'x';
	const char computer = 'o';
	const char empty = '.';
}

class Board {
private:
	// a board is two-dimensional vector
	vector<vector<char>> board;

	// record the number of grids the
	// human and computer players occupy
	int x_count;
	int o_count;
public:
	// make a board that has a dimension of l x l
	Board(int l);

	// returns the length of the board vector
	int length() const;

	// returns the count of all x game pieces on the board
	int get_x() const;

	// returns the count of all o game pieces on the board
	int get_o() const;

	// returns the specified location on the board vector
	char get_loc(Location& l) const;

	// prints the board to cout
	void print_board() const;

	// prints the number of pieces the
// human and computer players have on the board
	void print_state() const;

	// prints the board and its state
	void print() const;

	// Pre-condition: the location is valid to place a piece
	// and char g == human || char g == computer
	// Post-condition: place a piece for the player or the computer
	// and update the pieces count variables
	void set(Location& l, const char& g);

	// flip the pieces that can be turned over
	// by the other player's pieces
	void flip(Location& l, const char& g);

	// Pre-condition: l is a valid location to place a piece, one of hr and vt is non-zero
	// hr moves the piece to left or right; vt moves the piece up or down
	// Post-condition: the pieces that can be flipped by the player in one of the eight 
	// directions are flipped and the numbers of pieces flipped are returned
	int flip(Location& l, const char& g, int hr, int vt);

	// true if the player makes a valid move
	// false otherwise
	bool legal(Location& l, const char& g) const;

	// Pre-condition: one of hr and vt is non-zero
	// hr moves the piece to left or right; vt moves the piece up or down
	// Post-condition: return true if location l is a valid position to place, false otherwise
	bool legal(Location& l, const char& g, int hr, int vt) const ;
    
	// reset the board to its starting position
	void reset();

	// returns a location on the board vector
	char get(int r, int c) const;

}; // class Board

class Player {
public:
	
	// show the variable representing the player’s location
	virtual char show() const = 0;

	// return the location of the next piece
	// that the player is going to place
	virtual void move(Board& b) = 0;

	// stores all the legal moves into the next moves vector for the player
	virtual void hint(Board& b) = 0;

	// true if one of the player wins or
	// a tie happens
	// false if the winner is not determined yet
	virtual bool end() const = 0;

	// virtual destructor
	virtual ~Player() {}
}; //class Player

class Human_player : public Player {
private:
	vector<Location> next_move;
public:  
	Human_player(Board& b);

	// returns the variable representing the human player's location
	virtual char show() const {return Grid::human;}

	// allows the human player to move to a legal location on the board
	virtual void move(Board& b);

	// returns hints of legal next moves in a vector
	virtual void hint(Board& b);

	// prints hints the cout
	void show_hint() const;

	// returns true if player has no moves left, false otherwise
	virtual bool end() const;

	// returns the number of moves in the next_move vector
	int get_size() const;

}; //class Human_player

class Computer_player : public Player {
private:
	vector<Location> next_move;
public:    
	Computer_player(Board& b);

	// returns the variable representing the computer player's location
	virtual char show() const {return Grid::computer;}

	// allows the computer player to move to a legal location on the board
	virtual void move(Board& b);

	// returns hints of legal next moves in a vector
	virtual void hint(Board& b);

	// returns true if player has no moves left, false otherwise
	virtual bool end() const;

	// calculate the best position for the next move
	// Location best(Board& b);
	bool can_get_corner() const;

	// Pre-condition:
	// can_get_corner() is true
	Location get_corner() const;

	// return the total pieces that can be flipped
	int flip_count(const Location& l, const Board& b) const;

	// flip the pieces that can be flipped
	int flip(const Location& l, const Board& b, int hr, int vt) const;

	// return the number of moves the opponent will have on their next turn
	int opponent_moves_next_turn(Location l, Board b) const;

	// return the location of the move the computer should make
	Location best_move(Board& b);

}; //class Computer_player

// ask the player if they want to play again
// if they choose to play again, then a brand new game is started
// otherwise, the program ends
void play_again();

// ANSI foreground colors
const string red = "\033[0;31m";
const string green = "\033[1;32m";
const string yellow = "\033[1;33m";
const string cyan = "\033[0;36m";
const string magenta = "\033[0;35m";
const string reset_color = "\033[0m";

// coordinate the human and computer to play against each other
void play(Player*& p1, Player*& p2, Board& b);
