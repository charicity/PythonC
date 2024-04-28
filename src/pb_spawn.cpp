#include <google/protobuf/text_format.h>

#include <fstream>
#include <iostream>
#include <string>

#include "data.pb.h"

int main() {
    ArrayInt64 a;

    for (int i = 0; i < 10; ++i) {
        std::cout << "appending " << i << std::endl;
        a.add_data(i);
    }

    std::string str;
    a.SerializeToString(&str);

    std::string str1;
    google::protobuf::TextFormat::PrintToString(a, &str1);

    std::ofstream out("output.txt");
    out << str;
    out.close();

    return 0;
}