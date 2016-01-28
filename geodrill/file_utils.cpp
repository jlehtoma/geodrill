//  simple_ls program  -------------------------------------------------------//

//  Copyright Jeff Garland and Beman Dawes, 2002

//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/filesystem for documentation.

#define BOOST_FILESYSTEM_VERSION 3



#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <iostream>

namespace fs = boost::filesystem;

void list_files(std::string dir, std::vector<std::string>& raster_files)
{
    fs::path p(fs::current_path());

    p = fs::system_complete(dir.c_str());

    unsigned long file_count = 0;
    unsigned long err_count = 0;

    if (!fs::exists(p))
    {
        std::cout << "\nNot found: " << p << std::endl;
    }

    if (fs::is_directory(p))
    {
        p = fs::canonical(p);
        std::cout << "\nIn directory: " << p << "\n\n";
        fs::directory_iterator end_iter;
        for (fs::directory_iterator dir_itr(p); dir_itr != end_iter; ++dir_itr)
        {
            try
            {
                if (fs::is_regular_file(dir_itr->status()))
                {
                    ++file_count;
                    raster_files.push_back(dir_itr->path().string());
                }
            }
            catch (const std::exception & ex)
            {
                ++err_count;
                std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
            }
        }
        std::cout << "Found " << file_count << " files with " << err_count << " errors:" << std::endl;
    }
    else // must be a file
    {
        std::cout << "\nFound: " << p << "\n";
    }
}