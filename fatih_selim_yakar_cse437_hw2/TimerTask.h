#ifndef TIMERENTITY
#define TIMERENTITY

#include "ITimer.h"

using namespace std;

class TimerTask{
    private:
        Timepoint tp;
        Timepoint passed_tp;
        TPredicate p_func;	
        Millisecs period;
        TTimerCallback cb;
        int type;	

    public:
        /* Constructors */
        TimerTask(const Timepoint &passed_tp,const Timepoint &tp, const TTimerCallback &cb);
        TimerTask(const Timepoint &passed_tp,const Millisecs & period, const TTimerCallback &cb);
        TimerTask(const Timepoint &passed_tp,const Timepoint &tp, const Millisecs & period, const TTimerCallback &cb);
        TimerTask(const Timepoint &passed_tp,const TPredicate &pred, const Millisecs & period, const TTimerCallback &cb);

        /* Setters */
        void set_tp(const Timepoint &tp);
        void set_passed_tp(const Timepoint &p_tp);
        void set_p_func(const TPredicate &pred);
        void set_period(const Millisecs & period);
        void set_cb(const TTimerCallback &cb);
        void set_type(int type);

        /* Getters */
        Timepoint get_tp();
        Timepoint get_passed_tp();
        TPredicate get_p_func();
        Millisecs get_period();
        TTimerCallback get_cb();
        int get_type();
        //bool get_running();

};

#endif