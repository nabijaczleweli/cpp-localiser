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
#include <fstream>
#include <sstream>


TEST_CASE("No-arg constructor makes for empty localiser", "[constructor]") {
	std::mt19937 rand_gen{std::random_device{}()};

	const cpp_localiser::localiser loc;

	CHECK(loc.empty());

	std::uniform_int_distribution<int> char_dist(' ', '~');
	std::uniform_int_distribution<int> len_dist(1, 10);
	std::string key;
	for(auto i = 0u; i < 100u; ++i) {
		key.resize(len_dist(rand_gen));
		std::generate(key.begin(), key.end(), [&]() { return char_dist(rand_gen); });
		CHECK_FALSE(loc.can_translate_key(key));
	}
}

TEST_CASE("istream& constructor works properly", "[constructor]") {
	std::stringstream config_file(R"(
asdf=fdsa
fdsa=asdf
123.321=321.123
)");
	const cpp_localiser::localiser loc(config_file);

	CHECK_FALSE(loc.empty());
	for(const auto key : {"asdf", "fdsa", "123.321"})
		CHECK(loc.can_translate_key(key));
	for(const auto key : {std::make_pair("asdf", "fdsa"), std::make_pair("fdsa", "asdf"), std::make_pair("123.321", "321.123")})
		REQUIRE(loc.translate_key(key.first) == key.second);
}

TEST_CASE("root+lang constructor works properly", "[constructor]") {
	const auto work_dir = temp_dir + "/root-lang-ctor/";
	make_dir(work_dir);
	std::ofstream(work_dir + "en_US.lang") << R"(
blerb=blurb
blarb=blurg
567.765=765.567
)";
	std::ofstream(work_dir + "pl_PL.lang") << R"(
blerb=blurb_PL
blarb=blurg_PL
567.765=765.567_PL
)";
	const cpp_localiser::localiser loc_us(work_dir);
	const cpp_localiser::localiser loc_pl(work_dir, "pl_PL");

	CHECK_FALSE(loc_us.empty());
	CHECK_FALSE(loc_pl.empty());
	for(const auto key : {"blerb", "blarb", "567.765"})
		CHECK(loc_us.can_translate_key(key));
	for(const auto key : {std::make_pair("blerb", "blurb"), std::make_pair("blarb", "blurg"), std::make_pair("567.765", "765.567")})
		REQUIRE(loc_us.translate_key(key.first) == key.second);

	for(const auto key : {"blerb", "blarb", "567.765"})
		CHECK(loc_pl.can_translate_key(key));
	for(const auto key : {std::make_pair("blerb", "blurb_PL"), std::make_pair("blarb", "blurg_PL"), std::make_pair("567.765", "765.567_PL")})
		REQUIRE(loc_pl.translate_key(key.first) == key.second);
}

TEST_CASE("two localiser constructor works properly", "[constructor]") {
	const auto work_dir = temp_dir + "/two-localiser-ctor/";
	make_dir(work_dir);
	std::ofstream(work_dir + "en_US.lang") << R"(
blarb=blurg
)";
	std::ofstream(work_dir + "pl_PL.lang") << R"(
blarb=blurg_PL
blerb=blurb_PL
)";
	const cpp_localiser::localiser loc_us(work_dir);
	const cpp_localiser::localiser loc_pl(work_dir, "pl_PL");
	const cpp_localiser::localiser loc(loc_us, loc_pl);

	CHECK_FALSE(loc.empty());
	for(const auto key : {"blarb", "blerb"})
		CHECK(loc.can_translate_key(key));
	for(const auto key : {std::make_pair("blarb", "blurg"), std::make_pair("blerb", "blurb_PL")})
		REQUIRE(loc.translate_key(key.first) == key.second);
}
