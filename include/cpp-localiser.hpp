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


#pragma once


#include <istream>
#include <map>
#include <string>


namespace cpp_localiser {
	class localiser {
	private:
		using map_t = std::map<std::string, std::string>;

		map_t language;
		mutable map_t::const_iterator end;

		void init(std::istream & from);

	public:
		localiser();
		explicit localiser(std::istream & from);
		explicit localiser(const std::string & localisation_root, const std::string & locale = "en_US");
		/** Turns out to be loc0.merge(loc1) */
		explicit localiser(const localiser & loc0, const localiser & loc1);

		/** Adds keys from `loc` for which there are no elements in `this` */
		localiser & merge(const localiser & loc);
		localiser & open(const std::string & localisation_root, const std::string & locale = "en_US");
		localiser & swap(localiser & with);

		bool empty() const;
		bool can_translate_key(const std::string & key) const;

		const std::string & translate_key(const std::string & key) const;
	};
}

namespace std {
	void swap(cpp_localiser::localiser & lhs, cpp_localiser::localiser & rhs);
}
