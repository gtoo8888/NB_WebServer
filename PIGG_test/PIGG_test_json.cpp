#include "PIGG_test.h"
#include <fstream>
#include <iostream>
#include <json/json.h>    


// （1）JsonCpp主要包含三种类型：Value、Reader和Writer;
// Json::Value是 JsonCpp 中最基本、最重要的类，用于表示各种类型的对象，JsonCpp 支持的对象类型可见 Json::ValueType 枚举值。
// Json::Reader是用于读取的，说的确切点，是用于将字符串转换为 Json::Value 对象的。
// Json::Writer类是一个纯虚类，并不能直接使用。在此我们使用 Json::Writer 的子类：Json::FastWriter、Json::StyledWrite、Json::StyledStreamWriter。
// （2）Jsoncpp 中所有对象、类名都在 namespace json 中，包含 json.h 即可


namespace PIGG_init_test{

void PIGG_test::test_json1_read(){
    std::ifstream ifs;
    ifs.open("./PIGG_test/test.json");
    if(!ifs.is_open()){
        std::cout << "OK" << std::endl;
    }
    Json::Reader reader;
    Json::Value root;
    if(!reader.parse(ifs,root,false)){
        printf("read error\n");
        return ;
    }

    std::string name = root["encoding"].asString();
    int age = root["age"].asInt();

    std::cout << name << std::endl;
    std::cout << age  << std::endl;
}


void PIGG_test::test_json2_write(){
    Json::Value root;
    Json::FastWriter writer;
    Json::Value person;

    person["name"] = "hello world";
    person["age"] = 100;
    root.append(person);

    std::string json_file = writer.write(root);

    std::ofstream ofs;
    ofs.open("temp.json");
    if(!ofs.is_open()){

    }
    ofs << json_file;
    ofs.close();
}


void PIGG_test::test_json3(){
    std::fstream ifs;
    ifs.open("./PIGG_test/test3.json");
    if(!ifs.is_open()){
        std::cout << "OK" << std::endl;
    }  
    Json::Reader read;
    Json::Value root;
    if(!read.parse(ifs,root,false)) {
        printf("read error\n");
        return ;
    }
    bool json1 = root["test_bool"].asBool();
    int json2 = root["test_int"].asInt();
    long long json3 = root["test_int64"].asInt64();
    unsigned int json4 = root["test_uint"].asUInt();
    unsigned long long json5 = root["test_uint64"].asUInt64();
    float json6 = root["test_float"].asFloat();
    double json7 = root["test_double"].asDouble();
    std::string json8 = root["test_string"].asString();
    const char* json9 = root["test_cstring"].asCString();
    std::cout << json1 << " " << json2 << " " << json3 << " " << json4 << " " << std::endl;
    std::cout << json5 << " " << json6 << " " << json7 << " " << json8 << " " << json9 << " " << std::endl;
}


}