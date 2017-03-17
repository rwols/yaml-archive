#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

// random collection of various unicode symbols
static const wchar_t* s_wide_string_samples[] = {
    // L"Euro Symbol: €",
    // L"Greek: Μπορώ να φάω σπασμένα γυαλιά χωρίς να πάθω τίποτα.",
    // L"Íslenska / Icelandic: Ég get etið gler án þess að meiða mig.",
    // L"Polish: Mogę jeść szkło, i mi nie szkodzi.",
    L"Romanian: Pot să mănânc sticlă și ea nu mă rănește.",
    L"Ukrainian: Я можу їсти шкло, й воно мені не пошкодить.",
    L"Armenian: Կրնամ ապակի ուտել և ինծի անհանգիստ չըներ։",
    L"Georgian: მინას ვჭამ და არა მტკივა.",
    L"Hindi: मैं काँच खा सकता हूँ, मुझे उस से कोई पीडा नहीं होती.",
    L"Hebrew(2): אני יכול לאכול זכוכית וזה לא מזיק לי.",
    L"Yiddish(2): איך קען עסן גלאָז און עס טוט מיר נישט װײ.",
    L"Arabic(2): أنا قادر على أكل الزجاج و هذا لا يؤلمني.",
    L"Japanese: 私はガラスを食べられます。それは私を傷つけません。",
    L"Thai: ฉันกินกระจกได้ แต่มันไม่ทำให้ฉันเจ็บ",
    L"👌👀👌👀👌👀👌👀👌👀 good shit go౦ԁ sHit👌 thats ✔ some good👌👌shit right👌👌there👌👌👌 "
    L"right✔there ✔✔if i do ƽaү so my self 💯 i say so 💯 thats what im talking "
    L"about right there right there (chorus: ʳᶦᵍʰᵗ ᵗʰᵉʳᵉ) mMMMMᎷМ💯 👌👌 "
    L"👌НO0ОଠOOOOOОଠଠOoooᵒᵒᵒᵒᵒᵒᵒᵒᵒ👌 👌👌 👌 💯 👌 👀 👀 👀 👌👌Good shit",
    L"◕‿◕ இ௰இ ઊઠઊ ꉨڡꉨ ꈿ۝ꈿ ஞ౩ஞ ب_ب "
    L"ꆨ❥ꆨ "
    L"ꆧεꆧ "
    L"ꆤॄꆤ"};

// first page of chapter 1 of "the problems of philosophy" by Bertrand Russell
static const char* s_narrow_string_samples[] = {
    "Is there any knowledge in the world which is so certain that no "
    "reasonable man could doubt it?",
    "This question, which at first sight might not seem difficult, is really "
    "one of the most difficult that can be asked.",
    "When we have realized the obstacles in the way of a straightforward and "
    "confident answer, we shall be well launched on the study of "
    "philosophy—for philosophy is merely the attempt to answer such ultimate "
    "questions, not carelessly and dog- matically, as we do in ordinary life "
    "and even in the sciences, but critically, after exploring all that makes "
    "such questions puzzling, and after realizing all the vagueness and "
    "confusion that underlie our ordinary ideas.",
    "In daily life, we assume as certain many things which, on a closer "
    "scrutiny, are found to be so full of apparent contradictions that only a "
    "great amount of thought enables us to know what it is that we really may "
    "believe.",
    "In the search for certainty, it is nat- ural to begin with our present "
    "experiences, and in some sense, no doubt, knowledge is to be derived from "
    "them.",
    "But any statement as to what it is that our immediate experiences make us "
    "know is very likely to be wrong.",
    "It seems to me that I am now sitting in a chair, at a table of a certain "
    "shape, on which I see sheets of paper with writing or print.",
    "By turning my head I see out of the window buildings and clouds and the "
    "sun.",
    "I believe that the sun is about ninety-three million miles from the "
    "earth; that it is a hot globe many times bigger than the earth; that, "
    "owing to the earth’s rotation, it rises every morning, and will continue "
    "to do so for an indefinite time in the future.",
    "Here is a string with some \"qoutes\" inside it."

};

io_fixture::io_fixture()
    : m_filename(boost::unit_test::framework::current_test_case().p_name)
{
    m_filename += ARCHIVE_FILENAME_EXTENSION;
}

io_fixture::outputter::outputter(const char* filename)
    : m_os(filename, STREAM_FLAGS), m_oa(m_os, ARCHIVE_FLAGS)
{
}

io_fixture::inputter::inputter(const char* filename)
    : m_is(filename, STREAM_FLAGS), m_ia(m_is, ARCHIVE_FLAGS)
{
}

io_fixture::outputter io_fixture::output() { return {m_filename.c_str()}; }

io_fixture::inputter io_fixture::input() { return {m_filename.c_str()}; }

const wchar_t* io_fixture::get_wstring_sample(const std::size_t index) const
    noexcept
{
    return s_wide_string_samples[index];
}

std::size_t io_fixture::get_wstring_sample_count() const noexcept
{
    return sizeof(s_wide_string_samples) / sizeof(s_wide_string_samples[0]);
}

const wchar_t* io_fixture::get_random_wstring_sample() const noexcept
{
    return get_wstring_sample(rand() % get_wstring_sample_count());
}

const char* io_fixture::get_string_sample(const std::size_t index) const noexcept
{
    return s_narrow_string_samples[index];
}

std::size_t io_fixture::get_string_sample_count() const noexcept
{
    return sizeof(s_narrow_string_samples) / sizeof(s_narrow_string_samples[0]);
}

const char* io_fixture::get_random_string_sample() const noexcept
{
    return get_string_sample(rand() % get_string_sample_count());
}
