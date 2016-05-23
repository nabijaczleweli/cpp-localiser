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


TEST_CASE("merge() properly doesn't replace old keys", "[util]") {
	std::stringstream first_config_file(R"(
asdf=1
)");
	std::stringstream second_config_file(R"(
asdf=2
)");
	cpp_localiser::localiser loc(first_config_file);
	loc.merge(cpp_localiser::localiser(second_config_file));

	CHECK_FALSE(loc.empty());
	CHECK(loc.can_translate_key("asdf"));
	REQUIRE(loc.translate_key("asdf") == "1");
}

TEST_CASE("merge() adds new keys", "[util]") {
	std::stringstream first_config_file(R"(
asdf=1
)");
	std::stringstream second_config_file(R"(
fdsa=2
)");
	cpp_localiser::localiser loc(first_config_file);
	loc.merge(cpp_localiser::localiser(second_config_file));

	CHECK_FALSE(loc.empty());
	for(const auto key : {"asdf", "fdsa"})
		CHECK(loc.can_translate_key(key));
	for(const auto key : {std::make_pair("asdf", "1"), std::make_pair("fdsa", "2")})
		REQUIRE(loc.translate_key(key.first) == key.second);
}

TEST_CASE("member swap() swaps properly", "[util]") {
	std::stringstream first_config_file(R"(
asdf=1
)");
	std::stringstream second_config_file(R"(
asdf=2
fdsa=2
)");
	cpp_localiser::localiser loc1(first_config_file);
	cpp_localiser::localiser loc2(second_config_file);

	loc1.swap(loc2);

	CHECK_FALSE(loc1.empty());
	CHECK_FALSE(loc2.empty());

	CHECK(loc1.can_translate_key("asdf"));
	CHECK(loc1.can_translate_key("fdsa"));
	REQUIRE(loc1.translate_key("asdf") == "2");
	REQUIRE(loc1.translate_key("fdsa") == "2");

	CHECK(loc2.can_translate_key("asdf"));
	CHECK_FALSE(loc2.can_translate_key("fdsa"));
	REQUIRE(loc2.translate_key("asdf") == "1");
}

TEST_CASE("non-member swap() swaps properly", "[util]") {
	std::stringstream first_config_file(R"(
asdf=1
)");
	std::stringstream second_config_file(R"(
asdf=2
fdsa=2
)");
	cpp_localiser::localiser loc1(first_config_file);
	cpp_localiser::localiser loc2(second_config_file);

	std::swap(loc1, loc2);

	CHECK_FALSE(loc1.empty());
	CHECK_FALSE(loc2.empty());

	CHECK(loc1.can_translate_key("asdf"));
	CHECK(loc1.can_translate_key("fdsa"));
	REQUIRE(loc1.translate_key("asdf") == "2");
	REQUIRE(loc1.translate_key("fdsa") == "2");

	CHECK(loc2.can_translate_key("asdf"));
	CHECK_FALSE(loc2.can_translate_key("fdsa"));
	REQUIRE(loc2.translate_key("asdf") == "1");
}
