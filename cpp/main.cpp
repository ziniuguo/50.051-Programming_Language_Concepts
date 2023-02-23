#include <iostream>
#include "fstream"


using namespace std;

int main() {

    fstream  f;
    f.open("notExist",  ios::in | ios::out | ios::app);
    f << "daisuki" << endl;
    return 0;
}
