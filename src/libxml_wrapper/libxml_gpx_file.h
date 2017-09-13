#ifndef LIBXML_GPX_FILE_H
#define LIBXML_GPX_FILE_H

#include <gpx/gpx_file.h>

namespace libxml_wrapper {

class libxml_gpx_file : public gpx::gpx_file
{
public:
	libxml_gpx_file(xmlDocPtr doc_);
	virtual ~libxml_gpx_file();

private:
	xmlDocPtr doc;
};

}

#endif /* LIBXML_GPX_FILE_H */