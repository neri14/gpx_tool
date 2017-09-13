#include "libxml_gpx_parser.h"

#include <helpers/not_implemented.h>
#include <helpers/printer.h>

namespace libxml_wrapper {
namespace constant {
const char* encoding(nullptr);
const int options(0)
}

std::shared_ptr<gpx::gpx_file> libxml_gpx_parser::read_gpx(std::string filepath)
{
	xmlDocPtr doc = xmlReadFile(filepath.c_str(), constant::encoding, constant::options);

	if (nullptr == doc) {
		LOG_ERROR(std::string("Failed to parse ") + filepath);
		return nullptr;
	}

	return std::make_shared<libxml_gpx_file>(doc);
}

void libxml_gpx_parser::write_gpx(std::string filepath, std::shared_ptr<gpx::gpx_file> file)
{
	NOT_IMPLEMENTED;

	(void)(filepath);
	(void)(file);
}

}