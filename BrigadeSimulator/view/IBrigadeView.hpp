#ifndef LAB33_IBRIGADEVIEW_H
#define LAB33_IBRIGADEVIEW_H

class IBrigadeView {
public:
	virtual ~IBrigadeView() = default;

	virtual void run() = 0;
};

#endif