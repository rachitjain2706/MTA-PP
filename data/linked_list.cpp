// g++ -std=c++1y -fopenmp -o linked_list linked_list.cpp

#include <iostream>
#include <omp.h>
#include <zconf.h>


struct Node {
    int data;
    struct Node *next = NULL;

    Node() {}

    Node(int data) {
        this->data = data;
    }

    Node(int data, Node *node) {
        this->data = data;
        this->next = node;
    }
};

void processNode(Node *pNode);

struct Queue {
    Node *head = NULL, *tail = NULL;

    Queue &add(int data) {
        add(new Node(data));
        return *this;
    }

    void add(Node *node) {
    #pragma omp critical
        {
            if (head == NULL) {
                head = node;
                tail = node;
            } else {
                tail->next = node;
                tail = node;
            }
        }
    }

    Node *remove() {
        Node *node;
    #pragma omp critical
        {
            node = head;
            if (head != NULL)
                head = head->next;
        }
        return node;
    }

};

int main(int argc, char *argv[]) {
    srand(12);
    Queue queue;

    int n_threads = atoi(argv[1]);
    int n_dims = atoi(argv[2]);

    #pragma omp parallel for
    for (int i = 0; i < n_dims; ++i) {
        int val = rand() / n_dims;
        queue.add(val);
    }

    size_t totalProcessedCounter = 0;
    double timer_started = omp_get_wtime();
    omp_set_num_threads(n_threads);
    #pragma omp parallel
    {
        Node *n;
        size_t processedCounter = 0;
        double started = omp_get_wtime();

        while ((n = queue.remove()) != NULL) {
            processNode(n);
            processedCounter++;
        }

        double elapsed = omp_get_wtime() - started;
        printf("Thread id: %d,  processed: %d nodes, took: %f seconds \n",
               omp_get_thread_num(), processedCounter, elapsed);
        #pragma omp atomic
        totalProcessedCounter += processedCounter;
    }

    double elapsed = omp_get_wtime() - timer_started;

    return 0;
}

void processNode(Node *node) {
    int r = rand() % 9 + 1;
    usleep(r * 10000);
}