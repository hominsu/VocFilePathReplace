# VocFilePathReplace

这个工具可以替换voc数据集中xml文件中的图片地址

- 通过定位\<filename\>和\</filename\>标签来获取文件名
- 然后将\<path\>和\</path\>标签之间的文件路径替换为定义的文件路径

后面会尝试加入多线程来加快替换速度

***

使用方法：

> main.exe [DataSet Path] [Replace Path]

> main.exe [数据集所在的文件路径] [替换xml中的图片路径]

例子：

> .\main.exe F:\DataSet\ D:\xyolo\images\train\
