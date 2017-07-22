#include "command_parser.h"

#include <map>
#include <iostream>

namespace gpx {
namespace constant {

const std::map<std::string, ECommand> command_map =
    {{"check", ECommand::Check},
     {"fix", ECommand::Fix}};

const std::map<ECommand, int> arg_count =
    {{ECommand::Check, 1},
     {ECommand::Fix, 2}};

}

void command_parser::parse(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "Command not specified" << std::endl;
		print_usage(std::string(argv[0]));
		return;
	}

	command = parse_command(std::string(argv[1]));
	if (!command) {
		std::cerr << "Unrecognized command" << std::endl;
		print_usage(std::string(argv[0]));
		return;
	}

	int expected_args = constant::arg_count.at(*command);

	if (argc != expected_args + 2) {
		std::cerr << "Incorrect number of arguments for command" << std::endl;
		print_usage(std::string(argv[0]));
		return;
	}

	for (int i = 0; i < expected_args; ++i) {
		arguments.push_back(std::string(argv[i + 2]));
	}
}

std::experimental::optional<ECommand> command_parser::get_command() const
{
	return command;
}

const std::vector<std::string>& command_parser::get_arguments() const
{
	return arguments;
}

void command_parser::print_usage(const std::string& name) const
{
	std::cerr << "Usage:" << std::endl;
	std::cerr << name << " check input_file" << std::endl;
	std::cerr << name << " fix input_file output_file" << std::endl;
}

std::experimental::optional<ECommand> command_parser::parse_command(std::string str) const
{
	auto it = constant::command_map.find(str);
	if (it != constant::command_map.end()) {
		return it->second;
	}
	return std::experimental::optional<ECommand>();
}

}