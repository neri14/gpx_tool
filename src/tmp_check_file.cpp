#include "tmp_check_file.h"

#include <tmp_consts.h>
#include <tmp_get_nodes.h>
#include <tmp_get_time.h>

#include <libxml/parser.h>
#include <iostream>
#include <list>
#include <ctime>

bool check_timestamps(xmlDocPtr doc, std::list<xmlNode*> points, int expected_diff)
{
	if (points.size() == 0) {
		std::cerr << "nothing to check" << std::endl;
		return true;
	}

	int out_of_order = 0;
	std::time_t last_timestamp = get_time(doc, *points.begin()) - expected_diff;

	for (xmlNode* point : points) {
		std::time_t t = get_time(doc, point);

		int diff = t - last_timestamp;
		if (diff != expected_diff) {
			out_of_order++;
		}

		last_timestamp = t;
	}

	std::cout << "entries out of order: " << out_of_order << std::endl;
	return out_of_order == 0;
}

bool check_file(std::string file)
{
	std::cout << "=== CHECKING FILE <" << file << "> ===" << std::endl;

	xmlDocPtr doc;
	doc = xmlReadFile(file.c_str(), NULL, 0);
	if (doc == NULL) {
		std::cerr << "Failed to parse " << file << std::endl;
		exit(2);
	}

	xmlNode* root = xmlDocGetRootElement(doc);

	std::list<xmlNode*> segments = get_segments(root);
	std::cout << segments.size() << " segment(s) found" << std::endl;

	int i = 0;
	bool result = true;
	for (xmlNode* segment : segments) {
		std::cout << "parsing segment " << ++i << std::endl;
		std::list<xmlNode*> track_points = get_trackpoints(segment);
		std::cout << track_points.size() << " trackpoints(s) found" << std::endl;

		result = check_timestamps(doc, track_points, constant::EXPECTED_DIFF) && result;
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();

	return result;
}