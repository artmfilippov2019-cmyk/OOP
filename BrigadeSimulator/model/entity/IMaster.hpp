#ifndef LAB33_IMASTER_H
#define LAB33_IMASTER_H

class IMaster {
public:
	virtual ~IMaster() = default;

	[[nodiscard]] virtual double get_efficiency() const = 0;
	virtual void set_efficiency(double value) = 0;
};

#endif