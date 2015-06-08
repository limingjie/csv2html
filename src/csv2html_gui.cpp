#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/fl_ask.H> // fl_alert

#include <fstream>
#include <string>

#include "csvmm.hpp"

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
        std::ofstream ofs(filename + ".html", std::ios_base::out);
        if (ofs)
        {
            ofs << html_begin;
            for (size_t row = 0; row < csv.size(); ++row)
            {
                ofs << "<tr>\n";
                for (size_t col = 0; col < csv.size(row); ++col)
                {
                    std::string field = csv.to_string(row, col, true);
                    ofs << "  <td>"
                        << encode(field)
                        << "</td>\n";
                }
                ofs << "</tr>\n";
            }
            ofs << html_end;
            ofs.close();
        }
        else
        {
            fl_alert("Failed to open output file %s.html!", filename.c_str());
        }
    }
    else
    {
        fl_alert("Failed to open input file or file empty (%s)!", filename.c_str());
    }
}

void PickFile_CB(Fl_Widget*, void*)
{
    Fl_Native_File_Chooser native;
    native.title("Pick a file");
    native.type(Fl_Native_File_Chooser::BROWSE_MULTI_FILE);
    native.filter("CSV\t*.csv");

    if (native.show() == 0)
    {
        int count = native.count();
        for (int i = 0; i < count; ++i)
        {
            csv2html(native.filename(i));
        }
    }

    fl_alert("Done.");
}

int main(int argc, char *argv[])
{
    Fl::get_system_colors();
    Fl_Window *window = new Fl_Window(100, 100, 350, 45, "CSV2HTML");
    {
        Fl_Box *o = new Fl_Box(10, 10, 240, 25, "Open CSV file to convert to HTML.");
        o->box(FL_BORDER_BOX);
        o->align(FL_ALIGN_INSIDE | FL_ALIGN_WRAP| FL_ALIGN_CENTER);
        o->color((Fl_Color)215);
        o->labelfont(FL_HELVETICA_BOLD);

        Fl_Button *but = new Fl_Button(260, 10, 80, 25, "Pick File");
        but->callback(PickFile_CB);
    }
    window->show();
    return Fl::run();
}
