#pragma once

class vertNode {
private:
	int index;
	double dist;
public:
	vertNode() {}
	vertNode(int i, double d) :index(i), dist(d) {}
	bool operator< (vertNode& other) { return this->dist < other.dist; }
	bool operator<= (vertNode& other) { return this->dist <= other.dist; }
	bool operator>= (vertNode& other) { return this->dist >= other.dist; }
	bool operator> (vertNode& other) { return this->dist > other.dist; }
	bool operator== (vertNode& other) { return this->dist == other.dist; }
	int getIndex() { return index; }
	double getDist() { return dist; }
};
