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


#include "catch.hpp"
#include "cpp-localiser.hpp"
#include "main.hpp"
#include <sstream>


TEST_CASE("Translation of nonexistant key returns it", "[functionality]") {
	std::mt19937 rand_gen{std::random_device{}()};

	const cpp_localiser::localiser loc;

	std::uniform_int_distribution<int> char_dist(' ', '~');
	std::uniform_int_distribution<int> len_dist(1, 10);
	std::string key;
	for(auto i = 0u; i < 100u; ++i) {
		key.resize(len_dist(rand_gen));
		std::generate(key.begin(), key.end(), [&]() { return char_dist(rand_gen); });
		REQUIRE(loc.translate_key(key) == key);
	}
}

TEST_CASE("Translation of existing key returns value", "[functionality]") {
	std::stringstream config_file(R"(
asdf=fdsa
fdsa=asdf
)");
	const cpp_localiser::localiser loc(config_file);

	REQUIRE(loc.translate_key("asdf") == "fdsa");
	REQUIRE(loc.translate_key("fdsa") == "asdf");
}
