#ifndef LAB3_TIMINGPOINT_H
#define LAB3_TIMINGPOINT_H

struct TimingPoint {
	int worksites_count;
	double single_thread_ms;
	double multi_thread_ms;
	double speedup;
};

#endif