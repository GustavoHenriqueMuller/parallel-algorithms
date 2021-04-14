#include <iostream>
#include "matmul.h"
#include "arraysum.h"

using namespace std;

int main() {
    cout << "Processing..." << endl;
    matmul::times();
    arraysum::times();

    return 0;
}
