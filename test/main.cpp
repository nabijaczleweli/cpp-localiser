// The MIT License (MIT)

// Copyright (c) 2016 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "main.hpp"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cstdlib>
#include <random>


static inline std::string OS_temp_dir() {
	if(const auto tmp = std::getenv("TMP"))
		return tmp;
	else if(const auto temp = std::getenv("TEMP"))
		return temp;
	else
		return "/tmp";
}


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <direct.h>
#include <windows.h>


std::string temp_dir = []() -> std::string {
	char buf[MAX_PATH + 2];
	GetLongPathName(OS_temp_dir().c_str(), buf, sizeof buf);
	return buf;
}() + "/cpp-localiser-test";

static const auto make_dir_func = [](const auto & path) { _mkdir(path.c_str()); };
#else
#include <sys/stat.h>
#include <sys/types.h>


std::string temp_dir = OS_temp_dir() + "/cpp-localiser-test";


static const auto make_dir_func = [](const auto & path) { mkdir(path.c_str(), 0733); };
#endif


void make_dir(const std::string & path) {
	std::size_t idx = 1;
	do {
		idx = path.find_first_of("/\\", idx + 1);
		make_dir_func(path.substr(0, idx));
	} while(idx != std::string::npos);
}
