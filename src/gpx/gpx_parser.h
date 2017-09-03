#ifndef GPX_PARSER_H
#define GPX_PARSER_H

#include <memory>
#include <gpx/gpx_file.h>

namespace gpx {

class gpx_parser
{
public:
	virtual ~gpx_parser() = default;
	virtual std::shared_ptr<gpx_file> read_gpx(std::string filepath) = 0;
	virtual void write_gpx(std::string filepath, std::shared_ptr<gpx_file> file) = 0;
};

}

#endif /* GPX_PARSER_H */