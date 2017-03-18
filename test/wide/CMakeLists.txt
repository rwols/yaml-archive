target_create_boost_archive_test_suite(
  LIBRARY_TARGET yaml-archive
  INPUT_ARCHIVE_HEADER boost/archive/yaml_wiarchive.hpp 
  OUTPUT_ARCHIVE_HEADER boost/archive/yaml_woarchive.hpp
  INPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::yaml_wiarchive 
  OUTPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::yaml_woarchive 
  TEST_ISTREAM std::wifstream 
  TEST_OSTREAM std::wofstream
  ARCHIVE_FILENAME_EXTENSION wide.yml
  ARCHIVE_FLAGS "0"
  STREAM_FLAGS "(std::ios_base::openmode)0"
  )
