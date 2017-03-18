target_create_boost_archive_test_suite(
  LIBRARY_TARGET yaml-archive
  INPUT_ARCHIVE_HEADER boost/archive/yaml_iarchive.hpp 
  OUTPUT_ARCHIVE_HEADER boost/archive/yaml_oarchive.hpp
  INPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::yaml_iarchive 
  OUTPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::yaml_oarchive 
  TEST_ISTREAM std::ifstream 
  TEST_OSTREAM std::ofstream
  ARCHIVE_FILENAME_EXTENSION yml
  ARCHIVE_FLAGS "0"
  STREAM_FLAGS "(std::ios_base::openmode)0"
  )
