#include <bits/stdc++.h>
using namespace std;

class Point {
public:
	double x;
	double y;

	Point(double x = 0.0, double y = 0.0): x(x), y(y) {}
};

class Segment {
public:
	Point pLeft;
	Point pRight;

	Segment(Point p, Point q): pLeft(p), pRight(q) {
		// have to make sure pLeft is the point on the left, in case of tie, resolve by lexicographic order
		if (pLeft.x > pRight.x || (pLeft.x == pRight.x && pLeft.y > pRight.y))
			swap(pLeft, pRight);
	}

	// relative position of point target and the segment.
	// 1 for above
	// 0 for on the support line
	// -1 for below
	int isAbove(Point target) {
		double pos = turn(target);
		if (pos > 0)
			return 1;
		else if (pos < 0)
			return -1;
		else
			return 0;
	}

	bool onSegment(Point p, Point q, Point r) { 
	    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && 
	        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) 
	       return true; 
	  
	    return false; 
	}

	int orientation(Point p, Point q, Point r) {
	    int val = (q.y - p.y) * (r.x - q.x) - 
	              (q.x - p.x) * (r.y - q.y); 
	  
	    if (val == 0) return 0;  // colinear 
	  
	    return (val > 0)? 1: 2; // clock or counterclock wise 
	}

	bool isIntersecting(Segment *other) { 
	    // Find the four orientations needed for general and 
	    // special cases
	    Point p1 = pLeft;
	    Point q1 = pRight; 
		Point p2 = other->pLeft;
		Point q2 = other->pRight;

	    int o1 = orientation(p1, q1, p2); 
	    int o2 = orientation(p1, q1, q2); 
	    int o3 = orientation(p2, q2, p1); 
	    int o4 = orientation(p2, q2, q1); 
	  
	    // General case 
	    if (o1 != o2 && o3 != o4) 
	        return true; 
	  
	    // Special Cases 
	    // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
	  
	    // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
	    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
	  
	    // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
	  
	     // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
	  
	    return false; // Doesn't fall in any of the above cases 
	}
private:
	// return the turn of pLeft, pRight, target
	double turn(Point target) {
		return (pRight.x - pLeft.x) * (target.y - pLeft.y) - (target.x - pLeft.x) * (pRight.y - pLeft.y);
	}
};

class Node;

class Trapezoid {
public:
	string name;

	Trapezoid *upLeft;
	Trapezoid *upRight;
	Trapezoid *lowLeft;
	Trapezoid *lowRight;

	Segment *top;
	Segment *bot;

	Node *node;

	Point left;
	Point right;

	Trapezoid(): upLeft(NULL), upRight(NULL), lowLeft(NULL), lowRight(NULL), node(NULL) {}

	
};

class Node {
public:
	virtual ~Node() {}
	virtual Trapezoid* getTrapezoid() { return NULL; }	// return a trapezoid if the node is a square node, NULL otherwise
	virtual Node* next(Point target) { return NULL; }	// traverse the tree, inherited by classes below
	virtual string name() { return NULL; }				// name of the node

	Node *left;
	Node *right;
	vector <Node*> parent;
};

class XNode: public Node {
public:
	Point p;

	XNode(Point p): p(p) {}

	Node* next(Point target) {
		if (target.x < p.x)
			return left;
		else
			return right;
	}

	string name() { return "X"; }
};

class YNode: public Node {
public:
	Segment *seg;

	YNode(Segment *s): seg(s) {}

	Node* next(Point target) {
		int state = seg->isAbove(target);
		if (state > 0)
			return left;
		else if (state < 0)
			return right;
		else
			return NULL; // this suppose to report the segment, gonna deal with this later
	}

	string name() { return "Y"; }
};

class SquareNode: public Node {
public:
	Trapezoid *trapezoid;

	SquareNode(Trapezoid *trapezoid): trapezoid(trapezoid) {}

	Trapezoid* getTrapezoid() { return trapezoid; }

	string name() { return "Square"; }
};