#pragma once
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <ctype.h>
#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <string>

using tokenizer = boost::tokenizer<boost::char_separator<char>>;
const std::unordered_map<char, int> x_index_map{{'1', 7}, {'2', 6}, {'3', 5}, {'4', 4}, {'5', 3},
		{'6', 2}, {'7', 1}, {'8', 0}};
const std::unordered_map<char, int> y_index_map{{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4},
		{'f', 5}, {'g', 6}, {'h', 7}, {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5},
		{'G', 6}, {'H', 7}};

bool set_piece(std::string pc_str, Move* move) {
	if (pc_str.length() != 1) { return false; }
	move->set_pc(piece(pc_str.at(0)));
	return true;
}

bool set_coord(std::string human_coord, Move* move, bool from) {
	int y;
	int x;
	if (human_coord.length() != 2) { return false; }
	boost::to_lower(human_coord);
	switch(human_coord.at(0)) {
		case 'a': y = 0; break;
		case 'b': y = 1; break;
		case 'c': y = 2; break;
		case 'd': y = 3; break;
		case 'e': y = 4; break;
		case 'f': y = 5; break;
		case 'g': y = 6; break;
		case 'h': y = 7; break;
		default: return false;
	}
	if (!std::isdigit(human_coord.at(1))) { return false; }
	x = 8 - std::atoi(&human_coord.at(1));
	if (x > 7 || x < 0) { return false; }
	
	if (from) move->set_from(x, y);
	else move->set_to(x, y);
	return true;
}

bool set_convert_to(std::string convert_to_str, Move* move) {
	if (convert_to_str.length() != 1) { return false; }
	move->set_convert_to(piece(convert_to_str.at(0)));
	return true;
}

bool parse_command(std::string command, Move* move) {
	std::vector<std::string> tokens;
	boost::split(tokens, command, boost::is_any_of(" ,:"));
	// for (auto tok: tokens) {
	// 	std::cout << tok << " ";
	// }
	// std::cout << std::endl;
	// std::cout << "Size: " << tokens.size() << std::endl;
	if (tokens.size() != 3 && tokens.size() != 4) { return false; }
	// std::cout << "Move after size check: ";
	// move->print_move();
	if (!set_piece(tokens.at(0), move)) { return false; }
	// std::cout << "Move after set_piece: ";
	// move->print_move();
	if (!set_coord(tokens.at(1), move, true)) { return false; }
	// std::cout << "Move after set_coord from: ";
	// move->print_move();
	if (!set_coord(tokens.at(2), move, false)) { return false; }
	// std::cout << "Move after set_coord to: ";
	// move->print_move();
	if ( tokens.size() == 4 && !set_convert_to(tokens.at(3), move)) { return false; }
	// std::cout << "Move after set_convert_to: ";
	// move->print_move();
	return true;
}

void convert_upper(std::string& command) {
	boost::to_upper(command);
}

void convert_lower(std::string& command) {
	boost::to_lower(command);
}