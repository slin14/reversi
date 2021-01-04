#include "reversi.h"
#include <cassert>
#include <string>

using namespace std;

Location::Location(int n, int c) {
	row = n;
	column = c;
}

Location::Location(char n, char c) {
	row = n - '1' + 1;
	column = c - 'a' + 1;
}

ostream& operator<<(ostream& os, const Location& l) {
	os << l.row;
	os << char('a' + l.column -1);
	return os;
}

bool operator==(const Location& a, const Location& b) {
	if (a.row == b.row && a.column == b.column) {
    	return true;
	}
	return false;
}

Board::Board(int l)
: board(l), x_count(2), o_count(2){
	using namespace Grid;
	//assert(l == 10);

	//initialize all grids to empty
	for (int i = 0; i < l; i++) {
    	board[i] = vector<char>(l, empty);
	}

	char row1 = 'a';
	for (int i = 1; i < l - 1; i++) {
    	board[0][i] = board[9][i] = row1;
    	row1++;
	}

	char col1 = '1';
	for (int i = 1; i < l - 1; i++) {
    	board[i][0] = board[i][9] = col1;
    	col1++;
	}

	board[4][4] = board[5][5] = human;
	board[5][4] = board[4][5] = computer;

    
	board[0][0] = board[9][9] = board[0][9] = board[9][0] = ' ';
}

int Board::length() const {
	return board.size();
}

int Board::get_x() const {
	return x_count;
}

int Board::get_o() const {
	return o_count;
}

char Board::get_loc(Location& l) const {
	return board[l.row][l.column];
}

void Board::print_board() const {
	using namespace Grid;
	cout << cyan;
	for (int i = 0 ; i < length(); i++) {
    	for (int j = 0; j < length(); j++) {

        	if (board[i][j] == empty) {
            	cout << yellow;
        	} else if(board[i][j] == human) {
            	cout << red;
        	} else if(board[i][j] == computer) {
            	cout << green;
        	} else if (board[i][j] == '?') {
            	cout << reset_color;
        	} else{
            	// the wall
            	cout << cyan;
        	}

        	cout << board[i][j] << " ";
    	}
    	cout <<"\n";
	}
	cout << reset_color;
}
void Board::print_state() const {
	cout << red << "human(x) : "
    	<< get_x() << "\n";

	cout << green << "computer(o) : "
    	<< get_o() << "\n";

	cout << reset_color;
}

void Board::print() const {
	print_board();
	cout << "\n";
	print_state();
	cout << "\n";
}

void Board::set(Location& l, const char& g) {
	// assert(legal(l,g));
	// assert(g == human || g == computer);
	using namespace Grid;

	int r = l.row;
	int c = l.column;
	if (g == human) {
    	board[r][c] = human;
    	x_count++;
	} else if (g == computer) {
    	board[r][c] = computer;
    	o_count++;
	} else {
    	board[r][c] = g;
	}
}

void Board::flip(Location& l, const char& g) {
	// assert(legal(l,g));
	// assert(g == human || g == computer);
	using namespace Grid;

	int flip_count = 0;

	flip_count += flip(l, g, 1, 0);
	flip_count += flip(l, g, -1, 0);
	flip_count += flip(l, g, 0, 1);
	flip_count += flip(l, g, 0, -1);
	flip_count += flip(l, g, 1, 1);
	flip_count += flip(l, g, 1, -1);
	flip_count += flip(l, g, -1, 1);
	flip_count += flip(l, g, -1, -1);

	if (g == human) {
    	x_count += flip_count;
    	o_count -= flip_count;
	} else {
    	x_count -= flip_count;
    	o_count += flip_count;
	}
}

int Board::flip(Location& l, const char& g, int hr, int vt) {
	// assert(hr != 0 || vt != 0);

	int flip_count = 0;
	for (int r = l.row, c = l.column; board[r+vt][c+hr] != Grid::empty &&
    	r+vt >= 1 && r+vt <= 8 && c+hr >= 1 && c+hr <= 8; r+=vt, c+=hr) {
    	if (board[r+vt][c+hr] == g) {
        	for (; board[r][c] != g; r-=vt, c-=hr) {
            	board[r][c] = g;
            	flip_count++;
        	}
    	}
	}
	return flip_count;
}

bool Board::legal(Location& l, const char& g) const {
	using namespace Grid;

	if (get_loc(l) != empty)  return false;
    
	if (legal(l, g, 1, 0)) return true;
	if (legal(l, g, -1, 0)) return true;
	if (legal(l, g, 0, 1)) return true;
	if (legal(l, g, 0, -1)) return true;
	if (legal(l, g, 1, 1)) return true;
	if (legal(l, g, 1, -1)) return true;
	if (legal(l, g, -1, 1)) return true;
	if (legal(l, g, -1, -1)) return true;

	return false;
}

bool Board::legal(Location& l, const char& g, int hr, int vt) const {
	// assert(hr != 0 || vt != 0);

	for (int r = l.row+vt, c = l.column+hr; board[r][c] != Grid::empty && board[r][c] != g &&
    	r >= 1 && r <= 8 && c >= 1 && c <= 8; r+=vt, c+=hr) {
    	if (board[r+vt][c+hr] == g) {
        	return true;
    	}
	}
	return false;
}


void Board::reset() {
	using namespace Grid;
	//initialize all grids to empty
	for (int i = 1; i < length() - 1; i++) {
    	for (int j = 1; j < length() - 1; j++) {
        	board[i][j] = empty;
    	}
	}

	board[4][4] = board[5][5] = human;
	board[5][4] = board[4][5] = computer;

	x_count = o_count = 2;
}

char Board::get(int r, int c) const {
	return board[r][c];
}

Human_player::Human_player(Board& b) {
	hint(b);
}


void Human_player::move(Board& b) {
	using namespace Grid;
    
	show_hint();
	if (end()) {
    	cout << "You have no move!\n";
    	return;
	}

	bool loc_ok = false;
	while (!loc_ok) {
    	cout << "Enter your next move: ";
    	char r = cin.get();
    	char c = cin.get();
   	 
    	if (r < '1' || r > '8' || c < 'a' || c > 'h') {
        	// cout << r << c << "\nError 1\n";
        	cin.ignore(10000,'\n');
        	cin.clear();
        	continue;
    	}
   	 
    	Location l(r, c);
    	for (int i = 0; i < next_move.size(); i++) {
        	if (next_move[i].row == l.row
            	&& next_move[i].column == l.column) {
            	loc_ok = true;
            	break;
        	}
    	}
   	 
    	if (!loc_ok) {
        	cin.ignore(10000,'\n');
        	cin.clear();
        	continue;
    	}
    	cin.ignore(10000, '\n');
    	cin.clear();
    	loc_ok = true;
    	cout << "Your move: " << l << "\n";
    	for (Location l : next_move) {
        	// clear the hint ('?')
        	b.set(l, Grid::empty);
    	}
    	b.set(l, show());
    	b.flip(l, show());
	}
    
}

void Human_player::hint(Board& b) {
	next_move.clear();
	for (int i = 1; i < 9; i++) {
    	for (int j = 1 ; j < 9; j++) {
        	Location l(i, j);
        	if (b.legal(l, show())) {
            	next_move.push_back(l);
        	}
    	}
	}

	if (!next_move.empty()) {
    	for (Location l : next_move) {
        	b.set(l, '?');
    	}
	}
}

void Human_player::show_hint() const{
	// hint(b);
	if (end()) return;
	cout << "Your next possible move: ";
    	for (Location l : next_move) {
        	cout << l << " ";
    	}
    	cout << "\n";
}

bool Human_player::end() const {
	if (next_move.empty()) {
    	return true;
	} else {
    	return false;
	}
}

int Human_player::get_size() const {
	return next_move.size();
}

Computer_player::Computer_player(Board& b) {
	hint(b);
}

void Computer_player::move(Board& b) {
	hint(b);
	if (end()) return;
	Location move = best_move(b);
	b.set(move, show());
	b.flip(move, show());
	cout << "Computer's move: " << move << "\n";
}

void Computer_player::hint(Board& b) {
	next_move.clear();
	for (char i = '1'; i < '9'; i++) {
    	for (char j = 'a' ; j < 'i'; j++) {
        	Location l(i, j);
        	if (b.legal(l, show())) next_move.push_back(l);
    	}
	}
}

bool Computer_player::end() const {
	if (next_move.empty()) {
    	return true;
	} else {
    	return false;
	}
}

bool Computer_player::can_get_corner() const {
	Location top_l ('1', 'a');
	Location top_r ('1', 'h');
	Location btm_l ('8', 'a');
	Location btm_r ('8', 'h');
	for (int i = 0; i < next_move.size(); i++) {
    	// Location top_l ('1', 'a');
    	// Location top_r ('1', 'h');
    	// Location btm_l ('8', 'a');
    	// Location btm_r ('8', 'h');
    	if (next_move[i] == top_l || next_move[i] == top_r || next_move[i] == btm_l || next_move[i] == btm_r) {
        	return true;
    	}
	}
	return false;
}

Location Computer_player::get_corner() const {
	// assert(!can_get_corner());

	Location top_l ('1', 'a');
	Location top_r ('1', 'h');
	Location btm_l ('8', 'a');
	Location btm_r ('8', 'h');
	for (int i = 0; i < next_move.size(); i++) {
    	// Location top_l ('1', 'a');
    	// Location top_r ('1', 'h');
    	// Location btm_l ('8', 'a');
    	// Location btm_r ('8', 'h');
    	if (next_move[i] == top_l || next_move[i] == top_r || next_move[i] == btm_l || next_move[i] == btm_r) {
        	return next_move[i];
    	}
	}
	cout << "should never get here\n";
	return next_move[0]; // to satify the compiler

}

int Computer_player::flip_count(const Location& l, const Board& b) const {
	// assert(legal(l,g));
	using namespace Grid;

	int flip_count = 0;

	flip_count += flip(l, b, 1, 0);
	flip_count += flip(l, b, -1, 0);
	flip_count += flip(l, b, 0, 1);
	flip_count += flip(l, b, 0, -1);
	flip_count += flip(l, b, 1, 1);
	flip_count += flip(l, b, 1, -1);
	flip_count += flip(l, b, -1, 1);
	flip_count += flip(l, b, -1, -1);

	return flip_count;
}

int Computer_player::flip(const Location& l, const Board& b, int hr, int vt) const {
	// assert(hr != 0 || vt != 0);

	int flip_count = 0;
	for (int r = l.row, c = l.column; b.get(r+vt, c+hr) != Grid::empty &&
    	r+vt > 1 && r+vt < 8 && c+hr > 1 && c+hr < 8; r+=vt, c+=hr) {
    	if (b.get(r+vt, c+hr) == 'o') {
        	for (; b.get(r+vt, c+hr) != 'o'; r-=vt, c-=hr) {
            	flip_count++;
        	}
    	}
	}
	return flip_count;
}

int Computer_player::opponent_moves_next_turn(Location l, Board b) const {
	// runs a stimulation to find out the number of moves
	// the opponent will have on their next turn
	b.set(l, show());
	b.flip(l, show());
	//stimulate human's turn
	Human_player opponent(b);
	return opponent.get_size();
}

Location Computer_player::best_move(Board& b) {
	hint(b);
	//assert(!end());
	if (next_move.size() == 1) return next_move[0];
	int max_flip = 0;
	for (int i = 0; i < next_move.size(); i++) {
    	if (can_get_corner()) { // will always get corners if it is possible
        	return get_corner();
    	}
    	//find the max pieces that can be flipped
    	int num_flip = flip_count(next_move[i], b);
    	if (num_flip > max_flip) {
        	max_flip = num_flip;
    	}
	}
	//insert all moves that would flip the most pieces into a vector
	vector<Location> max_flip_loc;
	for (int i = 0; i < next_move.size(); i++) {
    	int num_flip = flip_count(next_move[i], b);
    	if (num_flip == max_flip) {
        	max_flip_loc.push_back(next_move[i]);
    	}
	}
	if (max_flip_loc.size() == 1) return max_flip_loc[0];
	//find the min number of moves the opponent will have
	int min_opp_moves = 1000000;
	for (int i = 0; i < max_flip_loc.size(); i++) {
    	int opp_moves = opponent_moves_next_turn(max_flip_loc[i], b);
    	if (opp_moves < min_opp_moves) {
        	min_opp_moves = opp_moves;
    	}
	}
	//for all max flip locations, select one that would minimize opponents' next moves
	for (int i = 0; i < max_flip_loc.size(); i++) {
    	int opp_moves = opponent_moves_next_turn(max_flip_loc[i], b);
    	if (opp_moves == min_opp_moves) {
        	return max_flip_loc[i];
    	}
	}
	cout << "should never get here\n";
	return next_move[0]; // to satisfy compiler
}

void play(Player*& p1, Player*& p2, Board& b) {
	while ((!p1->end()) || (!p2->end())) {
    	b.print();
    	p1->move(b);
    	if (!p1->end()) b.print();
    	p2->move(b);
    	p1->hint(b);
	}
	b.print();
	if(b.get_x() > b.get_o()) {
    	cout << "Human wins\n";
	} else if (b.get_o() > b.get_x()) {
    	cout << "Computer wins\n";
	} else {
    	cout << "A tie!\n";
	}
}

int main() {
	Board b(10);
	Computer_player c(b);
	Human_player p(b);
	Player* p2 = &c;
	Player* p1 = &p;
	Player* p2 = &c;
	Player* p1 = &p;
	play(p1, p2, b);
	cout << "Play again? (y/n)\n";
	string ans = "";
	 while (cin >> ans && ans == "y") {
    	b.reset();
	cin.ignore(10000, '\n');
    	cin.clear();
    	p1->hint(b);
    	p2->hint(b);
    	play(p1, p2, b);    
	}
}

