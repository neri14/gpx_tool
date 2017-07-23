# GPX tool

[![Build Status](https://travis-ci.org/neri14/gpx_tool.svg?branch=master)](https://travis-ci.org/neri14/gpx_tool)
[![GitHub version](https://badge.fury.io/gh/neri14%2Fgpx_tool.svg)](https://badge.fury.io/gh/neri14%2Fgpx_tool)

Tool for checking if all trackpoints in gpx file are consecutive and if not - correcting them.

### Requirements

- libxml2

### Usage

**gpx_tool check filename** - checks if all trackpoints in provided file are consecutive

**gpx_tool fix input_file output_file** - reads input_file and writes corrected file to output_file

