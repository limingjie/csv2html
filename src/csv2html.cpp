#include <iostream>
#include <fstream>
#include <string>

#include <csvmm/csvmm.hpp>

const std::string html_begin =
    "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n<title>csvtohtm"
    "l</title>\n<style type=\"text/css\">\ntable.csvtohtml{border-collapse:coll"
    "apse;border-spacing:0;display:block;width:100%;overflow:auto;font-size:12p"
    "x;word-break:normal;word-break:keep-all;}\ntable.csvtohtml td{padding:0;pa"
    "dding:2px 4px;border:1px solid #ddd;}\ntable.csvtohtml tr{border:0;backgro"
    "und-color:#fff;border-top:1px solid #ccc;}\ntable.csvtohtml tr:nth-child(1"
    "){background-color:#007098;color:white;font-weight:bold;}\ntable.csvtohtml"
    " tr:nth-child(2n){background-color:#f8f8f8;}\n</style>\n</head>\n<body>\n<"
    "table class=\"csvtohtml\">\n<tbody>\n";

const std::string html_end = "</tbody>\n</table>\n</body>\n</html>\n";

// http://stackoverflow.com/questions/5665231/most-efficient-way-to-escape-xml-html-in-c-string
std::string encode(const std::string& data)
{
    std::string buffer;
    buffer.reserve(data.size());
    for (size_t pos = 0; pos != data.size(); ++pos)
    {
        switch(data[pos])
        {
            case '&':  buffer.append("&amp;");       break;
            case '\"': buffer.append("&quot;");      break;
            case '\'': buffer.append("&apos;");      break;
            case '<':  buffer.append("&lt;");        break;
            case '>':  buffer.append("&gt;");        break;
            case '\r':                               break;
            case '\n': buffer.append("<br>");        break;
            default:   buffer.append(&data[pos], 1); break;
        }
    }

    return buffer;
}

void csv2html(const std::string &filename)
{
    csvmm csv;

    if (csv.read(filename) && csv.size() > 0)
    {
        std::cout << html_begin;
        for (size_t row = 0; row < csv.size(); ++row)
        {
            std::cout << "<tr>\n";
            for (size_t col = 0; col < csv.size(row); ++col)
            {
                std::string field = csv.to_string(row, col, true);
                std::cout << "  <td>"
                    << encode(field)
                    << "</td>\n";
            }
            std::cout << "</tr>\n";
        }
        std::cout << html_end;
        std::cout.flush();
    }
    else
    {
        std::cerr << "Error: Could not open file " << filename << '.'
            << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: csv2html file" << std::endl;
    }
    else
    {
        csv2html(argv[1]);
    }

    return 0;
}
