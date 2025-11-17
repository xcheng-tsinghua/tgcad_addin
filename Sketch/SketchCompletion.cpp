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
        //Mat feat = extractFeature(img);
        //m_dataset_feature.push_back(feat);   // N x 1024
        //cv::imwrite("C:\\Users\\ChengXi\\Desktop\\cstnet2\\output.png", img);

        img = preprocess(img);
        m_dataset_imgs.push_back(img);

        

    }

    //SKUtils::CreateConsole();

}


SketchCompletion::~SketchCompletion()
{

}


vector<vector<SKPnt_2d>> SketchCompletion::Infer2(vector<vector<SKPnt_2d>> partial_sketch)
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


vector<vector<SKPnt_2d>> SketchCompletion::Infer(vector<vector<SKPnt_2d>> partial_sketch)
{
    // 用户草图 → 像素图 → 特征
    Mat query = renderSketch(partial_sketch);

    // 读取查询图
    query = preprocess(query);

    double best_score = -1.0;
    int best_idx = -1;

    vector<double> all_scores;

    // 遍历数据集
    int dataset_size = m_dataset_imgs.size();
    for (int i = 0; i < dataset_size; i++) {

        double ssim = fastSSIM(query, m_dataset_imgs[i]);
        all_scores.push_back(ssim);

        if (ssim > best_score) {
            best_score = ssim;
            best_idx = i;
        }
    }

    // 找到对应的草图
    vector<vector<SKPnt_2d>> searched_sketch = m_dataset_point[best_idx];

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
    //Mat canvas(imgSize, imgSize, CV_8UC1, cv::Scalar(0));

    //for (const auto& stroke : strokes)
    //{
    //    for (size_t i = 1; i < stroke.size(); i++)
    //    {
    //        Point p1(stroke[i - 1].X(), stroke[i - 1].Y());
    //        Point p2(stroke[i].X(), stroke[i].Y());
    //        line(canvas, p1, p2, cv::Scalar(255), thickness, cv::LINE_AA);
    //    }
    //}

    //return canvas;

    Mat canvas(imgSize, imgSize, CV_8UC1, cv::Scalar(0));

    // 1. 找 min/max
    double minX = 1e9, minY = 1e9, maxX = -1e9, maxY = -1e9;
    for (auto& stroke : strokes)
        for (auto& p : stroke) {
            minX = std::min(minX, p.X());
            minY = std::min(minY, p.Y());
            maxX = std::max(maxX, p.X());
            maxY = std::max(maxY, p.Y());
        }

    double scale = imgSize / std::max(maxX - minX, maxY - minY + 1e-6);

    // 2. 画到画布
    for (auto& stroke : strokes) {
        for (size_t i = 1; i < stroke.size(); i++) {
            Point p1((stroke[i - 1].X() - minX) * scale,
                (stroke[i - 1].Y() - minY) * scale);
            Point p2((stroke[i].X() - minX) * scale,
                (stroke[i].Y() - minY) * scale);

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

double SketchCompletion::fastSSIM(const Mat& img1, const Mat& img2)
{
    // 需要 float 类型
    Mat I1, I2;
    img1.convertTo(I1, CV_32F);
    img2.convertTo(I2, CV_32F);

    // 均值
    Mat mu1, mu2;
    GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);

    // 方差
    Mat mu1_2 = mu1.mul(mu1);
    Mat mu2_2 = mu2.mul(mu2);
    Mat mu1_mu2 = mu1.mul(mu2);

    Mat sigma1_2, sigma2_2, sigma12;
    GaussianBlur(I1.mul(I1), sigma1_2, cv::Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2.mul(I2), sigma2_2, cv::Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

    GaussianBlur(I1.mul(I2), sigma12, cv::Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    const double C1 = 6.5025, C2 = 58.5225;

    Mat t1 = 2 * mu1_mu2 + C1;
    Mat t2 = 2 * sigma12 + C2;
    Mat t3 = t1.mul(t2);

    Mat t4 = mu1_2 + mu2_2 + C1;
    Mat t5 = sigma1_2 + sigma2_2 + C2;
    Mat t6 = t4.mul(t5);

    Mat ssim_map;
    divide(t3, t6, ssim_map);

    cv::Scalar mssim = mean(ssim_map);
    return mssim[0];  // 单通道

}


Mat SketchCompletion::preprocess(const Mat& img) {
    Mat small;
    // 你的草图已经是 CV_8UC1，不需要 cvtColor
    resize(img, small, cv::Size(64, 64));
    return small;
}

