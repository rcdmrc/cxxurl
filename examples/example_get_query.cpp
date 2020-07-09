/**
 * @author : xiaozhuai
 * @date   : 17/1/4
 */

#include <iostream>
#include <sstream>
#include <cxxurl/cxxurl_all.h>

using namespace std;
using namespace CXXUrl;


int main(int argc, char **argv) {
    ostringstream contentOutput;
    ExecResult res;

    /*
     * With individual parameters
     */
    Request request1 = RequestBuilder()
            .url("http://localhost:3000/get")
            .queryParameter("parameter_name", "parameter_value")
            .followLocation(true)
            .contentOutput(&contentOutput)
            .build();

    res = request1.get();

    cout << "------------ Code ------------" << endl
         << res.getCode() << endl
         << "----------- Content ----------" << endl
         << contentOutput.str() << endl
         << flush;

    /*
     * With all parameters
     */
    Request request2 = RequestBuilder()
            .url("http://localhost:3000/get")
            .queryParameter({
                                    {"parameter1_name", "parameter1_value"},
                                    {"paramete2r_name", "parameter2_value"}
                            })
            .followLocation(true)
            .contentOutput(&contentOutput)
            .build();

    res = request2.get();

    cout << "------------ Code ------------" << endl
         << res.getCode() << endl
         << "----------- Content ----------" << endl
         << contentOutput.str() << endl
         << flush;
}