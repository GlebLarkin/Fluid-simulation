#ifndef BARRIER_CLASS_HPP_
#define BARRIER_CLASS_HPP_

#include <class.h>
#include <mutex>
#include <condition_variable>

struct NullCallback {
	void operator() (){}
};

static std::atomic_bool Exit = false;

void callback(){}

template<class TCallback>
class Barrier {
public:
    Barrier(uint16_t size, TCallback callback_)
            : total(size), waiting(0), epoch(0), callback(callback_), mtx(), cv() {}

    Barrier(const Barrier<TCallback> &) = delete;

    Barrier(Barrier<TCallback> &&) = delete;

    void ArriveAndWait() {
        ::std::unique_lock lock(mtx);
        int32_t ep = epoch;

        if (++waiting == total) {
            waiting = 0;
            callback();
            ++epoch;
            cv.notify_all();
        } else {
            cv.wait(lock, [this, &ep] { return ep != epoch; });
        }
    }

    ~Barrier() {
        //FT_ASSERT(waiting == 0, "Barrier destroyed with sleeping threads inside!");
    }

private:
    int32_t total, waiting, epoch;
    TCallback callback;
    ::std::mutex mtx;
    ::std::condition_variable cv;
};

#endif  // BARRIER_CLASS_HPP_