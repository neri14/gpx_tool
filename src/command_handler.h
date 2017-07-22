#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <experimental/optional>
#include <vector>

namespace gpx {

enum class ECommand {
	Check,
	Fix
};

class command_handler
{
public:
	void parse(int argc, char **argv);
	std::experimental::optional<ECommand> get_command() const;
	const std::vector<std::string>& get_arguments() const;

private:
	void print_usage(const std::string& name) const;
	std::experimental::optional<ECommand> parse_command(std::string str) const;

	std::experimental::optional<ECommand> command;
	std::vector<std::string> arguments;
};

} // namespace gpx

#endif /* COMMAND_HANDLER_H */