#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
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


int main() {
    ejemplo_variable_compartida();
    return 0;
}
