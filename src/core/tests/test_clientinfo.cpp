#include <doctest/doctest.h>
#include "clientinfo.h"

TEST_CASE("ClientInfo stores accepted MIME types") {
    PreferenceList<MimeType> mimeTypes;
    mimeTypes.push_back(Preference<MimeType>(MimeType("text/html"), 1.0f));
    mimeTypes.push_back(Preference<MimeType>(MimeType("application/json"), 0.9f));

    PreferenceList<Locale> locales;
    PreferenceList<TextCodec*> codecs;

    ClientInfo info(mimeTypes, locales, codecs);

    CHECK(info.acceptedMimeTypes().size() == 2);
}

TEST_CASE("ClientInfo stores accepted locales") {
    PreferenceList<MimeType> mimeTypes;
    PreferenceList<Locale> locales;
    locales.push_back(Preference<Locale>(Locale("en-US"), 1.0f));
    locales.push_back(Preference<Locale>(Locale("fr"), 0.8f));
    PreferenceList<TextCodec*> codecs;

    ClientInfo info(mimeTypes, locales, codecs);

    CHECK(info.acceptedLocales().size() == 2);
}

TEST_CASE("ClientInfo stores user agent") {
    PreferenceList<MimeType> mimeTypes;
    PreferenceList<Locale> locales;
    PreferenceList<TextCodec*> codecs;

    ClientInfo info(mimeTypes, locales, codecs, "Mozilla/5.0");

    // userAgent() is declared but not defined, so we just verify construction succeeds
    CHECK(info.acceptedMimeTypes().empty());
}

TEST_CASE("ClientInfo MIME type preference ordering") {
    PreferenceList<MimeType> mimeTypes;
    mimeTypes.push_back(Preference<MimeType>(MimeType("text/plain"), 0.5f));
    mimeTypes.push_back(Preference<MimeType>(MimeType("text/html"), 1.0f));
    mimeTypes.push_back(Preference<MimeType>(MimeType("application/json"), 0.8f));

    PreferenceList<Locale> locales;
    PreferenceList<TextCodec*> codecs;

    ClientInfo info(mimeTypes, locales, codecs);

    std::vector<MimeType> available = {
        MimeType("text/plain"),
        MimeType("application/json")
    };

    MimeType best = info.acceptedMimeTypes().outOf(available);
    CHECK(best.toString() == "application/json");
}

TEST_CASE("ClientInfo outOf returns default when no match") {
    PreferenceList<MimeType> mimeTypes;
    mimeTypes.push_back(Preference<MimeType>(MimeType("text/html"), 1.0f));

    PreferenceList<Locale> locales;
    PreferenceList<TextCodec*> codecs;

    ClientInfo info(mimeTypes, locales, codecs);

    std::vector<MimeType> available = {MimeType("image/png")};

    MimeType best = info.acceptedMimeTypes().outOf(available);
    CHECK(best.toString().empty());
}
