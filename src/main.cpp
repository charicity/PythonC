#include <google/protobuf/text_format.h>

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
    std::cout << "str.size()=" << str.size() << " | str=" << str1 << std::endl;

    return 0;
}