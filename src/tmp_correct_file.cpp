#include "tmp_correct_file.h"

#include <tmp_consts.h>
#include <tmp_get_nodes.h>
#include <tmp_get_time.h>

#include <libxml/parser.h>
#include <libxml/xmlsave.h>
#include <iostream>
#include <sstream>
#include <list>
#include <ctime>
#include <iomanip>

void set_time(xmlDocPtr doc, xmlNode* point, std::string time_to_set)
{
	xmlNode * t_node = point->children;
	while (t_node && xmlStrcmp(t_node->name, xmlCharStrdup("time")) != 0) {
		t_node = t_node->next;
	}

	if (t_node) {
		xmlNodeSetContent(t_node, xmlCharStrdup(time_to_set.c_str()));
	} else {
		std::cerr << "no time for point" << std::endl;
		exit(1);
	}
}

std::string convert(std::time_t t)
{
	std::stringstream ss;
	ss << std::put_time(std::gmtime(&t), constant::TIME_FORMAT);
	return ss.str();
}

void correct_timestamps(xmlDocPtr doc, std::list<xmlNode*> points, int expected_diff)
{
	if (points.size() == 0) {
		std::cerr << "nothing to correct" << std::endl;
		return;
	}

	int max_abs_diff = 0;
	int corrected = 0;

	std::time_t expected_timestamp = get_time(doc, *points.begin());
	for (xmlNode* point : points) {
		std::time_t t = get_time(doc, point);

		int diff = t - expected_timestamp;
		if (diff != 0) {
			std::string expected_time_string = convert(expected_timestamp);
			std::string actual_time_string = convert(t);
			// std::cout << "error found - correcting: " << actual_time_string << " -> " << expected_time_string << std::endl;

			set_time(doc, point, expected_time_string);

			max_abs_diff = max_abs_diff < abs(diff) ? abs(diff) : max_abs_diff;
			corrected++;
		}

		expected_timestamp += expected_diff;
	}

	std::cout << "corrected: " << corrected << std::endl;
	std::cout << "max abs diff: " << max_abs_diff << std::endl;
}

bool correct_file(std::string input_file, std::string output_file)
{
	std::cout << "=== CORRECTING FILE <" << input_file << "> ===" << std::endl;

	xmlDocPtr doc;
	doc = xmlReadFile(input_file.c_str(), NULL, 0);
	if (doc == NULL) {
		std::cerr << "Failed to parse " << input_file << std::endl;
		exit(2);
	}

	xmlNode* root = xmlDocGetRootElement(doc);

	std::list<xmlNode*> segments = get_segments(root);
	std::cout << segments.size() << " segment(s) found" << std::endl;

	int i = 0;
	for (xmlNode* segment : segments) {
		std::cout << "parsing segment " << ++i << std::endl;
		std::list<xmlNode*> track_points = get_trackpoints(segment);
		std::cout << track_points.size() << " trackpoints(s) found" << std::endl;

		correct_timestamps(doc, track_points, constant::EXPECTED_DIFF);
	}

	xmlSaveCtxt *ctxt = xmlSaveToFilename(output_file.c_str(), "UTF-8", XML_SAVE_FORMAT | XML_SAVE_NO_EMPTY);
	if (!ctxt || xmlSaveDoc(ctxt, doc) < 0 || xmlSaveClose(ctxt) < 0){
		std::cerr << "error saving file" << std::endl;
		exit(3);
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();

	return true;
}