#include <command_parser.h>

#include <tmp_check_file.h>
#include <tmp_correct_file.h>
#include <tmp_consts.h>

#include <libxml_wrapper/libxml_gpx_parser.h>
#include <helpers/printer.h>

enum EReturnCode {
	EReturnCode_OK = 0,
	EReturnCode_IncorrectCommand = 1,
	EReturnCode_CheckFailed = 2,
	EReturnCode_FixFailed = 3,
	EReturnCode_UnsupportedCommand = 4
};

void tmp_main()
{
	LOG_WARNING("running temporary main");

	libxml_wrapper::libxml_gpx_parser libxml_parser;
	gpx::gpx_parser& parser = libxml_parser;

	auto file = parser.read_gpx("test");
	parser.write_gpx("test", file);
}

int main(int argc, char **argv)
{
	if (argc = 1) {
		tmp_main();
		return 0;
	}

	gpx::command_parser cmd_parser;
	cmd_parser.parse(argc, argv);

	auto cmd = cmd_parser.get_command();
	const std::vector<std::string>& args = cmd_parser.get_arguments();

	EReturnCode ret_code = EReturnCode_OK;

	if (!cmd) {
		return EReturnCode_IncorrectCommand;
	}

	switch (*cmd) {
		case gpx::ECommand::Check:
		ret_code = check_file(args[0]) ?
		    EReturnCode_OK :
		    EReturnCode_CheckFailed;

		break;

		case gpx::ECommand::Fix:
		ret_code = correct_file(args[0], args[1]) ?
		    EReturnCode_OK :
		    EReturnCode_FixFailed;
		break;

		default:
		ret_code = EReturnCode_UnsupportedCommand;
		break;
	}

	return ret_code;
}
