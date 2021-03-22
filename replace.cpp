#include "replace.h"


Replace::Replace(const std::string& replace, const std::string& xmlname): replace(replace), xmlname(xmlname)
{
    filename_label_start    = "<filename>";
    filename_label_end      = "</filename>";
    path_label_start        = "<path>";
    path_label__end         = "</path>";
    filename_pos_start  = 0;
    filename_pos_end    = 0;
    filenameLength      = 0;
    path_pos_start      = 0;
    path_pos__end       = 0;
    pathLength          = 0;
}

bool Replace::work()
{
    f.open(xmlname.c_str());

    if(!f.is_open())
    {
        return false;
    }

    while(!f.eof())
	{
		f.getline(buf, 1024);
		str.append(buf);
		str.append("\n");
	}

    str.pop_back();

    // 从filname标签中截取filename
    filename_pos_start = str.find(filename_label_start.c_str());
    filename_pos_end = str.find(filename_label_end.c_str());
    filenameLength = filename_pos_end - filename_pos_start - filename_label_start.length();
    filename = str.substr(filename_pos_start + filename_label_start.length(), filenameLength);

    // 将path标签中的文件路径替换为定义的路径加上文件名
    path_pos_start = str.find(path_label_start.c_str());
    path_pos__end = str.find(path_label__end.c_str());
    pathLength = path_pos__end - path_pos_start - path_label_start.length();
    str.replace(path_pos_start + path_label_start.length(), pathLength, (replace + filename).c_str());

    f.clear();
    f.seekp(0, std::ios::beg);
    f << str;
    f.close();

    return true;
}
