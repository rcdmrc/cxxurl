/**
 * @author : xiaozhuai
 * @date   : 17/1/4
 */

#include <iostream>
#include <sstream>
#include <cxxurl/cxxurl_all.h>

using namespace std;
using namespace CXXUrl;

string writeKeyValues(map<std::string, std::string> const &m) {
    stringstream ss;
    for (auto const &h: m) {
        ss << h.first << "= " << h.second << endl;
    }
    return ss.str();
}

int main(int argc, char **argv) {
    ostringstream headerOutput;
    ostringstream contentOutput;
    map<std::string, std::string> headerMapOutput;

    Request request = RequestBuilder()
            .url("http://localhost:3000/get")
            .followLocation(true)
            .headerOutput(&headerOutput)
            .headerOutput(&headerMapOutput)
            .contentOutput(&contentOutput)
            .build();
    auto const res = request.get();

    cout << "----------- Code ----------------" << endl
         << res.getCode() << endl
         << "----------- Header (Raw) --------" << endl
         << headerOutput.str() << endl
         << "----------- Header (key-value) --" << endl
         << writeKeyValues(headerMapOutput)
         << flush;
}