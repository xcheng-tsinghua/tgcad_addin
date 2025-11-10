#include "SketchRNN.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <stdio.h>
#include <curl/curl.h>
#include <json/json.hpp>
#include "SKUtils.h"

using json = nlohmann::json;
using std::string;
using std::cout;
using std::endl;
using std::cerr;

SketchRNN::SketchRNN()
{

}

SketchRNN::~SketchRNN()
{

}

vector<vector<double>> SketchRNN::Infer(vector<vector<double>> input_tensor)
{
    //SKUtils::CreateConsole();
    vector<vector<double>> infered_result;

    CURL* curl = curl_easy_init();
    if (!curl) {
        cerr << "curl init failed\n";
        return infered_result;
    }

    //ector<vector<float>> data = {
    //{1.0, 2.0, 0.0},
    //{4.0, 5.0, 0.0},
    //{7.0, 8.0, 1.0}
    //};

    json input_json;
    input_json["input"] = input_tensor;

    //string url = "http://localhost:5000/infer";
    string url = "http://127.0.0.1:8888/infer";
    string input_data_string = input_json.dump();
    string response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, input_data_string.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(NULL, "Content-Type: application/json"));
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
        +[](char* ptr, size_t size, size_t nmemb, void* userdata) {
            string* str = static_cast<string*>(userdata);
            str->append(ptr, size * nmemb);
            return size * nmemb;
        });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << '\n';
    }
    else {

        json output_json = json::parse(response);
        auto arr = output_json["output"];

        for (auto& row : arr) {
            vector<double> tmp;
            for (auto& val : row) {
                tmp.push_back(val.get<double>());
            }
            infered_result.push_back(tmp);
        }

        // ¥Ú”°—È÷§
        for (auto& r : infered_result) {
            for (auto& v : r) cout << v << " ";
            cout << "\n";
        }
    }

    curl_easy_cleanup(curl);
    return infered_result;

}
