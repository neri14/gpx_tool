#include <cmath>
#include <iostream>
#include <fstream>
#include <list>
#include <cstring>
#include <string>
#include <deque>
#include <iomanip>
#include <ctime>
#include <stdlib.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlsave.h>

const int EXPECTED_DIFF = 1;
const char* TIME_FORMAT = "%Y-%m-%dT%H:%M:%SZ";
//const std::string OUTPUT_FILE = "out.gpx";

std::list<xmlNode*> get_all_nodes_named(xmlNode *node, std::string name)
{
	std::list<xmlNode*> segments;
	xmlNode *cur_node = NULL;

	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if(xmlStrcmp(cur_node->name, xmlCharStrdup(name.c_str())) == 0) {
				segments.push_back(cur_node);
			} else {
				segments.splice(segments.end(), get_all_nodes_named(cur_node->children, name));
			}
		}
	}

	return segments;
}

std::list<xmlNode*> get_segments(xmlNode *node)
{
	return get_all_nodes_named(node, "trkseg");
}

std::list<xmlNode*> get_trackpoints(xmlNode *node)
{
	return get_all_nodes_named(node, "trkpt");
}

std::time_t get_time(xmlDocPtr doc, xmlNode* point)
{
	xmlNode * t_node = point->children;
	while (t_node && xmlStrcmp(t_node->name, xmlCharStrdup("time")) != 0) {
		t_node = t_node->next;
	}

	if (t_node) {
		std::string time_str((char*)xmlNodeListGetString(doc, t_node->xmlChildrenNode, 1));
		// std::cout << time_str << std::endl;

		std::tm tm;
		std::istringstream ss(time_str);
		ss >> std::get_time(&tm, TIME_FORMAT);//2017-03-08T15:04:34Z
		return timegm(&tm);
	} else {
		std::cerr << "no time for point" << std::endl;
		exit(1);
	}
}

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
	ss << std::put_time(std::gmtime(&t), TIME_FORMAT);
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

void correct_file(std::string input_file, std::string output_file)
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

		correct_timestamps(doc, track_points, EXPECTED_DIFF);
	}

	xmlSaveCtxt *ctxt = xmlSaveToFilename(output_file.c_str(), "UTF-8", XML_SAVE_FORMAT | XML_SAVE_NO_EMPTY);
	if (!ctxt || xmlSaveDoc(ctxt, doc) < 0 || xmlSaveClose(ctxt) < 0){
		std::cerr << "error saving file" << std::endl;
		exit(3);
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();
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

		result = check_timestamps(doc, track_points, EXPECTED_DIFF) && result;
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();

	return result;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "missing command";
		return 1;
	}
	std::string command(argv[1]);

	if (command == "check") {
		if (argc < 3) {
			std::cerr << "missing input filename" << std::endl;
			return 1;
		}
		std::string input_file(argv[2]);

		if (!check_file(input_file)) {
			return 1;
		}
	} else if (command == "fix") {
		if (argc < 4){
			std::cerr << "missing input and or output filename" << std::endl;
			return 1;
		}
		std::string input_file(argv[2]);
		std::string output_file(argv[3]);

		correct_file(input_file, output_file);
	}

	return 0;
}
