#include <iostream>
#include "BitArray.h"

using namespace std;

void passByValue(BitArray val) {
    val.set(0, false);
}

void ConstRef(const BitArray& ref) {
    cout << "Const read [0]: " << ref[0] << endl;
}

int main() {
    BitArray b1(5);
    b1.set(0);
    b1.set(1);

    cout << "b1 init: " << b1.to_string() << endl;

    // Конструктор копирования
    BitArray b2 = b1;
    b2.set(2, true);

    cout << "b2:  " << b2.to_string() << endl;
    cout << "b1:" << b1.to_string() << endl;

    // Копирование в функцию
    passByValue(b1);
    cout << "b1 func copy: " << b1.to_string() << endl;

    // Оператор присваивания
    BitArray b3(2);
    b3 = b1;

    cout << "b3: " << b3.to_string() << endl;


    b1[4] = true;
    cout << "b1 ref:  " << b1.to_string() << endl;

    ConstRef(b1);

    return 0;
}