#include <iostream>

using namespace std;

void test(bool& co) {
    co = true;
}

int main() {
    bool um = false;
    cout << um << endl;


    test(um);

    cout << um << endl;
}