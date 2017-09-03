#include "libxml_gpx_parser.h"

#include <helpers/not_implemented.h>

namespace libxml_wrapper {

std::shared_ptr<gpx::gpx_file> libxml_gpx_parser::read_gpx(std::string filepath)
{
	NOT_IMPLEMENTED;

	(void)(filepath);
	return std::shared_ptr<gpx::gpx_file>();
}

void libxml_gpx_parser::write_gpx(std::string filepath, std::shared_ptr<gpx::gpx_file> file)
{
	NOT_IMPLEMENTED;

	(void)(filepath);
	(void)(file);
}

}