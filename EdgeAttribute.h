#ifndef __EDGE_ATTRIBUTE_H__
#define __EDGE_ATTRIBUTE_H__

#include <string>
#include <vector>

using namespace std;

class EdgeAttribute
{
public:
	EdgeAttribute();
	EdgeAttribute(vector<double> v);
	EdgeAttribute(string s);
	EdgeAttribute(const EdgeAttribute& other); // copy-constructor
	// ~EdgeAttribute();
	static EdgeAttribute difference(EdgeAttribute a1, EdgeAttribute a2);
	static EdgeAttribute add(EdgeAttribute a1, EdgeAttribute a2);
	static EdgeAttribute weightedPairAverage(EdgeAttribute start, EdgeAttribute target, double position);
	static EdgeAttribute zeroAttribute(size_t size);
	double getAttribute();
	void setEdgeAttribute(const EdgeAttribute& attrib);
	EdgeAttribute clone();
	string toString();
	bool equals(const EdgeAttribute& other) const;
	double norm();
	void scaleBy(double a);
	size_t size() const;
	void ensurePositive();

private:
	vector<double> vect;
};

#endif /* __EDGE_ATTRIBUTE_H__ */