#pragma once

#include "defs.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <stack>

class Move {
public:
	player pl_{black};
	piece pc_{pawn_b};
	piece convert_to_{empty};
	int from_x_{0};
	int from_y_{0};
	int to_x_{0};
	int to_y_{0};
	piece captured_pc_{empty};
	bool en_passant_{false};

public:

	Move(){}
	Move(player pl, piece pc, int from_x, int from_y, int to_x, int to_y, piece captured_pc, bool en_passant):
			pl_(pl), pc_(pc), from_x_(from_x), from_y_(from_y), to_x_(to_x), to_y_(to_y),
			captured_pc_(captured_pc), en_passant_(en_passant) {}
	Move(player pl, piece pc, int from_x, int from_y, int to_x, int to_y, piece captured_pc):
			pl_(pl), pc_(pc), from_x_(from_x), from_y_(from_y), to_x_(to_x), to_y_(to_y),
			captured_pc_(captured_pc) {}
	Move(player pl, piece pc, piece convert_to, int from_x, int from_y, int to_x, int to_y, piece captured_pc):
			pl_(pl), pc_(pc), convert_to_(convert_to), from_x_(from_x), from_y_(from_y), to_x_(to_x),
			to_y_(to_y), captured_pc_(captured_pc) {}
	Move(player pl, piece pc, int from_x, int from_y, int to_x, int to_y):
			pl_(pl), pc_(pc), from_x_(from_x), from_y_(from_y), to_x_(to_x), to_y_(to_y) {}
	Move(player pl, piece pc, piece convert_to, int from_x, int from_y, int to_x, int to_y):
			pl_(pl), pc_(pc), convert_to_(convert_to), from_x_(from_x), from_y_(from_y), to_x_(to_x),
			to_y_(to_y) {}

	void copy(Move& move) {
		pl_ = move.pl_;
		pc_ = move.pc_;
		convert_to_ = move.convert_to_;
		from_x_ = move.from_x_;
		from_y_ = move.from_y_;
		to_x_ = move.to_x_;
		to_y_ = move.to_y_;
		captured_pc_ = move.captured_pc_;
		en_passant_ = move.en_passant_;
	}

	void set_player(player pl) { pl_ = pl; }

	void set_pc(piece pc) { pc_ = pc; }

	void set_from(int x, int y) { from_x_ = x; from_y_ = y; }

	void set_to(int x, int y) { to_x_ = x; to_y_ = y; }

	void set_en_passant_true() { en_passant_ = true; }

	void set_captured_pc(piece pc) { captured_pc_ = pc; }

	void set_convert_to(piece pc) { convert_to_ = pc; }

	void clear_move() {
		convert_to_ = empty;
		captured_pc_ = empty;
		en_passant_ = false;
	}

	void print() const {
		std::cout << "Piece: " << char(pc_) << " from: (" << from_x_ << ", " << from_y_ << ") To: (" << to_x_ << ", ";
		std::cout << to_y_ << ") Captures: " << char(captured_pc_) << " en_passant: " << en_passant_;
		std::cout << " converts_to: " << char(convert_to_) << std::endl;
	}
};

class State {
public:
	const Move* move_;
	const bool check_black_;
	const bool check_white_;

	const int black_king_x_{0};
	const int black_king_y_{4};
	const int white_king_x_{7};
	const int white_king_y_{4};

	const int black_en_passant_pawn_x_{-1};
	const int black_en_passant_pawn_y_{-1};
	const int white_en_passant_pawn_x_{-1};
	const int white_en_passant_pawn_y_{-1};
	
	const bool black_king_moved_;
	const bool white_king_moved_;

	const bool black_left_rook_moved_;
	const bool black_right_rook_moved_;
	const bool white_left_rook_moved_;
	const bool white_right_rook_moved_;

public:
	State(Move* move, bool cb, bool cw, int bkx, int bky, int wkx, int wky, int beppx, int beppy, int weppx, int weppy,
			bool bkm, bool wkm, bool blrm, bool brrm, bool wlrm, bool wrrm):
			move_(move), check_black_(cb), check_white_(cw), black_king_x_(bkx), black_king_y_(bky),
			white_king_x_(wkx), white_king_y_(wky), black_en_passant_pawn_x_(beppx), black_en_passant_pawn_y_(beppy),
			white_en_passant_pawn_x_(weppx), white_en_passant_pawn_y_(weppy), black_king_moved_(bkm),
			white_king_moved_(wkm), black_left_rook_moved_(blrm), black_right_rook_moved_(brrm),
			white_left_rook_moved_(wlrm), white_right_rook_moved_(wrrm) {}

	void print() {
		std::cout << "*******************************************************************************" << std::endl;
		std::cout << "Move: ";
		move_->print();
		std::cout << "check_black: " << check_black_ << ", check_white: " << check_white_ << std::endl;
		std::cout << "black_king: (" << black_king_x_ << "," << black_king_y_ << "), ";
		std::cout << "white_king: (" << white_king_x_ << "," << white_king_y_ << ") " << std::endl;
		std::cout << "black_king_moved: " << black_king_moved_ << ", ";
		std::cout << "black_left_rook_moved: " << black_left_rook_moved_ << ", ";
		std::cout << "black_right_rook_moved: " << black_right_rook_moved_ << std::endl;
		std::cout << "white_king_moved: " << white_king_moved_ << ", ";
		std::cout << "white_left_rook_moved: " << white_left_rook_moved_ << ", ";
		std::cout << "white_right_rook_moved: " << white_right_rook_moved_ << std::endl;
		std::cout << "black_en_passant_pawn: (" << black_en_passant_pawn_x_ << "," << black_en_passant_pawn_y_ << "), ";
		std::cout << "white_en_passant_pawn: (" << white_en_passant_pawn_x_ << "," << white_en_passant_pawn_y_ << ")" << std::endl;
	}
};

class Board {
public:
	Board() {
		piece** temp_board = new piece*[size_];
		for (int i = 0; i < size_; i++) {
			temp_board[i] = new piece[size_];
		}
		for (int i = 0; i < size_; i++) {
			for (int j = 0; j < size_; j++) {
				temp_board[i][j] = empty;
			}
		}
		board_ = temp_board;
		set_default();
		// piece p = first_piece_in_direction(0, 7, 1, -1);
		// std::cout << "First piece: " << (char)p << std::endl;
	};

	void print_board() {
		for (int i = 0; i < size_; i++) std::cout << "----";
		std::cout << "-\n";
		for (int i = 0; i < size_; i++) {
			for (int j = 0; j < size_; j++) {
				std::cout << "| " << char(board_[i][j]) << " ";
			}
			std::cout << "|\n";
			for (int j = 0; j < size_; j++) {
				std::cout << "----";
			}
			std::cout << "-\n";
		}
		std::cout << "\n\n";
	}

	bool play_move(player pl, piece pc, int from_x, int from_y, int to_x, int to_y, piece convert_to) {
		// std::cout << "Move: " << (char)pc << " from: (" << from_x << "," << from_y << ") to: (" << to_x << "," << to_y;
		// std::cout << ")" << std::endl;
		if (check_validity(pl, pc, from_x, from_y, to_x, to_y, convert_to)) {
			return execute_move(pl, pc, from_x, from_y, to_x, to_y, convert_to, true);
		}
		else {
			std::cout << "Invalid move! Try again." << std::endl;
			return false;
		}
	}

	bool play_bot_move(Move* move, bool logging) {
		// std::cout << "Move: ";
		// move->print();
		// std::cout << "Turn: " << turn_ << " Player: " << move->pl_ << std::endl;
		if (check_validity(move->pl_, move->pc_, move->from_x_, move->from_y_, move->to_x_, move->to_y_, move->convert_to_)) {
			return execute_move(
					move->pl_, move->pc_, move->from_x_, move->from_y_, move->to_x_, move->to_y_, move->convert_to_, logging);
		}
		std::cout << "Erroneous move: ";
		move->print();
		print_game_state();
		print_board();
		return false;
	}

	bool undo_last() {
		auto state = moves_with_states_.top();
		game_ended_ = false;
		stalemate_ = false;
		check_black_ =  state->check_black_;
		check_white_ =  state->check_white_;
		black_king_x_ = state->black_king_x_;
		black_king_y_ = state->black_king_y_;
		white_king_x_ = state->white_king_x_;
		white_king_y_ = state->white_king_y_;

		black_en_passant_pawn_x_ = state->black_en_passant_pawn_x_;
		black_en_passant_pawn_y_ = state->black_en_passant_pawn_y_;
		white_en_passant_pawn_x_ = state->white_en_passant_pawn_x_;
		white_en_passant_pawn_y_ = state->white_en_passant_pawn_y_;

		black_king_moved_ = state->black_king_moved_;
		white_king_moved_ = state->white_king_moved_;

		black_left_rook_moved_ = state->black_left_rook_moved_;
		black_right_rook_moved_ = state->black_right_rook_moved_;
		white_left_rook_moved_ = state->white_left_rook_moved_;
		white_right_rook_moved_ = state->white_right_rook_moved_;
		
		switch (state->move_->pc_) {
			case pawn_w: {
				if (state->move_->en_passant_) {
					board_[black_en_passant_pawn_x_][black_en_passant_pawn_y_] = pawn_b;
					board_[state->move_->to_x_][state->move_->to_y_] = empty;
					board_[state->move_->from_x_][state->move_->from_y_] = pawn_w;
				} else {
					board_[state->move_->to_x_][state->move_->to_y_] = state->move_->captured_pc_;
					board_[state->move_->from_x_][state->move_->from_y_] = pawn_w;
				}
				break;
			}
			case pawn_b: {
				if (state->move_->en_passant_) {
					board_[white_en_passant_pawn_x_][white_en_passant_pawn_y_] = pawn_w;
					board_[state->move_->to_x_][state->move_->to_y_] = empty;
					board_[state->move_->from_x_][state->move_->from_y_] = pawn_b;
				} else {
					board_[state->move_->to_x_][state->move_->to_y_] = state->move_->captured_pc_;
					board_[state->move_->from_x_][state->move_->from_y_] = pawn_b;
				}
				break;
			}
			case king_w: {
				if (state->move_->to_y_ - state->move_->from_y_ == 2) {
					board_[7][7] = rook_w;
					board_[7][5] = empty;
				} else if (state->move_->from_y_ - state->move_->to_y_ == 2) {
					board_[7][0] = rook_w;
					board_[7][3] = empty;
				}
				board_[state->move_->from_x_][state->move_->from_y_] = king_w;
				board_[state->move_->to_x_][state->move_->to_y_] = state->move_->captured_pc_;
				break;
			}
			case king_b: {
				if (state->move_->to_y_ - state->move_->from_y_ == 2) {
					board_[0][7] = rook_b;
					board_[0][5] = empty;
				} else if (state->move_->from_y_ - state->move_->to_y_ == 2) {
					board_[0][0] = rook_b;
					board_[0][3] = empty;
				}
				board_[state->move_->from_x_][state->move_->from_y_] = king_b;
				board_[state->move_->to_x_][state->move_->to_y_] = state->move_->captured_pc_;
				break;
			}
			default: {
				board_[state->move_->from_x_][state->move_->from_y_] = state->move_->pc_;
				board_[state->move_->to_x_][state->move_->to_y_] = state->move_->captured_pc_;
				break;
			}
		}
		moves_with_states_.pop();
		change_turn();
		return true;
	}

	int get_size() { return size_; }

	piece get_piece_at(int i, int j) { return board_[i][j]; }

	bool white_check() { return check_white_; }

	bool black_check() { return check_black_; }

	bool game_not_end() { return !game_ended_; }

	bool get_stalemate() { return stalemate_; }

	bool get_winner() { return winner_; }

	std::vector<Move> get_all_possible_moves(player pl) {
		std::vector<Move> all_moves;
		switch(pl) {
			case white: {
				for (int i = 0; i < size_; i++) {
					for (int j = 0; j < size_; j++) {
						add_moves(pl, queen_w, i, j, all_moves);
						add_moves(pl, rook_w, i, j, all_moves);
						add_moves(pl, bishop_w, i, j, all_moves);
						add_moves(pl, knight_w, i, j, all_moves);
						add_moves(pl, king_w, i, j, all_moves);
						add_moves(pl, pawn_w, i, j, all_moves);
					}
				}
				break;
			}
			case black: {
				for (int i = 0; i < size_; i++) {
					for (int j = 0; j < size_; j++) {
						add_moves(pl, queen_b, i, j, all_moves);
						add_moves(pl, rook_b, i, j, all_moves);
						add_moves(pl, bishop_b, i, j, all_moves);
						add_moves(pl, knight_b, i, j, all_moves);
						add_moves(pl, king_b, i, j, all_moves);
						add_moves(pl, pawn_b, i, j, all_moves);
					}
				}
				break;
			}
		}
		return all_moves;
	}

	void print_game_state() {
		std::cout << "Moves played: " << moves_with_states_.size() << std::endl;
		while (moves_with_states_.size()) {
			auto state = moves_with_states_.top();
			error_stack_.push(moves_with_states_.top());
			state->print();
			moves_with_states_.pop();
		}
		while (error_stack_.size()) {
			moves_with_states_.push(error_stack_.top());
			error_stack_.pop();
		}
	}

	bool not_check_on_square(player pl, int x, int y) {
		switch(pl) {
			case white: {
				if (x-1 >= 0 && y-1 >= 0 && board_[x-1][y-1] == pawn_b) return false; // pawn check
				if (x-1 >= 0 && y+1 >= 0 && board_[x-1][y+1] == pawn_b) return false; // pawn check

				if (x+2 < size_ && y-1 >= 0 && board_[x+2][y-1] == knight_b) return false; // knight check
				if (x+2 < size_ && y+1 < size_ && board_[x+2][y+1] == knight_b) return false; // knight check
				if (x+1 < size_ && y-2 >= 0 && board_[x+1][y-2] == knight_b) return false; // knight check
				if (x+1 < size_ && y+2 < size_ && board_[x+1][y+2] == knight_b) return false; // knight check
				if (x-2 >= 0 && y-1 >= 0 && board_[x-2][y-1] == knight_b) return false; // knight check
				if (x-2 >= 0 && y+1 < size_ && board_[x-2][y+1] == knight_b) return false; // knight check
				if (x-1 >= 0 && y-2 >= 0 && board_[x-1][y-2] == knight_b) return false; // knight check
				if (x-1 >= 0 && y+2 < size_ && board_[x-1][y+2] == knight_b) return false; // knight check

				if (abs(black_king_x_ - x) <= 1 && abs(black_king_y_ - y) <= 1) return false; // king check

				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (i == 0 && j == 0) continue;
						piece p = first_piece_in_direction(x, y, i, j);
						if (abs(i) == 1 && abs(j) == 1) {
							if (p == bishop_b || p == queen_b) return false; // diagonal check
						} else {
							if (p == rook_b || p == queen_b) return false; // row-column check
						}
					}
				}
				return true;
			}
			case black: {
				if (x+1 < size_ && y+1 < size_ && board_[x+1][y+1] == pawn_w) return false; // pawn check
				if (x+1 < size_ && y-1 >= 0 && board_[x+1][y-1] == pawn_w) return false; // pawn check

				if (x+2 < size_ && y-1 >= 0 && board_[x+2][y-1] == knight_w) return false; // knight check
				if (x+2 < size_ && y+1 < size_ && board_[x+2][y+1] == knight_w) return false; // knight check
				if (x+1 < size_ && y-2 >= 0 && board_[x+1][y-2] == knight_w) return false; // knight check
				if (x+1 < size_ && y+2 < size_ && board_[x+1][y+2] == knight_w) return false; // knight check
				if (x-2 >= 0 && y-1 >= 0 && board_[x-2][y-1] == knight_w) return false; // knight check
				if (x-2 >= 0 && y+1 < size_ && board_[x-2][y+1] == knight_w) return false; // knight check
				if (x-1 >= 0 && y-2 >= 0 && board_[x-1][y-2] == knight_w) return false; // knight check
				if (x-1 >= 0 && y+2 < size_ && board_[x-1][y+2] == knight_w) return false; // knight check

				if (abs(white_king_x_ - x) <= 1 && abs(white_king_y_ - y) <= 1) return false; // king check

				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (i == 0 && j == 0) continue;
						piece p = first_piece_in_direction(x, y, i, j);
						if (abs(i) == 1 && abs(j) == 1) {
							if (p == bishop_w || p == queen_w) return false; // diagonal check
						} else {
							if (p == rook_w || p == queen_w) return false; // row-column check
						}
					}
				}
				return true;
			}
		}
	}

	piece first_piece_in_direction (int x, int y, int x_increment, int y_increment) {
		x += x_increment;
		y += y_increment;
		while (y >= 0 && y < size_ &&
				x >= 0 && x < size_ &&
				board_[x][y] == empty) {
			x += x_increment;
			y += y_increment;
		}
		if (x == size_ || x == -1 || y == size_ || y == -1) return empty;
		else return board_[x][y];
	}

private:

	bool check_validity(player pl, piece pc, int from_x, int from_y, int to_x, int to_y, piece convert_to=empty, bool logging=false) {
		if (pl != turn_) {
			// std::cout << "Wrong turn!" << std::endl;
			return false;
		}
		if (board_[from_x][from_y] != pc) {
			// std::cout << "Piece not found!" << std::endl;
			return false;
		}
		if (from_x > 7 || from_x < 0) return false;
		if (from_y > 7 || from_y < 0) return false;
		if (to_x > 7 || to_x < 0) return false;
		if (to_y > 7 || to_y < 0) return false;
		switch (pc) {
			case knight_b: {
				if (std::abs(from_x - to_x) == 1 && std::abs(from_y - to_y) == 2 && check_to_loc(pl, to_x, to_y, true))
					return check_not_pinned(pl, from_x, from_y, to_x, to_y);
				if (std::abs(from_x - to_x) == 2 && std::abs(from_y - to_y) == 1 && check_to_loc(pl, to_x, to_y, true))
					return check_not_pinned(pl, from_x, from_y, to_x, to_y);
				return false;
			}
			case knight_w: {
				if (std::abs(from_x - to_x) == 1 && std::abs(from_y - to_y) == 2 && check_to_loc(pl, to_x, to_y, true))
					return check_not_pinned(pl, from_x, from_y, to_x, to_y);
				if (std::abs(from_x - to_x) == 2 && std::abs(from_y - to_y) == 1 && check_to_loc(pl, to_x, to_y, true))
					return check_not_pinned(pl, from_x, from_y, to_x, to_y);
				return false;
			}
			case rook_b: {
				if (from_x == to_x || from_y == to_y) {
					if (from_x == to_x) {
						if (check_range_empty_y(from_x, from_y, to_y) && check_to_loc(pl, to_x, to_y, true)) {
							return check_not_pinned(pl, from_x, from_y, to_x, to_y);
						}
						return false;
					} else {
						if (check_range_empty_x(from_y, from_x, to_x) && check_to_loc(pl, to_x, to_y, true)) {
							return check_not_pinned(pl, from_x, from_y, to_x, to_y);
						}
						return false;
					}
				}
				return false;
			}
			case rook_w: {
				if (from_x == to_x || from_y == to_y) {
					if (from_x == to_x) {
						if (check_range_empty_y(from_x, from_y, to_y) && check_to_loc(pl, to_x, to_y, true))
							return check_not_pinned(pl, from_x, from_y, to_x, to_y);
						return false;
					} else {
						if (check_range_empty_x(from_y, from_x, to_x) && check_to_loc(pl, to_x, to_y, true))
							return check_not_pinned(pl, from_x, from_y, to_x, to_y);
						return false;
					}
				}
				return false;
			}
			case bishop_b: {
				if (abs(from_x - to_x) == abs(from_y - to_y)) {
					if (check_diagonal_range_empty(from_x, from_y, to_x, to_y) && check_to_loc(pl, to_x, to_y, true))
						return check_not_pinned(pl, from_x, from_y, to_x, to_y);
					return false;
				}
				return false;
			}
			case bishop_w: {
				if (abs(from_x - to_x) == abs(from_y - to_y)) {
					if (check_diagonal_range_empty(from_x, from_y, to_x, to_y) && check_to_loc(pl, to_x, to_y, true))
						return check_not_pinned(pl, from_x, from_y, to_x, to_y);
					return false;
				}
				return false;
			}
			case queen_b: {
				if (from_x == to_x || from_y == to_y) {
					if (from_x == to_x) {
						if (check_range_empty_y(from_x, from_y, to_y) && check_to_loc(pl, to_x, to_y, true))
							return check_not_pinned(pl, from_x, from_y, to_x, to_y);
						return false;
					} else {
						if (check_range_empty_x(from_y, from_x, to_x) && check_to_loc(pl, to_x, to_y, true))
							return check_not_pinned(pl, from_x, from_y, to_x, to_y);
						return false;
					}
				}
				if (abs(from_x - to_x) == abs(from_y - to_y)) {
					if (check_diagonal_range_empty(from_x, from_y, to_x, to_y) && check_to_loc(pl, to_x, to_y, true))
						return check_not_pinned(pl, from_x, from_y, to_x, to_y);
					return false;
				}
				return false;
			}
			case queen_w: {
				if (from_x == to_x || from_y == to_y) {
					if (from_x == to_x) {
						if (check_range_empty_y(from_x, from_y, to_y) && check_to_loc(pl, to_x, to_y, true))
							return check_not_pinned(pl, from_x, from_y, to_x, to_y);
						return false;
					} else {
						if (check_range_empty_x(from_y, from_x, to_x) && check_to_loc(pl, to_x, to_y, true))
							return check_not_pinned(pl, from_x, from_y, to_x, to_y);
						return false;
					}
				}
				if (abs(from_x - to_x) == abs(from_y - to_y)) {
					if (check_diagonal_range_empty(from_x, from_y, to_x, to_y) && check_to_loc(pl, to_x, to_y, true))
						return check_not_pinned(pl, from_x, from_y, to_x, to_y);
					return false;
				}
				return false;
			}
			case king_b: {
				// if (logging)
				// 		std::cout << "Validating king b" << std::endl;
				if (abs(from_x - to_x) <= 1 && abs(from_y - to_y) <= 1 && check_to_loc(pl, to_x, to_y, true)) {
					// if (logging)
					// 	std::cout << "Validating as normal move " << std::endl;
					board_[black_king_x_][black_king_y_] = empty;
					bool ret = not_check_on_square(pl, to_x, to_y);
					board_[black_king_x_][black_king_y_] = king_b;
					// if (logging)
					// 	std::cout << "Returning: " << ret << std::endl;
					return ret;
				}
				if (from_x == 0 && from_y == 4 && to_x == 0 && to_y == 6) {
					// if (logging)
					// 	std::cout << "Validating as right castle " << std::endl;
					return not_check_on_square(pl, 0, 5) && not_check_on_square(pl, 0, 6) &&
							!black_right_rook_moved_ && !black_king_moved_ && !check_black_ &&
							board_[0][5] == empty && board_[0][6] == empty && board_[0][7] == rook_b;
				}
				if (from_x == 0 && from_y == 4 && to_x == 0 && to_y == 2) {
					// if (logging)
					// 	std::cout << "Validating as left castle " << std::endl;
					return not_check_on_square(pl, 0, 3) && not_check_on_square(pl, 0, 2) &&
							!black_left_rook_moved_ && !black_king_moved_ && !check_black_ &&
							board_[0][3] == empty && board_[0][2] == empty &&
							board_[0][1] == empty && board_[0][0] == rook_b;
				}
				return false;
			}
			case king_w: {
				if (abs(from_x - to_x) <= 1 && abs(from_y - to_y) <= 1 && check_to_loc(pl, to_x, to_y, true)) {
					board_[white_king_x_][white_king_y_] = empty;
					bool ret = not_check_on_square(pl, to_x, to_y);
					board_[white_king_x_][white_king_y_] = king_w;
					return ret;
				}

				if (from_x == 7 && from_y == 4 && to_x == 7 && to_y == 6) {
					return not_check_on_square(pl, 7, 5) && not_check_on_square(pl, 7, 6) &&
							!white_right_rook_moved_ && !white_king_moved_ && !check_white_ &&
							board_[7][5] == empty && board_[7][6] == empty && board_[7][7] == rook_w;
				}
				if (from_x == 7 && from_y == 4 && to_x == 7 && to_y == 2) {
					return not_check_on_square(pl, 7, 3) && not_check_on_square(pl, 7, 2) &&
							!white_left_rook_moved_ && !white_king_moved_ && !check_white_ &&
							board_[7][3] == empty && board_[7][2] == empty &&
							board_[7][1] == empty && board_[7][0] == rook_w;
				}
				return false;
			}
			case pawn_b: {
				if (to_x == 7 &&
						!(convert_to == queen_b || convert_to == bishop_b || convert_to == rook_b || convert_to == knight_b)) {
							return false;
				}
				if (to_x - from_x == 1 && to_y == from_y && board_[to_x][to_y] == empty)
					return check_not_pinned(pl, from_x, from_y, to_x, to_y); // single move
				if (to_x -  from_x == 2 && to_y == from_y && board_[to_x - 1][to_y] == empty && from_x == 1 && board_[to_x][to_y] == empty)
					return check_not_pinned(pl, from_x, from_y, to_x, to_y); // double move
				if (abs(to_y - from_y) == 1 && (to_x - from_x) == 1) { // diagonal capture
					if (check_to_loc(pl, to_x, to_y, false))
						return check_not_pinned(pl, from_x, from_y, to_x, to_y);
					if (to_x-1 == white_en_passant_pawn_x_ && to_y == white_en_passant_pawn_y_)
						return check_not_pinned(pl, from_x, from_y, to_x, to_y);
				}
					
				return false;
			}
			case pawn_w: {
				if (to_x == 0 &&
						!(convert_to == queen_w || convert_to == bishop_w || convert_to == rook_w || convert_to == knight_w)) {
							return false;
				}
				if (from_x - to_x == 1 && to_y == from_y && board_[to_x][to_y] == empty)
					return check_not_pinned(pl, from_x, from_y, to_x, to_y); // single move
				if (from_x -  to_x == 2 && to_y == from_y && board_[to_x + 1][to_y] == empty && from_x == 6 && board_[to_x][to_y] == empty)
					return check_not_pinned(pl, from_x, from_y, to_x, to_y); // double move
				if (abs(to_y - from_y) == 1 && (from_x - to_x) == 1) { // diagonal capture
					if (check_to_loc(pl, to_x, to_y, false))
						return check_not_pinned(pl, from_x, from_y, to_x, to_y);
					if (to_x+1 == black_en_passant_pawn_x_ && to_y == black_en_passant_pawn_y_)
						return check_not_pinned(pl, from_x, from_y, to_x, to_y);
				}
				return false;
			}
			default: {
				std::cerr << "Bad piece!" << std::endl;
			}
		}
		
		return true;
	}

	bool check_not_pinned(player pl, int remove_x, int remove_y, int at_x, int at_y) {
		piece moving_piece = board_[remove_x][remove_y];
		piece moved_to_piece = board_[at_x][at_y];
		board_[remove_x][remove_y] = empty;
		board_[at_x][at_y] = moving_piece;
		bool causes_check;
		switch(pl) {
			case white: {
				causes_check = !not_check_on_square(pl, white_king_x_, white_king_y_);
				break;
			}
			case black: {
				causes_check = !not_check_on_square(pl, black_king_x_, black_king_y_);
				break;
			}
		}
		board_[remove_x][remove_y] = moving_piece;
		board_[at_x][at_y] = moved_to_piece;
		return !causes_check;
	}

	State* copy_current_state(Move* move) {
		return new State(move, check_black_, check_white_, black_king_x_, black_king_y_, white_king_x_, white_king_y_,
				black_en_passant_pawn_x_, black_en_passant_pawn_y_, white_en_passant_pawn_x_, white_en_passant_pawn_y_,
				black_king_moved_, white_king_moved_, black_left_rook_moved_, black_right_rook_moved_,
				white_left_rook_moved_, white_right_rook_moved_);
	}

	bool execute_move(player pl, piece pc, int from_x, int from_y, int to_x, int to_y, piece convert_to, bool logging) {
		Move* move = new Move(pl, pc, from_x, from_y, to_x, to_y);
		State* state = copy_current_state(move);
		switch(pc) {
			case king_b: {
				black_king_x_ = to_x;
				black_king_y_ = to_y;
				black_king_moved_ = true;
				move->set_captured_pc(board_[to_x][to_y]);
				basic_move(pc, from_x, from_y, to_x, to_y);
				if (to_y - from_y == 2) {
					board_[0][7] = empty;
					board_[0][5] = rook_b;
					black_right_rook_moved_ = true;
				}
				if (from_y - to_y == 2) {
					board_[0][0] = empty;
					board_[0][3] = rook_b;
					black_left_rook_moved_ = true;
				}
				break;
			}
			case king_w: {
				white_king_x_ = to_x;
				white_king_y_ = to_y;
				white_king_moved_ = true;
				move->set_captured_pc(board_[to_x][to_y]);
				basic_move(pc, from_x, from_y, to_x, to_y);
				if (to_y - from_y == 2) {
					board_[7][7] = empty;
					board_[7][5] = rook_w;
					white_right_rook_moved_ = true;
				}
				if (from_y - to_y == 2) {
					board_[7][0] = empty;
					board_[7][3] = rook_w;
					white_left_rook_moved_ = true;
				}
				break;
			}
			case pawn_b: {
				move->set_captured_pc(board_[to_x][to_y]);
				if (to_x-1 == white_en_passant_pawn_x_ && to_y == white_en_passant_pawn_y_) {
					board_[white_en_passant_pawn_x_][white_en_passant_pawn_y_] = empty;
					move->set_en_passant_true();
					move->set_captured_pc(pawn_b);
				}
				basic_move(pc, from_x, from_y, to_x, to_y);
				if (to_x - from_x == 2) {
					black_en_passant_pawn_x_ = to_x;
					black_en_passant_pawn_y_ = to_y;
				}
				if (to_x == 7) board_[to_x][to_y] = convert_to;
				break;
			}
			case pawn_w: {
				move->set_captured_pc(board_[to_x][to_y]);
				if (to_x+1 == black_en_passant_pawn_x_ && to_y == black_en_passant_pawn_y_) {
					board_[black_en_passant_pawn_x_][black_en_passant_pawn_y_] = empty;
					move->set_en_passant_true();
					move->set_captured_pc(pawn_b);
				}
				basic_move(pc, from_x, from_y, to_x, to_y);
				if (from_x - to_x == 2) {
					white_en_passant_pawn_x_ = to_x;
					white_en_passant_pawn_y_ = to_y;
				}
				if (to_x == 0) board_[to_x][to_y] = convert_to;
				break;
			}
			case rook_b: {
				if (from_x == 0 && from_y == 0) black_left_rook_moved_ = true;
				if (from_x == 0 && from_y == size_-1) black_right_rook_moved_ = true;
				move->set_captured_pc(board_[to_x][to_y]);
				basic_move(pc, from_x, from_y, to_x, to_y);
				break;
			}
			case rook_w: {
				if (from_x == size_-1 && from_y == 0) white_left_rook_moved_ = true;
				if (from_x == size_-1 && from_y == size_-1) white_right_rook_moved_ = true;
				move->set_captured_pc(board_[to_x][to_y]);
				basic_move(pc, from_x, from_y, to_x, to_y);
				break;
			}
			default: {
				move->set_captured_pc(board_[to_x][to_y]);
				basic_move(pc, from_x, from_y, to_x, to_y);
			}
		}

		moves_with_states_.push(state);

		switch(turn_) {
			case black: {
				check_white_ = !not_check_on_square(white, white_king_x_, white_king_y_);
				turn_ = white;
				auto moves = get_all_possible_moves(turn_);
				if (moves.size() == 0 && check_white_) {
					game_ended_ = true;
					winner_ = black;
					if (logging)
						std::cout << "Checkmate! Black won!" << std::endl;
				} else if (moves.size() == 0) {
					game_ended_ = true;
					stalemate_ = true;
					if (logging)
						std::cout << "Stalemate! White has no more moves!" << std::endl;
				}
				break;
			}
			case white: {
				check_black_ = !not_check_on_square(black, black_king_x_, black_king_y_);
				turn_ = black;
				auto moves = get_all_possible_moves(turn_);
				if (moves.size() == 0 && check_black_) {
					game_ended_ = true;
					winner_ = white;
					if (logging)
						std::cout << "Checkmate! White won!" << std::endl;
				} else if (moves.size() == 0) {
					game_ended_ = true;
					stalemate_ = true;
					if (logging)
						std::cout << "Stalemate! Black has no more moves!" << std::endl;
				}
				break;
			}
		}
		return true;
	}

	void basic_move(piece pc, int from_x, int from_y, int to_x, int to_y) {
		board_[from_x][from_y] = empty;
		board_[to_x][to_y] = pc;
		black_en_passant_pawn_x_ = -1;
		black_en_passant_pawn_y_ = -1;
		white_en_passant_pawn_x_ = -1;
		white_en_passant_pawn_y_ = -1;
	}

	bool check_to_loc(player pl, int to_x, int to_y, bool allow_empty) {
		if (board_[to_x][to_y] == empty && allow_empty) {
			return true;
		}	
		switch(pl) {
			case black: {
				switch(board_[to_x][to_y]) {
					case king_w: { return false; }
					case queen_w: { return true; }
					case bishop_w: { return true; }
					case pawn_w: { return true; }
					case knight_w: { return true; }
					case rook_w: { return true; }
					case king_b: {return false; }
					case queen_b: {return false; }
					case rook_b: {return false; }
					case bishop_b: {return false; }
					case knight_b: {return false; }
					case pawn_b: {return false; }
					default: { return false; }
				}
			}
			case white: {
				switch(board_[to_x][to_y]) {
					case king_b: { return false; }
					case queen_b: { return true; }
					case bishop_b: { return true; }
					case pawn_b: { return true; }
					case knight_b: { return true; }
					case rook_b: { return true; }
					case king_w: {return false; }
					case queen_w: {return false; }
					case rook_w: {return false; }
					case bishop_w: {return false; }
					case knight_w: {return false; }
					case pawn_w: {return false; }
					default: { return false; }
				}
			}
		}
	}

	bool check_range_empty_y (int x, int from_y, int to_y) {
		int smaller = (from_y > to_y) ? to_y : from_y;
		int larger = (from_y > to_y) ? from_y : to_y;

		bool all_empty = true;
		for (int i = smaller+1; i < larger; i++) {
			if (board_[x][i] != empty) {
				all_empty = false;
				break;
			}
		}
		if (!all_empty) return false;
		return true;
	}

	bool check_range_empty_x (int y, int from_x, int to_x) {
		int smaller = (from_x > to_x) ? to_x : from_x;
		int larger = (from_x > to_x) ? from_x : to_x;

		bool all_empty = true;
		for (int i = smaller+1; i < larger; i++) {
			if (board_[i][y] != empty) {
				all_empty = false;
				break;
			}
		}
		if (!all_empty) return false;
		return true;
	}

	bool check_diagonal_range_empty(int from_x, int from_y, int to_x, int to_y) {
		int x_increment = 1;
		int y_increment = 1;
		if (to_x < from_x) x_increment = -1;
		if (to_y < from_y) y_increment = -1;

		int cur_x = from_x;
		int cur_y = from_y;
		bool all_empty = true;
		while (cur_x != to_x && cur_y != to_y) {
			cur_x += x_increment;
			cur_y += y_increment;
			if (board_[cur_x][cur_y] != empty && cur_x != to_x && cur_y != to_y) {
				all_empty = false;
				break;
			}
		}
		if (!all_empty) return false;
		return true;
	}

	void set_default() {
		for (int i = 0; i < size_; i++) {
			board_[1][i] = pawn_b;
			board_[6][i] = pawn_w;
		}
		// board_[1][6] = empty;
		board_[0][0] = rook_b;
		board_[0][7] = rook_b;
		// board_[0][7] = queen_w;
		board_[7][0] = rook_w;
		board_[7][7] = rook_w;

		board_[0][1] = knight_b;
		board_[0][6] = knight_b;
		board_[7][1] = knight_w;
		board_[7][6] = knight_w;

		board_[0][2] = bishop_b;

		board_[0][5] = bishop_b;
		board_[7][2] = bishop_w;
		board_[7][5] = bishop_w;

		// board_[2][5] = king_b;
		board_[0][4] = king_b;
		board_[0][3] = queen_b;
		board_[7][4] = king_w;
		board_[7][3] = queen_w;

		// board_[0][1] = king_b;
		// board_[4][2] = queen_w;
		// board_[2][7] = king_w;
		// board_[4][4] = knight_w;
		// board_[5][4] = pawn_w;
		// board_[6][5] = pawn_w;
		// black_king_moved_ = true;
		// white_king_moved_ = true;
		// black_left_rook_moved_ = true;
		// black_right_rook_moved_ = true;
		// white_left_rook_moved_ = true;
		// white_right_rook_moved_ = true;
		// black_king_x_ = 0;
		// black_king_y_ = 1;
		// white_king_x_ = 2;
		// white_king_y_ = 7;
		// turn_ = white;
	}

	void add_moves(player pl, piece pc, int x, int y, std::vector<Move>& moves) {
		if (board_[x][y] != pc) return;
		for (int i = 0; i < size_; i++) {
			for (int j = 0; j < size_; j++) {
				if (pc != pawn_b && pc != pawn_w) {
					if ((i == 0 && j == 0 && pc == king_b) || (i == 0 && j == 2 && pc == king_b)) {
						// std::cout << "Checking " << i << ", " << j << std::endl;
						if (check_validity(pl, pc, x, y, i, j, empty, false)) {
							// std::cout << "Valid move " << i << ", " << j << std::endl;
							moves.push_back(Move(pl, pc, x, y, i, j, board_[i][j]));
						}
					} else {
						if (check_validity(pl, pc, x, y, i, j)) {
							moves.push_back(Move(pl, pc, x, y, i, j, board_[i][j]));
						}
					}
				} else if (pc != pawn_w) {
					if (i == 0) {
						if (check_validity(pl, pc, x, y, i, j)) {
							moves.push_back(Move(pl, pc, queen_w, x, y, i, j, board_[i][j]));
							moves.push_back(Move(pl, pc, rook_w, x, y, i, j, board_[i][j]));
							moves.push_back(Move(pl, pc, bishop_w, x, y, i, j, board_[i][j]));
							moves.push_back(Move(pl, pc, knight_w, x, y, i, j, board_[i][j]));
						}
					} else {
						if (check_validity(pl, pc, x, y, i, j) && y != j && board_[i][j] == empty)
							moves.push_back(Move(pl, pc, x, y, i, j, board_[i+1][j], true));
						else if (check_validity(pl, pc, x, y, i, j))
							moves.push_back(Move(pl, pc, x, y, i, j, board_[i][j]));
					}
				} else {
					if (i == size_) {
						if (check_validity(pl, pc, x, y, i, j)) {
							moves.push_back(Move(pl, pc, queen_b, x, y, i, j, board_[i][j]));
							moves.push_back(Move(pl, pc, rook_b, x, y, i, j, board_[i][j]));
							moves.push_back(Move(pl, pc, bishop_b, x, y, i, j, board_[i][j]));
							moves.push_back(Move(pl, pc, knight_b, x, y, i, j, board_[i][j]));
						}
					} else {
						if (check_validity(pl, pc, x, y, i, j) && y != j && board_[i][j] == empty)
							moves.push_back(Move(pl, pc, x, y, i, j, board_[i-1][j], true));
						else if (check_validity(pl, pc, x, y, i, j))
							moves.push_back(Move(pl, pc, x, y, i, j, board_[i][j]));
					}
				}
			}
		}
	}

	void change_turn() {
		if (turn_ == white) {
			turn_ = black;
		} else {
			turn_ = white;
		}
	}

private:
	piece** board_;
	player turn_{white};
	player winner_{white};
	bool stalemate_{false};
	bool check_white_{false};
	bool check_black_{false};
	bool game_ended_{false};
	int black_king_x_{0};
	int black_king_y_{4};
	int white_king_x_{7};
	int white_king_y_{4};

	int black_en_passant_pawn_x_{-1};
	int black_en_passant_pawn_y_{-1};
	int white_en_passant_pawn_x_{-1};
	int white_en_passant_pawn_y_{-1};
	
	bool black_king_moved_{false};
	bool white_king_moved_{false};

	bool black_left_rook_moved_{false};
	bool black_right_rook_moved_{false};
	bool white_left_rook_moved_{false};
	bool white_right_rook_moved_{false};

	static constexpr int size_ = 8;
	std::stack<State*> moves_with_states_;
	std::stack<State*> error_stack_;
};
