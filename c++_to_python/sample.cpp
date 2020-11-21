#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <list>

#define MAX_X 100 //受け渡したい行列のサイズ。

namespace np = boost::python::numpy;

int main() {

    //Python、numpyモジュールの初期化
    Py_Initialize();
    np::initialize();

    //名前空間の確保
    auto main_ns = boost::python::import("__main__").attr("__dict__");

    //Pythonスクリプトの読み込み
    std::ifstream ifs("mat_numpy.py");
    std::string script((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());

    //100x100行列の準備
    boost::python::tuple shapeA = boost::python::make_tuple(MAX_X, MAX_X);
    np::ndarray A = np::zeros(shapeA, np::dtype::get_builtin<double>());
    for(int i=0; i != MAX_X; i++) {
        for(int j=0; j != MAX_X; j++) {
            A[i][j] = i+j;
        }
    }

    //mat_numpy.mulの実行
    boost::python::exec(script.c_str(), main_ns);
    auto func = main_ns["mul"];
    auto pyresult_numpy = func(A);

    //結果の受け取り
    //stl_input_iteratorを使ってタプル全要素を受け取る
    boost::python::stl_input_iterator<np::ndarray> begin(pyresult_numpy), end;
    std::list<np::ndarray> pyresult_list(begin, end);

    for(auto itr = pyresult_list.begin(); itr != pyresult_list.end(); ++itr) {
        double *p = reinterpret_cast<double *>((*itr).get_data());
        //ndarrayでは基本的にメモリは連続領域上に保持されるので、
        //各要素には[]演算子を使ってアクセスできる
        std::cout << p[0] << ',' << p[1] << ',' << p[2] << ',' << p[MAX_X * MAX_X - 1] << std::endl;
    }
    return 0;
}