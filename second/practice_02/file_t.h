#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>

class file_t
{
public:
    file_t(const std::string& path)
        : path_(path)
        , file_(fopen(path.c_str(), "r"))
    {
    }

    file_t(const file_t& file)
        : file_t(file.path_)
    {
        std::cerr << "copy" << std::endl;
    }

    file_t(file_t&& file)
    {
        std::cerr << "move" << std::endl;

        file.swap(this);
    }

    file_t& operator=(file_t file)
    {
        swap(&file);
        return *this;
    }

    ~file_t()
    {
        std::cerr << "free" << std::endl;

        if (file_)
        {
            fclose(file_);
        }
    }

    bool good()
    {
        return !fail() && !eof();
    }

    bool fail()
    {
        return ferror(file_);
    }

    bool eof()
    {
        return feof(file_);
    }

    friend file_t& operator>>(file_t& file, int& result);

private:
    std::string path_;
    FILE* file_;

    void swap(file_t* file)
    {
        std::swap(path_, file->path_);
        std::swap(file_, file->file_);
    }
};

file_t& operator>>(file_t& file, int& result)
{
    fscanf(file.file_, "%d", &result);
    return file;
}
