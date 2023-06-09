#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <random>
#include <map>
#include <algorithm>
#include <numeric>
#include "count_frequence.h"
using namespace std;

mutex mtx;

class guardia {
    mutex& _mtx;
public:
    guardia(mutex& mtx): _mtx(mtx) {
        _mtx.lock();
    }
    ~guardia() {
        _mtx.unlock();
    }
};

void incremento(int& x) {
    this_thread::sleep_for(chrono::milliseconds(5));
    // INICIO DE SECCION DE SINCRONIZACION
//    guardia g(mtx);
//    mtx.lock();
//    lock_guard lg(mtx);
    unique_lock ul(mtx);
    ++x;
    ul.unlock();

    // otras instruccion

    ul.lock();
    ++x;
    // FIN DE SECCION DE SINCRONIZACION
//    mtx.unlock();
}

void ejemplo_variable_compartida() {
    int nh = 100;
    int rep = 200;
    for (int i=0; i < rep; ++i) {
        int x = 0;
        // 1. Crear los hilos
        vector<thread> vh(nh);
        // 2. Asignar la tarea
        for (auto& h: vh) {
            h = thread(incremento, ref(x));
        }
        // 3. Unir los hilos
        for (auto& h: vh) {
            h.join();
        }
        cout << x << " ";
    }
}

auto generar_vector(int n, int first, int last) {
    vector<int> vec(n);
    random_device rd;
    uniform_int_distribution<int> uid(first, last);
    for (auto& item: vec)
        item = uid(rd);
    return vec;
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& vec) {
    for (const auto& item: vec)
        out << item << " ";
    return out;
}

template<typename KeyType, typename ValueType>
ostream& operator<<(ostream& out, const map<KeyType, ValueType>& mp) {
    for (const auto& [key, value] : mp)
        out << "{" << key << ": " << value << "}\n";
    return out;
}

void ejemplo_frecuencias() {
    auto vec = generar_vector(100, 1, 20);
    auto res = count_frequence(begin(vec), end(vec));
    cout << res << endl;
    auto freq = accumulate(begin(res), end(res), 0,
                           [](auto sum, auto item) {
        sum += item.second;
        return sum;
    });
    cout << size(vec) << "==" << freq << endl;
}


int main() {
//    ejemplo_variable_compartida();
    ejemplo_frecuencias();
    return 0;
}
