#include "Timer.h"
#include "ITimer.h"
#include <ctime>
#include <ratio>
#include "TimerTask.h"

/* Constructor of the Timer class, creates a new thread and starts the timer */
Timer::Timer() {
    /* Thread creating */
    th = thread([=](){
        while (true) {
            /* if the program is not finished and timer_task_vector size is not 0 then timer starts */
            if ((!finished) && (timer_task_vector.size()!=0)){
                /* Gets the minimum time in the timer_task_vector */
                auto sleeping_time = find_next_min();

                /* Waits the nearist(minimum) task */ 			
                std::this_thread::sleep_for(sleeping_time);

                /* If deadline comes in the sleep, then breaks the loop */
                if(finished)
                    break;
            
                /* Lock the mutex */
                std::unique_lock<std::mutex> ulock(m);
                std::chrono::duration<double> duration_diff;
                
                /* If the timer is type 1 then runs the method and deletes in the vector */
                if (timer_task_vector[current_timer].get_type() == 1){
                    timer_task_vector[current_timer].get_cb()(); 
                    duration_diff = CLOCK::now() - timer_task_vector[current_timer].get_tp();
                    timer_task_vector.erase(timer_task_vector.begin()+current_timer);
                    
                }
                /* If the timer is type 2 then runs the method periodically */
                else if(timer_task_vector[current_timer].get_type() == 2){
                    timer_task_vector[current_timer].get_cb()();
                    duration_diff=(timer_task_vector[current_timer].get_passed_tp()+timer_task_vector[current_timer].get_period())-CLOCK::now();
                    timer_task_vector[current_timer].set_passed_tp(CLOCK::now());
                }
                /* If the timer is type 3 then runs the method periodically but if the stated time over, deletes the timer in the vector */
                else if(timer_task_vector[current_timer].get_type() == 3){
                    duration_diff=(timer_task_vector[current_timer].get_passed_tp()+timer_task_vector[current_timer].get_period())-CLOCK::now();
                    timer_task_vector[current_timer].set_passed_tp(CLOCK::now());
                    
                    /* The time limit achieved */
                    if (CLOCK::now()>=timer_task_vector[current_timer].get_tp()){
                        timer_task_vector.erase(timer_task_vector.begin()+current_timer);
                    }
                    /* Method running */
                    else{
                        timer_task_vector[current_timer].get_cb()();
                    }
                }
                /* If the timer is type 4 then runs the method periodically but if the predicate returns false, deletes the timer in the vector */
                else if(timer_task_vector[current_timer].get_type() == 4){
                    timer_task_vector[current_timer].get_cb()();
                    duration_diff = (timer_task_vector[current_timer].get_passed_tp() + timer_task_vector[current_timer].get_period() ) - CLOCK::now();

                    if (timer_task_vector[current_timer].get_p_func()()){
                        timer_task_vector[current_timer].set_passed_tp(CLOCK::now());
                    }
                    else{	
                        timer_task_vector.erase(timer_task_vector.begin()+current_timer);
                    }				
                }

                /* Deadline miss situation */
                if ((duration_diff.count()*1000)>10){
                    cerr<<"Timer | Deadline miss occured."<<endl;
                }
            }
            /* if finished then breaks the loop */
            else if(finished){ 
                break;
            }
            /* Finishing or beginning wait state */
            else{
                std::unique_lock<std::mutex> ulock(m);
                cv.wait(ulock,[&]{return cv_flag; });
                cv_flag=false;  
            }
        }
    });
}

/* Destructor of the timer. Changes the states, flags and condition variable. At the passed_time waits the thread's end */
Timer::~Timer(){
    cout<<"Timer::thread function terminating..."<<endl;
    {
        std::unique_lock<std::mutex> ulock(m);
        finished = true;
        cv_flag=true;
        cv.notify_one();
        ulock.unlock();
    }
    th.join();
}

/* run the callback once at time point tp. */
void Timer::registerTimer(const Timepoint &tp, const TTimerCallback &cb){
        
    std::unique_lock<std::mutex> ulock(m);
    auto passed_time = CLOCK::now();
    timer_task_vector.push_back(TimerTask(passed_time,tp,cb));
    cv_flag=true;
    cv.notify_one();
}

/* run the callback periodically forever. The first call will be executed after the first period. */
void Timer::registerTimer(const Millisecs & period, const TTimerCallback &cb){
        
    std::unique_lock<std::mutex> ulock(m);
    auto passed_time = CLOCK::now();
    timer_task_vector.push_back(TimerTask(passed_time,period,cb));
    cv_flag=true;
    cv.notify_one();   	
}

/* Run the callback periodically until time point tp. The first call will be executed after the first period. */
void Timer::registerTimer(const Timepoint &tp, const Millisecs & period, const TTimerCallback &cb){
    std::unique_lock<std::mutex> ulock(m);
    auto passed_time = CLOCK::now();
    //timer_task_vector[size] = new TimerTask(passed_time,tp,period,cb);
    //size++; 
    timer_task_vector.push_back(TimerTask(passed_time,tp,period,cb)); 	
    cv_flag=true;
    cv.notify_one();	
}

/* Run the callback periodically. After calling the callback every time, call the predicate to check if the termination criterion is satisfied. If the predicate returns false, stop calling the callback. */
void Timer::registerTimer(const TPredicate &pred, const Millisecs & period, const TTimerCallback &cb){
    std::unique_lock<std::mutex> ulock(m);
    auto passed_time = CLOCK::now();
    timer_task_vector.push_back(TimerTask(passed_time,pred,period,cb));
    cv_flag=true;
    cv.notify_one();
}

/* Finds the minimum duration time in the task vector */
std::chrono::duration<double> Timer::find_next_min(){
    auto now = CLOCK::now();
    int i,j;
    std::chrono::duration<double> duration_diff; 
    double min=DOUBLE_MAX;	
    
    for ( j = 0;(j<timer_task_vector.size()); j++)
    {	
        if (timer_task_vector[j].get_type() == 1){
            duration_diff = (timer_task_vector[j].get_tp()) - now; 
        }
        else{
            duration_diff = (timer_task_vector[j].get_passed_tp() + timer_task_vector[j].get_period()) - now;	
        }
        if (min>duration_diff.count()){	
            current_timer = j;
            min=duration_diff.count();
        }	
    }

    return std::chrono::duration<double>(min);
}
