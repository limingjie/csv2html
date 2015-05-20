#include <iostream>
#include <fstream>
#include <string>
#include <boost/tokenizer.hpp>

const std::string html_begin =
    "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n"
    "<title>csvtohtml</title>\n"
    "<style type=\"text/css\">\n"
    "table.csvtohtml"
    "{border-collapse:collapse;border-spacing:0;display:block;width:100%;"
    "overflow:auto;font-size:1em;word-break:normal;word-break:keep-all;}\n"
    "table.csvtohtml td"
    "{padding:0;padding:2px 4px;border:1px solid #ddd;}\n"
    "table.csvtohtml tr"
    "{border:0;background-color:#fff;border-top:1px solid #ccc;}\n"
    "table.csvtohtml tr:nth-child(1)"
    "{background-color: #007098;color: white;font-weight: bold;}\n"
    "table.csvtohtml tr:nth-child(2n)"
    "{background-color: #f8f8f8;}\n"
    "</style>\n</head>\n<body>\n<table class=\"csvtohtml\">\n<tbody>\n";

const std::string html_end = "</tbody>\n</table>\n</body>\n</html>\n";

void process_line(std::string &line)
{
    static size_t count = 0;

    ++count;

    std::cout << "<tr>\n";

    try
    {
        std::string html;
        boost::escaped_list_separator<char> els("", ",", "\"");
        boost::tokenizer<boost::escaped_list_separator<char>> tok(line, els);
        for (auto it = tok.begin(); it != tok.end(); ++it)
        {
            html += "  <td>" + *it + "</td>\n";
        }
        std::cout << html;
    }
    catch (boost::escaped_list_error &e)
    {
        // invalid escape sequence, tokenize again by comma only.
        std::cerr << "Error:" << count << ": " << e.what()
            << "\nLine " << count << ": " << line << std::endl;
    }

    std::cout << "</tr>\n";
}

bool csv2html(std::string filename)
{
    // check file
    std::ifstream file(filename);
    if (!file.good())
    {
        std::cerr << "Error: Failed to open file " << filename << std::endl;
        return false;
    }

    // read file line by line
    std::string line;
    while (std::getline(file, line))
    {
        process_line(line);
    }

    file.close();

    return true;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: csv2html file" << std::endl;
    }
    else
    {
        std::cout << html_begin;
        csv2html(argv[1]);
        std::cout << html_end;
        std::cout.flush();
    }

    return 0;
}
