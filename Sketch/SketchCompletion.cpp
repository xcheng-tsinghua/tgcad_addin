#include "SketchCompletion.h"
#include <vector>
#include "SKPnt_2d.h"
#include "SKUtils.h"
#include <opencv2/flann/flann.hpp>
#include <filesystem>
#include <fstream>

using std::string;
using namespace SKUtils;
namespace fs = std::filesystem;
using cv::Mat;
using cv::Point;
using std::cout;
using std::endl;
using std::ifstream;
using std::cerr;

SketchCompletion::SketchCompletion(const string& folder_root)
{
    // 加载数据集（点）
    m_dataset_point = loadDataset(folder_root);

    // 构建特征库
    for (auto& sk : m_dataset_point)
    {
        Mat img = renderSketch(sk);
        Mat feat = extractFeature(img);
        m_dataset_feature.push_back(feat);   // N x 1024
    }

    //SKUtils::CreateConsole();

}


SketchCompletion::~SketchCompletion()
{

}


vector<vector<SKPnt_2d>> SketchCompletion::Infer(vector<vector<SKPnt_2d>> partial_sketch)
{
    // 用户草图 → 像素图 → 特征
    Mat userImg = renderSketch(partial_sketch);
    Mat userFeat = extractFeature(userImg);

    // 最近邻搜索
    int bestIdx = findNearest(userFeat, m_dataset_feature);

    // 找到对应的草图
    vector<vector<SKPnt_2d>> searched_sketch = m_dataset_point[bestIdx];

    // 进行平移，缩放以对齐草图
    vector<vector<SKPnt_2d>> transed_sketch = transformSketch(searched_sketch, partial_sketch);

    return transed_sketch;

}


vector<vector<SKPnt_2d>> SketchCompletion::transformSketch(const vector<vector<SKPnt_2d>>& full, const vector<vector<SKPnt_2d>>& user)  //平移 + 缩放使完整草图对齐用户草图
{
    cv::Rect2f bboxUser = getBoundingBox(user);
    cv::Rect2f bboxDB = getBoundingBox(full);

    float scaleX = bboxUser.width / bboxDB.width;
    float scaleY = bboxUser.height / bboxDB.height;
    float scale = 0.5f * (scaleX + scaleY);

    float dx = bboxUser.x - bboxDB.x * scale;
    float dy = bboxUser.y - bboxDB.y * scale;

    vector<vector<SKPnt_2d>> out;
    for (auto& stroke : full)
    {
        vector<SKPnt_2d> s2;
        for (auto& p : stroke)
        {
            s2.emplace_back(p.X() * scale + dx, p.Y() * scale + dy);
        }
        out.push_back(s2);
    }
    return out;
}


vector<vector<vector<SKPnt_2d>>> SketchCompletion::loadDataset(const string& folder)  // 递归扫描目录，读取所有 txt，并存入数据库
{
    vector<vector<vector<SKPnt_2d>>> dbStrokes;

    for (auto& entry : fs::recursive_directory_iterator(folder))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
        {
            string path = entry.path().string();
            cout << "Loading: " << path << endl;

            auto sketch = loadSketchFromTxt(path);

            if (!sketch.empty())
                dbStrokes.push_back(sketch);
        }
    }

    cout << "Total sketches loaded = " << dbStrokes.size() << endl;
    return dbStrokes;
}


vector<vector<SKPnt_2d>> SketchCompletion::loadSketchFromTxt(const string& path)  // 从单个 txt 读取草图, txt 格式: x y s, s=1 表示笔划结束
{
    vector<vector<SKPnt_2d>> strokes;
    vector<SKPnt_2d> currentStroke;

    ifstream fin(path);
    if (!fin.is_open()) {
        cerr << "Failed to open: " << path << endl;
        return strokes;
    }

    float x, y;
    int s;

    while (fin >> x >> y >> s)
    {
        currentStroke.emplace_back(x, y);

        if (s == 1) {  // 笔划结束
            strokes.push_back(currentStroke);
            currentStroke.clear();
        }
    }

    // 处理最后一条未结束的笔划（防止文件未以 s=1 结尾）
    if (!currentStroke.empty()) {
        strokes.push_back(currentStroke);
    }

    return strokes;
}


Mat SketchCompletion::renderSketch(const vector<vector<SKPnt_2d>>& strokes, int imgSize, int thickness)
/************* 1. 绘制笔划到图像（bitmap） *****************/
{
    Mat canvas(imgSize, imgSize, CV_8UC1, cv::Scalar(0));

    for (const auto& stroke : strokes)
    {
        for (size_t i = 1; i < stroke.size(); i++)
        {
            Point p1(stroke[i - 1].X(), stroke[i - 1].Y());
            Point p2(stroke[i].X(), stroke[i].Y());
            line(canvas, p1, p2, cv::Scalar(255), thickness, cv::LINE_AA);
        }
    }

    return canvas;

}


Mat SketchCompletion::extractFeature(const Mat& img)
/************* 2. 提取简单特征（32x32 展平） *****************/
{
    Mat resized;
    resize(img, resized, cv::Size(32, 32));

    Mat f;
    resized.reshape(1, 1).convertTo(f, CV_32F);

    return f;  // 1 x 1024
}


int SketchCompletion::findNearest(const Mat& queryFeature, const Mat& dbFeatures)
{
    /************* 3. 在数据库中最近邻搜索（FLANN） **************/
    cv::flann::Index flannIndex(dbFeatures, cv::flann::KDTreeIndexParams(4));

    vector<int> indices(1);
    vector<float> dists(1);

    flannIndex.knnSearch(queryFeature, indices, dists, 1, cv::flann::SearchParams(32));

    return indices[0];
}


cv::Rect2f SketchCompletion::getBoundingBox(const vector<vector<SKPnt_2d>>& sketch)
{
    double minx = 1e9, miny = 1e9, maxx = -1e9, maxy = -1e9;

    for (auto& stroke : sketch)
        for (auto& p : stroke)
        {
            minx = std::min(minx, p.X());
            miny = std::min(miny, p.Y());
            maxx = std::max(maxx, p.X());
            maxy = std::max(maxy, p.Y());
        }

    return cv::Rect2f(minx, miny, maxx - minx, maxy - miny);
}
