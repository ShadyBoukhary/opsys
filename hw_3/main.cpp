// Shady Boukhary
// Dr. Passos
// Operating Systems
// Assignment 3
// P-threads array sum

#include <pthread/pthread.h>
#include <stdint.h>
#include <stdio.h>

const uint32_t NUM_OF_THREADS = 4;
const uint32_t DATA_SIZE = 6000;
uint32_t data[DATA_SIZE];
uint64_t sum = 0;

pthread_mutex_t mtx;

// Finds 1 / 4th of the sum of the array per thread
void * sumArray(void *tid) {
    uint16_t id = reinterpret_cast<intptr_t>(tid);
    uint32_t start = id * DATA_SIZE / 4;
    uint32_t end = start + 1500;
    printf("Thread %d: Start at %d, end at %d\n", id, start, end - 1);
    for (int i = start; i < end; i++) {
        pthread_mutex_lock(&mtx);
        sum += data[i];
        pthread_mutex_unlock(&mtx);
    }
    pthread_exit(nullptr);
}

void populateData() {
    for (int i = 0; i < DATA_SIZE; i++) {
        data[i] = i + 1;
    }
}

int main() {
    pthread_t threads[NUM_OF_THREADS];
    uint32_t status;
    pthread_mutex_init(&mtx, nullptr);
    populateData();
    for (uint16_t i = 0; i < NUM_OF_THREADS; i++) {
        status = pthread_create(threads, nullptr, sumArray, reinterpret_cast<void*>(i));
    }
    for (const auto &thread : threads) {
        pthread_join(thread, nullptr);
    }
    printf("The sum is %llu\n", sum);
    pthread_mutex_destroy(&mtx);
    return 0;
}
