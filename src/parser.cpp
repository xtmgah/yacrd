/*
Copyright (c) 2018 Pierre Marijon <pierre.marijon@inria.fr>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <iostream>
/* standard include */
#include <fstream>

/* project include */
#include "parser.hpp"

void yacrd::parser::file(const std::string& filename, yacrd::utils::read2mapping_type* read2mapping)
{
    auto parse_line = yacrd::parser::paf_line;
    if(filename.substr(filename.find_last_of(".") + 1) == "mhap")
    {
        parse_line = yacrd::parser::mhap_line;
    }

    std::uint64_t switch_val;

    std::string line;
    std::ifstream infile(filename);
    std::vector<std::string> tokens;
    while(std::getline(infile, line))
    {
        std::string name_a, name_b;
        std::uint64_t len_a, beg_a, end_a, len_b, beg_b, end_b;

        (*parse_line)(line, &name_a, &len_a, &beg_a, &end_a, &name_b, &len_b, &beg_b, &end_b, tokens);

        if(beg_a > end_a)
        {
            switch_val = beg_a;
            beg_a = end_a;
            end_a = switch_val;
        }

        if(beg_b > end_b)
        {
            switch_val = beg_b;
            beg_b = end_b;
            end_b = switch_val;
        }

        if(read2mapping->count(std::make_pair(name_a, len_a)) == 0)
        {
            read2mapping->emplace(std::make_pair(name_a, len_a), std::vector<yacrd::utils::interval>());
        }
        read2mapping->at(std::make_pair(name_a, len_a)).push_back(std::make_pair(beg_a, end_a));

        if(read2mapping->count(std::make_pair(name_b, len_b)) == 0)
        {
            read2mapping->emplace(std::make_pair(name_b, len_b), std::vector<yacrd::utils::interval>());
        }
        read2mapping->at(std::make_pair(name_b, len_b)).push_back(std::make_pair(beg_b, end_b));
    }

}

void yacrd::parser::paf_line(const std::string& line, std::string* name_a, std::uint64_t* len_a, std::uint64_t* beg_a, std::uint64_t* end_a, std::string* name_b, std::uint64_t* len_b, std::uint64_t* beg_b, std::uint64_t* end_b, std::vector<std::string>& tokens)
{
    yacrd::utils::split(line, '\t', tokens);

    *name_a = tokens[0];
    *len_a = std::stoi(tokens[1]);
    *beg_a = std::stoi(tokens[2]);
    *end_a = std::stoi(tokens[3]);

    *name_b = tokens[5];
    *len_b = std::stoi(tokens[6]);
    *beg_b = std::stoi(tokens[7]);
    *end_b = std::stoi(tokens[8]);
}

void yacrd::parser::mhap_line(const std::string& line, std::string* name_a, std::uint64_t* len_a, std::uint64_t* beg_a, std::uint64_t* end_a, std::string* name_b, std::uint64_t* len_b, std::uint64_t* beg_b, std::uint64_t* end_b, std::vector<std::string>& tokens)
{
    yacrd::utils::split(line, ' ', tokens);

    *name_a = tokens[0];
    *len_a = std::stoi(tokens[7]);
    *beg_a = std::stoi(tokens[5]);
    *end_a = std::stoi(tokens[6]);

    *name_b = tokens[1];
    *len_b = std::stoi(tokens[11]);
    *beg_b = std::stoi(tokens[9]);
    *end_b = std::stoi(tokens[10]);
}

