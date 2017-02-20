
#define TEST_YAML_ARCHIVE

#ifdef TEST_YAML_ARCHIVE
#include <boost/archive/yaml_iarchive.hpp>
#include <boost/archive/yaml_oarchive.hpp>
using test_iarchive = boost::archive::yaml_iarchive;
using test_oarchive = boost::archive::yaml_oarchive;
#elif defined(TEST_XML_ARCHIVE)
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
using test_iarchive = boost::archive::xml_iarchive;
using test_oarchive = boost::archive::xml_oarchive;
#endif

#include <fstream>
using test_istream = std::ifstream;
using test_ostream = std::ofstream;
