#ifndef LIBXML_GPX_PARSER_H
#define LIBXML_GPX_PARSER_H

#include <gpx/gpx_parser.h>
#include <libxml_wrapper/libxml_gpx_file.h>

namespace libxml_wrapper {

class libxml_gpx_parser : public gpx::gpx_parser
{
public:
	virtual std::shared_ptr<gpx::gpx_file> read_gpx(std::string filepath) override;
	virtual void write_gpx(std::string filepath, std::shared_ptr<gpx::gpx_file> file) override;
};

}

#endif /* LIBXML_GPX_PARSER_H */