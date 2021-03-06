#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Check_Button.H>
#include <FL/fl_ask.H> // fl_alert
#include <FL/filename.H> // fl_open_uri

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

bool csv2html(const std::string &filename, bool encode_text)
{
    csvmm csv;
    if (!csv.read(filename) || csv.size() == 0)
    {
        fl_alert("Failed to open input file or file empty (%s)!",
            filename.c_str());
        return false;
    }

    std::ofstream ofs(filename + ".html", std::ios_base::out);
    if (!ofs)
    {
        fl_alert("Failed to open output file %s.html!", filename.c_str());
        return false;
    }

    ofs << html_begin;
    for (size_t row = 0; row < csv.size(); ++row)
    {
        ofs << "<tr>\n";
        for (size_t col = 0; col < csv.size(row); ++col)
        {
            std::string field = csv.to_string(row, col, true);
            ofs << "  <td>"
                << (encode_text ? encode(field) : field)
                << "</td>\n";
        }
        ofs << "</tr>\n";
    }
    ofs << html_end;
    ofs.close();

    return true;
}

class window : public Fl_Window
{
private:
    Fl_Check_Button *encode_check;
    Fl_Check_Button *open_file_check;

public:
    window(int x, int y, int w, int h, const char *label = 0)
        : Fl_Window(x, y, w, h, label)
    {
        Fl_Box *o = new Fl_Box(10, 10, 300, 25,
            "Open CSV file to convert to HTML -->");
        o->box(FL_BORDER_BOX);
        o->align(FL_ALIGN_INSIDE | FL_ALIGN_WRAP| FL_ALIGN_CENTER);
        o->color((Fl_Color)215);
        o->labelfont(FL_HELVETICA_BOLD);

        Fl_Button *but = new Fl_Button(320, 10, 80, 25, "Pick Files");
        but->callback(on_pick_button_click, this);

        encode_check = new Fl_Check_Button(10, 40, 280, 25,
            "Escape special HTML characters.");
        encode_check->value(1);

        open_file_check = new Fl_Check_Button(10, 65, 280, 25,
            "Open HTML in browser.");
        open_file_check->value(1);

        Fl_Button *author = new Fl_Button(320, 65, 80, 25, "Mingjie Li");
        author->box(FL_FLAT_BOX);
        author->align(FL_ALIGN_INSIDE | FL_ALIGN_RIGHT);
        author->labelsize(12);
        author->labelcolor(fl_rgb_color(128, 128, 128));
        author->callback(on_author_click, this);
    }

    static void on_pick_button_click(Fl_Widget *sender, void *obj)
    {
        Fl_Native_File_Chooser native;
        native.title("Pick a file");
        native.type(Fl_Native_File_Chooser::BROWSE_MULTI_FILE);
        native.filter("CSV\t*.csv");

        if (native.show() == 0)
        {
            bool encode = (((window *)obj)->encode_check->value() == 1);
            bool open = (((window *)obj)->open_file_check->value() == 1);
            int count = native.count();
            for (int i = 0; i < count; ++i)
            {
                std::string filename = native.filename(i);
                if (csv2html(filename, encode) && open)
                {
                    std::string uri("file://");
                    if (filename[0] != '/')
                    {
                        uri.push_back('/'); // Windows
                    }
                    uri += filename + ".html";

                    char errmsg[512];
                    fl_open_uri(uri.c_str(), errmsg, sizeof(errmsg));
                }
            }
        }
    }

    static void on_author_click(Fl_Widget *sender, void *obj)
    {
        char errmsg[512];
        fl_open_uri("https://github.com/limingjie/csv2html",
            errmsg, sizeof(errmsg));
    }

};

int main(int argc, char *argv[])
{
    Fl::get_system_colors();
    window *w = new window(100, 100, 410, 95, "CSV2HTML");
    w->show();
    return Fl::run();
}
