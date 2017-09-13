#include "libxml_gpx_file.h"

namespace libxml_wrapper {

libxml_gpx_file::libxml_gpx_file()
{}

void libcml_gpx_file::open(std::string filepath)
{
	doc = xmlReadFile(filepath.c_str(), constant::encoding, constant::options);

	if (nullptr == doc) {
		LOG_ERROR(std::string("Failed to parse ") + filepath);
		return nullptr;
	}
}

virtual ~libxml_gpx_file();

}