#include <queue>
#include <unordered_map>
#include <cmath>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>
#include <iostream>

std::mutex thread_lock;

template <typename T>
class safe_que {
    std::queue<T> que;
    std::mutex que_lock;
public:
    void push(T val) {
        que_lock.lock();
        que.push(val);
        que_lock.unlock();
    }
    bool empty() {
        return que.empty();
    }
    T pop() {
        que_lock.lock();
        T val = que.front();
        que.pop();
        que_lock.unlock();
        return val;
    }
};

template <typename T>
class Task {
public:
    Task() { };
    virtual void say_name() = 0;
    virtual T do_task() = 0;
};

template <typename T>
class SinTask : public Task<T> {
    T arg1;
public:
    std::string task_name = "SinTask";
    SinTask(T arg1) {
        this->arg1 = arg1;
    }
    void say_name() {
        std::cout << task_name;
    }
    T do_task() {
        return std::sin(this->arg1);
    }
};
template <typename T>
class SqrtTask : public Task<T> {
    T arg1;
public:
    std::string task_name = "SqrtTask";
    SqrtTask(T arg1) {
        this->arg1 = arg1;
    }
    void say_name() {
        std::cout << task_name;
    }
    T do_task() {
        return std::sqrt(this->arg1);
    }
};
template <typename T>
class PowTask : public Task<T> {
    T arg1, arg2;
public:
    std::string task_name = "PowTask";
    PowTask(T arg1, T arg2) {
        this->arg1 = arg1;
        this->arg2 = arg2;
    }
    void say_name() {
        std::cout << task_name;
    }
    T do_task() {
        return std::pow(this->arg1, this->arg2);
    }
};

template <typename T>
class Server {
private:
    struct task_with_id {
        Task<T>* task;
        size_t id;
    };
    safe_que<task_with_id> task_que;
    safe_que<size_t> free_ids;
    size_t max_id = 0;
    std::unordered_map<size_t, T> task_result;
    std::thread event_thread;
    bool running = false;
    void event_loop() {
        while (running) {
            if (task_que.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            task_with_id task_struct = task_que.pop();
            T return_value = task_struct.task->do_task();
            task_result.insert(std::make_pair(task_struct.id, return_value));
        }
    }
    size_t get_free_id() {
        thread_lock.lock();
        if (free_ids.empty()) {
            free_ids.push(max_id++);
        }
        size_t free_id = free_ids.pop();
        thread_lock.unlock();
        return free_id;
    }
public:
    ~Server() {
        this->stop();
        event_thread.join();
    }
    void start() {
        running = true;
        event_thread = std::thread(&Server::event_loop, this);
    }
    void stop() {
        running = false;
    }
    size_t add_task(Task<T>* task) {
        size_t free_id = get_free_id();
        task_with_id task_to_add = { task, free_id };
        task_que.push(std::move(task_to_add));
        return task_to_add.id;
    }
    T request_result(size_t id) {
        while (task_result.find(id) == task_result.end()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        T result = task_result.at(id);
        thread_lock.lock();
        task_result.erase(id);
        free_ids.push(id);
        thread_lock.unlock();
        return result;
    }
};

template <typename T>
void give_task_to_server(Server<T>* server, Task<T>* task, int num_of_tasks) {
    std::vector<size_t> task_ids(num_of_tasks);
    for (int i = 0; i < num_of_tasks; i++) {
        task_ids[i] = server->add_task(task);
    }
    for (auto task_id : task_ids) {
        T result = server->request_result(task_id);
        thread_lock.lock();
        //std::cout << result << " " << task_id << "\n";
        thread_lock.unlock();
    }
}

int main() {
    Server<float> server;
    server.start();
    std::vector<Task<float>*> tasks = { new PowTask<float>(5.0f, 2.0f), 
                                        new SinTask<float>(3.14 / 6),
                                        new SqrtTask<float>(25)
                                      };
    std::vector<std::thread> threads_l;
    for (Task<float>* task : tasks) {
        std::thread th(give_task_to_server<float>, &server, task, 10000);
        threads_l.push_back(std::move(th));
    }
    for (auto& thread : threads_l) {
        thread.join();
    }
    server.stop();
    return 0;
}