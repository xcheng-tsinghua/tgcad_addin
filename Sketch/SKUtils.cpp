#include "SKUtils.h"
#include <windows.h>
#include <iostream>


void SKUtils::CreateConsole()
{
    if (!AllocConsole())
    {
        // 错误处理：获取错误信息
        return;
    }
    FILE* fp = nullptr;
    // 重定向标准输出和错误
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    // 重定向标准输入
    freopen_s(&fp, "CONIN$", "r", stdin);

    // 清除所有流的错误状态
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

}


vector<double> SKUtils::linspace(double start, double end, int sec)
{
    vector<double>tmp;
    tmp.push_back(start);
    double section = (end - start) / (sec - 1);
    for (int i = 1; i <= sec - 2; i++)
    {
        tmp.push_back(start + section * i);
    }
    tmp.push_back(end);
    return tmp;

}









