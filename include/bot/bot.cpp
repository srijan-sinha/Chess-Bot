#include "game/board.h"
#include "game/defs.h"
#include "utils/util.h"
#include <unordered_map>
#include <climits>
#include <limits>
#include <chrono>

class Bot {
public:
	Bot(Board* board, player pl, int depth): board_(board), pl_(pl), depth_(depth), first_turn_(pl==white) {}

	void start_game() {
		board_->print_board();
		Move* move = new Move();
		if (first_turn_) {
			find_and_play_move();
			change_turn();
		}

		// board_->play_move(white, pawn_w, 6, 3, 4, 3, empty);
		// board_->print_board();
		// board_->play_move(black, pawn_b, 1, 4, 3, 4, empty);
		// board_->print_board();
		// board_->play_move(white, pawn_w, 4, 3, 3, 4, empty);
		// board_->print_board();
		// board_->play_move(black, queen_b, 0, 3, 4, 7, empty);
		// board_->print_board();
		// board_->play_move(white, knight_w, 7, 6, 5, 5, empty);
		// board_->print_board();
		// board_->play_move(black, bishop_b, 0, 5, 4, 1, empty);
		// board_->print_board();
		// board_->play_move(white, bishop_w, 7, 2, 6, 3, empty);
		// board_->print_board();
		// board_->play_move(black, queen_b, 4, 7, 4, 2, empty);
		// board_->print_board();
		// board_->play_move(white, bishop_w, 6, 3, 4, 1, empty);
		// board_->print_board();
		// board_->play_move(black, queen_b, 4, 2, 4, 1, empty);
		// board_->print_board();
		// board_->play_move(white, queen_w, 7, 3, 6, 3, empty);
		// board_->print_board();
		// board_->play_move(black, queen_b, 4, 1, 6, 1, empty);
		// board_->print_board();
		// board_->play_move(white, queen_w, 6, 3, 3, 6, empty);
		// board_->print_board();
		// board_->play_move(black, queen_b, 6, 1, 7, 0, empty);
		// board_->print_board();
		// board_->play_move(white, pawn_w, 4, 4, 3, 5, empty);
		// board_->print_board();
		// board_->play_move(black, pawn_b, 2, 4, 3, 5, empty);
		// board_->print_board();
		// board_->play_move(white, queen_w, 4, 6, 6, 4, empty);
		// board_->print_board();
		// board_->play_move(black, knight_b, 2, 2, 4, 3, empty);
		// board_->print_board();
		// board_->play_move(white, queen_w, 6, 4, 3, 4, empty);
		// board_->print_board();
		// board_->play_move(black, knight_b, 4, 3, 6, 2, empty);
		// board_->print_board();
		// board_->play_move(white, king_w, 7, 4, 7, 3, empty);
		// board_->print_board();
		// board_->play_move(black, knight_b, 6, 2, 7, 0, empty);
		// board_->print_board();
		// board_->play_move(white, queen_w, 3, 4, 0, 7, empty);
		// board_->print_board();
		// board_->play_move(black, bishop_b, 1, 4, 3, 6, empty);
		// board_->print_board();
		// board_->play_move(white, queen_w, 0, 7, 0, 6, empty);
		// board_->print_board();
		// board_->play_move(black, king_b, 0, 4, 1, 4, empty);
		// board_->print_board();
		// board_->play_move(white, queen_w, 0, 6, 1, 7, empty);
		// board_->print_board();
		// board_->play_move(black, king_b, 1, 4, 2, 5, empty);
		// board_->print_board();
		// board_->play_move(white, queen_w, 1, 7, 0, 7, empty);
		// board_->print_board();
		// std::cout << "Check on 3, 4: " << !board_->not_check_on_square(black, 3, 4) << std::endl;
		// std::cout << "First piece in direction: " << (char)(board_->first_piece_in_direction(3, 4, -1, 1)) << std::endl;
		// // board_->play_move(black, king_b, 2, 5, 3, 4, empty);
		// // board_->print_board();
		// turn_ = black;
		
		
		
		
		

		std::string command;
		while (board_->game_not_end()) {
			if (turn_ == pl_) {
				find_and_play_move();
			}
			else {
				do {
					if (turn_ == white) {
						std::cout << "Turn: white" << std::endl;
					} else {
						std::cout << "Turn: black" << std::endl;
					}
					std::getline(std::cin, command);
					move->clear_move();
					move->set_player(turn_);
					set_capitalization(command);
					parse_command(command, move);
				} while(!parse_command(command, move) || !board_->play_bot_move(move, true));
			}
			board_->print_board();
			change_turn();
		}
	}

private:

	player switch_turn(player pl) {
		switch(pl) {
			case white: return black;
			case black: return white;
		}
	}

	void change_turn() {
		if (turn_ == white) {
			turn_ = black;
		} else {
			turn_ = white;
		}
	}

	void set_capitalization(std::string& command) {
		if (turn_ == white) convert_lower(command);
		else convert_upper(command);
	}

	void find_and_play_move() {
		Move* move = new Move();
		auto start = std::chrono::high_resolution_clock::now();
		double a = -1*std::numeric_limits<double>::max();
		double b = std::numeric_limits<double>::max();
		double score = minimax(depth_, pl_, move, a, b);
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		move->print();
		std::cout << "Time taken by bot: " << duration.count() << " millis" << std::endl;
		std::cout << "Score of move: " << score << std::endl;
		board_->play_bot_move(move, true);
	}

	double minimax(int depth, player pl, Move* best_move, double a, double b) {
		if (depth == 0) {
			return eval();
		}
		if (!board_->game_not_end() && board_->get_stalemate()) {
			return 0.0;
		} else if (!board_->game_not_end() && !board_->get_stalemate()){
			if (board_->get_winner() == pl_) {
				return std::numeric_limits<double>::max();
				// return 1000000.0;
			} else {
				return -1*std::numeric_limits<double>::max();
				// return -1000000.0;
			}
		} else {
			Move* temp_move = new Move();
			double final_score;
			auto moves = board_->get_all_possible_moves(pl);
			if (pl == pl_) {
				double max_score = -1*std::numeric_limits<double>::max();
				double score;
				int i = 0;
				if (depth == depth_) {
					std::cout << "Move space size: " << moves.size() << std::endl;
				}
				for (auto& move: moves) {
					// if (depth == depth_) move.print();
					if (!board_->play_bot_move(&move, false)) {
						std::cout << "Bad bot move!" << std::endl;
					}
					score = minimax(depth - 1, switch_turn(pl), temp_move, a, b);
					// if (depth == depth_) std::cout << "Score: " << score << std::endl;
					board_->undo_last();
					a = std::max(a, score);
					if (score > max_score) {
						max_score = score;
						best_move->copy(move);
					}
					if (depth_ == depth) {
						i += 1;
						std::cout << i << " out of " << moves.size() << " moves explored, i.e. ";
						std::cout << ((double)i) * 100.0 / moves.size() << "\%" << std::endl;
					}
					if (a >= b) { break; }
				}
				final_score = max_score;
			} else {
				double min_score = std::numeric_limits<double>::max();
				double score;
				for (auto& move: moves) {
					if (!board_->play_bot_move(&move, false)) {
						std::cout << "Bad bot move!" << std::endl;
					}
					score = minimax(depth - 1, switch_turn(pl), temp_move, a, b);
					board_->undo_last();
					b = std::min(b, score);
					if (score < min_score) {
						min_score = score;
						best_move->copy(move);
					}
					if (a >= b) break;
				}
				final_score = min_score;
			}
			return final_score;
		}
	}

	double weight_of_check(player pl) {
		if (pl == pl_) {
			return 0.05;
		} else {
			return 20;
		}
	}

	double weight_of_piece1(piece pc) {
		switch(pl_) {
			case white: {
				switch(pc) {
					case empty: return 1;
					case king_w: return 20;
					case queen_w: return 18;
					case rook_w: return 10;
					case bishop_w: return 6.25;
					case knight_w: return 6;
					case pawn_w: return 2;
					case king_b: return 0.05;
					case queen_b: return 0.0555;
					case rook_b: return 0.1;
					case bishop_b: return 0.16;
					case knight_b: return 0.1666;
					case pawn_b: return 0.5;
				}
			}
			case black: {
				switch(pc) {
					case empty: return 1;
					case king_b: return 20;
					case queen_b: return 18;
					case rook_b: return 10;
					case bishop_b: return 6.25;
					case knight_b: return 6;
					case pawn_b: return 2;
					case king_w: return 0.05;
					case queen_w: return 0.0555;
					case rook_w: return 0.1;
					case bishop_w: return 0.16;
					case knight_w: return 0.1666;
					case pawn_w: return 0.5;
				}
			}
		}
	}

	double weight_of_piece2(piece pc) {
		switch(pl_) {
			case white: {
				switch(pc) {
					case empty: return 0;
					case king_w: return 30000;
					case queen_w: return 2521;
					case rook_w: return 1270;
					case bishop_w: return 836;
					case knight_w: return 817;
					case pawn_w: return 198;
					case king_b: return -30000;
					case queen_b: return -2521;
					case rook_b: return -1270;
					case bishop_b: return -836;
					case knight_b: return -817;
					case pawn_b: return -198;
				}
			}
			case black: {
				switch(pc) {
					case empty: return 0;
					case king_b: return 30000;
					case queen_b: return 2521;
					case rook_b: return 1270;
					case bishop_b: return 836;
					case knight_b: return 817;
					case pawn_b: return 198;
					case king_w: return -30000;
					case queen_w: return -2521;
					case rook_w: return -1270;
					case bishop_w: return -836;
					case knight_w: return -817;
					case pawn_w: return -198;
				}
			}
		}
	}

	double eval() {
		double score1 = 1.0;
		// double score2 = 0.0;
		for (int i = 0; i < board_->get_size(); i++) {
			for (int j = 0; j < board_->get_size(); j++) {
				score1 *= weight_of_piece1(board_->get_piece_at(i, j));
				// score2 += weight_of_piece2(board_->get_piece_at(i, j));
			}
		}
		score1 *= board_->white_check() ? weight_of_check(white) : 1;
		score1 *= board_->black_check() ? weight_of_check(black) : 1;
		return score1;
		// return score2;
	}

private:
	Board* board_;
	player pl_;
	player turn_{white};
	int depth_;
	bool first_turn_;
};

int main() {
	Board* board = new Board();
	Bot* bot = new Bot(board, black, 6);
	bot->start_game();
}
