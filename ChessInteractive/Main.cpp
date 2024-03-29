#include <iostream>
#include <vector>
#include <string>

#include <Chess/MoveGen.h>
#include <Chess/Search.h>
#include <Chess/Utils.h>

static std::vector<std::string> split_args(const std::string &text, char sep)
{
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos)
	{
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

class ChessRepl
{
public:
	ChessRepl()
	{
		current_board = Utils::get_start_position();
	}

	void print_state()
	{
		printf("=============================\n");
		printf("%s to move:", Utils::get_player_name(current_board.playerToMove()));
		Utils::print_board(current_board);
	}

	void print_command_list()
	{
		printf("e [depth]     : engine move\n");
		printf("ep [depth]    : engine move (preview)\n");
		printf("p [from] <to> : player move\n");
		printf("m             : move list\n");
		printf("u             : undo\n");
		printf("q             : quit\n");
		printf("fen <fen>     : load from fen\n");
		printf("perft <depth> : perft\n");
		printf("\n> ");
	}

	void execute_command(const std::string& command)
	{
		auto args = split_args(command, ' ');
		if (args.empty() || (args.size() == 1 && args[0] == ""))
			return;

		if (args[0] == "q")
			exit(0);
		else if (args[0] == "m")
		{
			std::vector<MoveGenResult> results;
			move_gen(current_board, results);

			for (int i = 0; i < results.size(); i++)
			{
				printf("%d. %s%s->%s %s\n",
					i + 1,
					i < 9 ? " " : "",
					Utils::get_square_name(results[i].sq_from),
					Utils::get_square_name(results[i].sq_to),
					Utils::get_move_type_as_string(results[i].move_type)
				);
			}
		}
		else if (args[0] == "perft")
		{
			if (args.size() != 2)
			{
				printf("Usage: perft <depth>\n");
				return;
			}

			int depth = std::stoi(args[1]);

			if (depth < 1)
			{
				printf("Invalid depth\n");
				return;
			}

			auto pr = Utils::perft(current_board, depth);

			printf("Perft d=%d\n", depth);
			printf("Nodes: %d\n", pr.nodes);
			printf("Captures: %d\n", pr.captures);
			printf("En Passants: %d\n", pr.en_passants);
			printf("Castles: %d\n", pr.castles);
			printf("Promotions: %d\n", pr.promotions);
		}
		else if (args[0] == "e")
		{
			int depth = 5;

			if (args.size() == 2)
				depth = std::stoi(args[1]);

			auto sr = search(current_board, depth, true);
			printf("d=%d, score=%s, %s->%s\n", depth, Utils::eval_score_to_string(sr.score).c_str(), Utils::get_square_name(sr.move.sq_from), Utils::get_square_name(sr.move.sq_to));

			undo_stack.push_back(current_board);
			current_board = sr.next;
		}
		else if (args[0] == "ep")
		{
			int depth = 5;

			if (args.size() == 2)
				depth = std::stoi(args[1]);

			auto sr = search(current_board, depth, true);
			printf("d=%d, score=%s, %s->%s\n", depth, Utils::eval_score_to_string(sr.score).c_str(), Utils::get_square_name(sr.move.sq_from), Utils::get_square_name(sr.move.sq_to));
		}
		else if (args[0] == "u")
		{
			if (undo_stack.empty())
			{
				printf("Nothing to undo\n");
				return;
			}
			current_board = undo_stack.back();
			undo_stack.pop_back();
		}
		else if (args[0] == "p")
		{
			if (args.size() < 2 || args.size() > 3)
			{
				printf("Usage: p [from] <to>\n");
				return;
			}

			std::vector<MoveGenResult> results;
			move_gen(current_board, results);
			const MoveGenResult* found = nullptr;

			if (args.size() == 2)
			{
				if (args[1].size() != 2)
				{
					printf("Usage: p [from] <to>\n");
					return;
				}

				int toFile = tolower(args[1][0]) - 'a';
				int toRank = tolower(args[1][1]) - '1';

				int toSq = mailbox64[toRank * 8 + toFile];

				for (const auto& mgr : results)
				{
					if (mgr.sq_to == toSq)
					{
						if (found)
						{
							printf("Ambiguous move, enter both squares\n");
							return;
						}
						else
						{
							found = &mgr;
						}
					}
				}
			}
			else if (args.size() == 3)
			{
				if (args[1].size() != 2 || args[2].size() != 2)
				{
					printf("Usage: p [from] <to>\n");
					return;
				}

				int fromFile = tolower(args[1][0]) - 'a';
				int fromRank = tolower(args[1][1]) - '1';
				int toFile = tolower(args[2][0]) - 'a';
				int toRank = tolower(args[2][1]) - '1';

				int fromSq = mailbox64[fromRank * 8 + fromFile];
				int toSq = mailbox64[toRank * 8 + toFile];

				for (const auto& mgr : results)
				{
					if (mgr.sq_to == toSq && mgr.sq_from == fromSq)
					{
						found = &mgr;
						break;
					}
				}
			}

			if (!found)
			{
				printf("Move not found!\n");
				return;
			}

			undo_stack.push_back(current_board);
			current_board = found->board;
		}
		else if (args[0] == "fen")
		{
			if (args.size() == 1)
			{
				printf("Usage: fen <fen>\n");
				return;
			}

			auto fen = command.substr(4);

			Board new_board = Utils::get_board_from_fen(fen);
			undo_stack.push_back(current_board);
			current_board = new_board;
		}
		else
			std::cout << "Unknown command: " << command << std::endl;
	}

private:
	Board current_board;
	std::vector<Board> undo_stack;
};

int main()
{
	ChessRepl cr;

	while (true)
	{
		cr.print_state();
		cr.print_command_list();

		std::string input;
		std::getline(std::cin, input);

		printf("\033c");

		cr.execute_command(input);
	}
}