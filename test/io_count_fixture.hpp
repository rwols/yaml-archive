#pragma once

#include "io_fixture.hpp"

class io_count_fixture : public io_fixture
{
  protected:
    int save_count = 0;
    int load_count = 0;
};
