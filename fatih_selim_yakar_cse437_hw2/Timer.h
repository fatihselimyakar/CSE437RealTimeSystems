#ifndef TIMER
#define TIMER

#include "TimerTask.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include "ITimer.h"
#include <vector> 
#include <mutex>
#include <condition_variable>
#include <atomic>

/* Maximum double value. */
#define DOUBLE_MAX 1.79769e+308

using namespace std;

/* Timer extends the ITimer interface */
class Timer : public ITimer{
private:

    /* vector that holds the timer task informations */
    vector<TimerTask> timer_task_vector;

    /* Synchronize and thread comm. variables */
    thread th;
    mutex m;
    condition_variable cv;

    /* Holds the current processing timer */
    atomic<int> current_timer {0};

    /* Holds the timer state */
    atomic<bool> finished{false};

    /* Avoids the spurious wakeup */
    bool cv_flag=false;

    /* Finds the minimum duration from now */
    std::chrono::duration<double> find_next_min();

public:
    /* Register timers, destructor, constructor. */
    Timer();
    ~Timer();
    void registerTimer(const Timepoint &tp, const TTimerCallback &cb);
    void registerTimer(const Millisecs & period, const TTimerCallback &cb);
    void registerTimer(const Timepoint &tp, const Millisecs & period, const TTimerCallback &cb);
    void registerTimer(const TPredicate &pred, const Millisecs & period, const TTimerCallback &cb);     

};



#endif