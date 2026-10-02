#ifndef LAB33_IWORKER_H
#define LAB33_IWORKER_H

class IWorker {
public:
	virtual ~IWorker() = default;

	[[nodiscard]] virtual int get_productivity() const = 0;
	virtual void set_productivity_value(int value) = 0;
};

#endif