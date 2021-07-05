	
 #include "TimerTask.h"   
    /* CONSTRUCTORS */
	TimerTask::TimerTask(const Timepoint &passed_tp,const Timepoint &tp, const TTimerCallback &cb)
    :passed_tp(passed_tp),tp(tp),cb(cb),type(1){
        /* Intentionally empty */
	}

    TimerTask::TimerTask(const Timepoint &passed_tp,const Millisecs & period, const TTimerCallback &cb)
    :passed_tp(passed_tp),period(period),cb(cb),type(2){   
        /* Intentionally empty */
	}

    TimerTask::TimerTask(const Timepoint &passed_tp,const Timepoint &tp, const Millisecs & period, const TTimerCallback &cb):
    passed_tp(passed_tp),tp(tp),period(period),cb(cb),type(3){   
        /* Intentionally empty */
	}

    TimerTask::TimerTask(const Timepoint &passed_tp,const TPredicate &pred, const Millisecs & period, const TTimerCallback &cb)
    :passed_tp(passed_tp),p_func(pred),period(period),cb(cb),type(4){   
        /* Intentionally empty */
	}

    /* SETTERS */
    void TimerTask::set_tp(const Timepoint &tp){
        this->tp=tp;
    }

    void TimerTask::set_passed_tp(const Timepoint &p_tp){
        this->passed_tp=p_tp;
    }

    void TimerTask::set_p_func(const TPredicate &pred){
        this->p_func=pred;
    }

    void TimerTask::set_period(const Millisecs & period){
        this->period=period;
    }

    void TimerTask::set_cb(const TTimerCallback &cb){
        this->cb=cb;
    }

    void TimerTask::set_type(int type){
        this->type=type;
    }

    /* GETTERS */
    Timepoint TimerTask::get_tp(){
        return tp;
    }

    Timepoint TimerTask::get_passed_tp(){
        return passed_tp;
    }

    TPredicate TimerTask::get_p_func(){
        return p_func;
    }

    Millisecs TimerTask::get_period(){
        return period;
    }

    TTimerCallback TimerTask::get_cb(){
        return cb;
    }

    int TimerTask::get_type(){
        return type;
    }
	