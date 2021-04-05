//
// Created by mauro delazeri on 4/4/21.
//

#pragma once

#include <cstdint>
#include <thread>

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else

#include <pthread.h>

#endif

inline void set_cpu_affinity(int32_t cpu_affinity) {
    if (cpu_affinity != -1) {
#ifdef _MSC_VER
        auto thrd = GetCurrentThread();
        SetThreadAffinityMask(thrd, 1LL << (cpu_affinity % std::thread::hardware_concurrency()));
#elif __APPLE__

#else
        cpu_set_t cpus;
        CPU_ZERO(&cpus);
        CPU_SET((cpu_affinity % std::thread::hardware_concurrency()), &cpus);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpus);
#endif
    }
}