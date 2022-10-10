#include <iostream>
#include <string>

#include <Chess/MoveGen.h>
#include <Chess/Search.h>
#include <Chess/Utils.h>

int main()
{
	Board board = Utils::get_start_position();

	while (true)
	{
		std::string input;
		std::getline(std::cin, input);

		auto end_of_command = input.find_first_of(" \n");
		std::string command = input.substr(0, end_of_command);

		if (command == "uci")
		{
			printf("uciok\n");
		}
		else if (command == "debug")
		{

		}
		else if (command == "isready")
		{
			printf("readyok\n");
		}
		else if (command == "setoption")
		{

		}
		else if (command == "ucinewgame")
		{
			
		}
		else if (command == "position")
		{
			std::vector<std::string> subcommands;
			auto end_of_subcommand = end_of_command;
			while (end_of_subcommand != std::string::npos)
			{
				auto x = input.find_first_of(" \n", end_of_subcommand + 1);
				subcommands.push_back(input.substr(end_of_subcommand + 1, x - end_of_subcommand - 1));
				end_of_subcommand = x;
			}

			if (subcommands[0] != "startpos")
			{
				printf("info string Error: position only supports startpos, got %s\n", command.c_str());
				exit(-1);
			}

			board = Utils::get_start_position();

			if (subcommands.size() > 1)
			{
				for (int i = 2; i < subcommands.size(); i++)
				{
					const auto& move_str = subcommands[i];

					std::vector<MoveGenResult> results;
					move_gen(board, results);

					int fromFile = tolower(move_str[0]) - 'a';
					int fromRank = tolower(move_str[1]) - '1';
					int toFile = tolower(move_str[2]) - 'a';
					int toRank = tolower(move_str[3]) - '1';

					int fromSq = mailbox64[fromRank * 8 + fromFile];
					int toSq = mailbox64[toRank * 8 + toFile];

					bool is_promotion = move_str.length() == 5;
					uint8_t promotion_piece = PieceTypeInvalid;
					if (is_promotion)
					{
						if (move_str[4] == 'q') promotion_piece = PieceTypeQueen;
						else if (move_str[4] == 'b') promotion_piece = PieceTypeBishop;
						else if (move_str[4] == 'n') promotion_piece = PieceTypeKnight;
						else if (move_str[4] == 'r') promotion_piece = PieceTypeRook;
						else
						{
							printf("info string Error: invalid promotion type while processing move=%s\n", move_str.c_str());
							exit(-1);
						}
					}

					const MoveGenResult* found = nullptr;

					for (const auto& mgr : results)
					{
						if (mgr.sq_to == toSq && mgr.sq_from == fromSq)
						{
							if (is_promotion)
							{
								if ((mgr.board.sq[mgr.sq_to] & PieceTypeMask) == promotion_piece)
									found = &mgr;
							}
							else
							{
								found = &mgr;
							}
						}
					}

					if (!found)
					{
						printf("info string Error: could not apply move while processing position command. move=%s\n", move_str.c_str());

						for (int i = 0; i < results.size(); i++)
						{
							printf("info string available: %d. %s%s->%s %s\n",
								i + 1,
								i < 9 ? " " : "",
								Utils::get_square_name(results[i].sq_from),
								Utils::get_square_name(results[i].sq_to),
								Utils::get_move_type_as_string(results[i].move_type)
							);
						}
						exit(-1);
					}

					board = found->board;
				}
			}
		}
		else if (command == "go")
		{
			auto sr = search(board, 5, true);
			if (sr.move.move_type == MoveType::Promotion || sr.move.move_type == MoveType::CapturePromotion)
			{
				char promotion_piece = 'q';
				if (sr.move.board.sq[sr.move.sq_to] & PieceTypeMask == PieceTypeQueen) promotion_piece = 'q';
				else if (sr.move.board.sq[sr.move.sq_to] & PieceTypeMask == PieceTypeBishop) promotion_piece = 'b';
				else if (sr.move.board.sq[sr.move.sq_to] & PieceTypeMask == PieceTypeKnight) promotion_piece = 'n';
				else if (sr.move.board.sq[sr.move.sq_to] & PieceTypeMask == PieceTypeRook) promotion_piece = 'r';
				printf("bestmove %s%s%c\n", Utils::get_square_name(sr.move.sq_from), Utils::get_square_name(sr.move.sq_to), promotion_piece);	
			}
			else
			{
				printf("bestmove %s%s\n", Utils::get_square_name(sr.move.sq_from), Utils::get_square_name(sr.move.sq_to));
			}
		}
		else if (command == "stop")
		{
			
		}
		else if (command == "quit")
		{
			exit(0);
		}
		else
		{
			printf("info string Error: unknown command %s\n", command.c_str());
			printf("info string args: %s\n", input.c_str());
			exit(0);
		}
	}
}