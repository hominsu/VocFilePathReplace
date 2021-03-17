#include <iostream>
#include <fstream>
#include <string>


class Replace
{
private:
    std::string str;
    std::fstream f;
    char buf[1024];

private:
    std::string replace;
    std::string xmlname;
    std::string filename;
    std::string filename_label_start;
    std::string filename_label_end;
    std::string path_label_start;
    std::string path_label__end;
    int filename_pos_start;
    int filename_pos_end;
    int filenameLength;
    int path_pos_start;
    int path_pos__end;
    int pathLength;

public:
    bool work();

public:
    Replace(const std::string& replace, const std::string& xmlname);

};
