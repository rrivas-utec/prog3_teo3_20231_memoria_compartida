//
// Created by rrivas on 9/06/2023.
//

#ifndef PROG3_TEO3_20231_MEMORIA_COMPARTIDA_COUNT_FREQUENCE_H
#define PROG3_TEO3_20231_MEMORIA_COMPARTIDA_COUNT_FREQUENCE_H

#include <iterator>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <chrono>
using namespace std;

mutex mtx2;

template<typename Iterator, typename T = typename Iterator::value_type>
void frequence(Iterator start, Iterator stop, map<T, int>& results) {
    while (start != stop) {
        this_thread::sleep_for(chrono::microseconds(5));
        {
            unique_lock ul(mtx2);
            results[*start]++;
        }
        ++start;
    }
}

template<typename Iterator>
auto count_frequence(Iterator start, Iterator stop) {
    // Variables Auxiliares
    auto n = distance(start, stop);
    auto nh = n < thread::hardware_concurrency()? 1: thread::hardware_concurrency();
    auto rango = n / nh;
    // Crear los contenedores
    vector<thread> vh(nh); // 1. Crear hilos
    using T = typename Iterator::value_type;
    map<T, int> results;
    // 2. Asignar la tarea a los hilos
    for (int i = 0; i < nh; ++i) {
        // Calculo de los subrangos
        auto r_start = next(start, i*rango);
        auto r_stop = i < nh - 1? next(start, (i+1)*rango): stop;
        // Asignar la tarea
        vh[i] = thread(frequence<Iterator> , r_start, r_stop, ref(results));
    }
    // 3. Unir los hilos
    for (int i = 0; i < nh; ++i) {
        vh[i].join();
    }
    return results;
}

#endif //PROG3_TEO3_20231_MEMORIA_COMPARTIDA_COUNT_FREQUENCE_H
