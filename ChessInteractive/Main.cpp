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
		player_to_move = Player::White;
	}

	void print_state()
	{
		printf("=============================\n");
		printf("%s to move:", Utils::get_player_name(player_to_move));
		Utils::print_board(current_board);
	}

	void print_command_list()
	{
		printf("e : engine move\n");
		printf("ep : engine move (preview)\n");
		printf("p : player move\n");
		printf("m : move list\n");
		printf("u : undo\n");
		printf("q : Quit\n");
		printf("fen : load from fen, args: fen\n");
		printf("perft : perft, args: depth\n");
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
			move_gen(current_board, player_to_move, results);

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

			auto pr = Utils::perft(current_board, player_to_move, depth);

			printf("Perft d=%d\n", depth);
			printf("Nodes: %d\n", pr.nodes);
			printf("Captures: %d\n", pr.captures);
			printf("En Passants: %d\n", pr.en_passants);
			printf("Castles: %d\n", pr.castles);
		}
		else if (args[0] == "e")
		{
			int depth = 5;

			if (args.size() == 2)
				depth = std::stoi(args[1]);

			auto sr = search(current_board, player_to_move, depth, true);
			printf("d=%d, score=%f, %s->%s\n", depth, sr.score, Utils::get_square_name(sr.move.sq_from), Utils::get_square_name(sr.move.sq_to));

			undo_stack.push_back(current_board);
			current_board = sr.next;
			player_to_move = Utils::opposite_player(player_to_move);
		}
		else if (args[0] == "ep")
		{
			int depth = 5;

			if (args.size() == 2)
				depth = std::stoi(args[1]);

			auto sr = search(current_board, player_to_move, depth, true);
			printf("d=%d, score=%f, %s->%s\n", depth, sr.score, Utils::get_square_name(sr.move.sq_from), Utils::get_square_name(sr.move.sq_to));
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
			player_to_move = Utils::opposite_player(player_to_move);
		}
		else if (args[0] == "p")
		{
			if (args.size() != 3 || args[1].size() != 2 || args[2].size() != 2)
			{
				printf("Usage: p <from> <to>\n");
				return;
			}

			int fromFile = tolower(args[1][0]) - 'a';
			int fromRank = tolower(args[1][1]) - '1';
			int toFile = tolower(args[2][0]) - 'a';
			int toRank = tolower(args[2][1]) - '1';

			int fromSq = mailbox64[fromRank * 8 + fromFile];
			int toSq = mailbox64[toRank * 8 + toFile];

			Board new_board = current_board;
			new_board.sq[fromSq] = Piece::None;
			new_board.sq[toSq] = current_board.sq[fromSq];

			undo_stack.push_back(current_board);
			current_board = new_board;
			player_to_move = Utils::opposite_player(player_to_move);
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
	Player player_to_move;
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