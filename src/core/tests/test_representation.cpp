#include <doctest/doctest.h>
#include "representation.h"

TEST_CASE("Representation stores and retrieves text") {
    Representation rep;
    rep.setText("hello");

    CHECK(rep.hasFormat("text/plain"));

    auto raw = rep.data(MimeType("text/plain"));
    std::string result(reinterpret_cast<const char*>(raw.data()), raw.size());
    CHECK(result == "hello");
}

TEST_CASE("Representation stores and retrieves HTML") {
    Representation rep;
    rep.setHtml("<p>hi</p>");

    CHECK(rep.hasFormat("text/html"));

    auto raw = rep.data(MimeType("text/html"));
    std::string result(reinterpret_cast<const char*>(raw.data()), raw.size());
    CHECK(result == "<p>hi</p>");
}

TEST_CASE("Representation stores and retrieves XHTML") {
    Representation rep;
    rep.setXhtml("<p>hi</p>");

    CHECK(rep.hasXhtml());
    CHECK(rep.xhtml() == "<p>hi</p>");
}

TEST_CASE("Representation constructed from string") {
    Representation rep("some data", MimeType("application/octet-stream"));

    CHECK(rep.hasFormat("application/octet-stream"));

    auto raw = rep.data(MimeType("application/octet-stream"));
    std::string result(reinterpret_cast<const char*>(raw.data()), raw.size());
    CHECK(result == "some data");
}

TEST_CASE("Representation reports available formats") {
    Representation rep;
    rep.setText("a");
    rep.setHtml("<b>b</b>");

    auto formats = rep.formats();
    CHECK(formats.size() == 2);
}
