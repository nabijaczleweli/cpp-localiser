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


#include "cpp-localiser.hpp"
#include "util/string.hpp"
#include <fstream>


using namespace cpp_localiser;


static inline const char * locale_midfix(const std::string & localisation_root) {
	if(localisation_root.empty() || localisation_root.back() == '/' || localisation_root.back() == '\\')
		return "";
	else
		return "/";
}


// Can't be defaulted for some reason?
localiser::localiser() {}

localiser::localiser(std::istream & in) {
	init(in);
}

localiser::localiser(const std::string & localisation_root, const std::string & locale) {
	std::ifstream in(localisation_root + locale_midfix(localisation_root) + locale + ".lang");
	if(in.is_open())
		init(in);
	end = language.cend();
}

localiser::localiser(const localiser & loc0, const localiser & loc1) : localiser(loc0) {
	merge(loc1);
}

/** Adds keys from `loc` for which there are no elements in `this` */
localiser & localiser::merge(const localiser & loc) {
	language.insert(loc.language.begin(), loc.language.cend());
	end = language.cend();
	return *this;
}

localiser & localiser::open(const std::string & localisation_root, const std::string & locale) {
	return merge(localiser(localisation_root, locale));
}

localiser & localiser::swap(localiser & with) {
	language.swap(with.language);
	end      = language.cend();
	with.end = with.language.cend();
	return *this;
}

void localiser::init(std::istream & in) {
	for(std::string line; getline(in, line);) {
		if(line.empty())
			continue;
		ltrim(line);
		if(line[0] == '#')
			continue;
		const auto idx = line.find('=');
		if(idx == std::string::npos)
			continue;
		language.emplace(rtrim(line.substr(0, idx)), trim(line.substr(idx + 1)));
	}
	end = language.cend();
}

bool localiser::empty() const {
	return language.empty();
}

bool localiser::can_translate_key(const std::string & key) const {
	return !language.empty() && language.count(key) > 0;
}

const std::string & localiser::translate_key(const std::string & key) const {
	const auto & itr = language.find(key);
	if(itr == end)
		return key;
	else
		return itr->second;
}

void std::swap(localiser & lhs, localiser & rhs) {
	lhs.swap(rhs);
}
