//  simple_ls program  -------------------------------------------------------//

//  Copyright Jeff Garland and Beman Dawes, 2002

//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/filesystem for documentation.

#define BOOST_FILESYSTEM_VERSION 3

#ifndef BOOST_FILESYSTEM_NO_DEPRECATED
#  define BOOST_FILESYSTEM_NO_DEPRECATED
#endif
#ifndef BOOST_SYSTEM_NO_DEPRECATED
#  define BOOST_SYSTEM_NO_DEPRECATED
#endif

#include <iostream>
#include "boost/filesystem/operations.hpp"
#include "boost/progress.hpp"

using namespace std;
namespace fs = boost::filesystem;

void list_files(std::string dir, std::vector<std::string>& raster_files, const string& ext)
{
    fs::path p(fs::current_path());

    p = fs::system_complete(dir.c_str());

    unsigned long file_count = 0;
    unsigned long err_count = 0;

    if (!fs::exists(p))
    {
        std::cout << "\nNot found: " << p << std::endl;
        return;
    }

    if (fs::is_directory(p))
    {
        p = fs::canonical(p);
        std::cout << "In directory: " << p << std::endl;
        fs::directory_iterator end_iter;
        for (fs::directory_iterator dir_itr(p); dir_itr != end_iter; ++dir_itr)
        {
            try
            {
                if (fs::is_regular_file(dir_itr->status()) && dir_itr->path().extension() == ext)
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
        std::cout << "Found " << file_count << " files with extension '" << ext << "'" << std::endl << std::endl;
        if (err_count > 0) {
            std::cout << "WARNING: encountered " << err_count << " errors" << std::endl;
        }
    }
    else // must be a file
    {
        std::cout << "\nFound: " << p << "\n";
    }
}