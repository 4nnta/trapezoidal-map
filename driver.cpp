#include <bits/stdc++.h>
#include "trapezoidal-map.h"
using namespace std;

void traverse(Node *node) {
	cout << node->name();

	cout << "left(";
	if (node->left != NULL)
		traverse(node->left);
	cout << ")";


	cout << "right(";
	if (node->right != NULL)
		traverse(node->right);
	cout << ")";
}

int toInt(string s) {
	int exp = 1, ans = 0;
	for (int i = s.size()-1; i >= 0; --i) {
		ans += (s[i] - '0') *  exp;
		exp *= 10;
	}

	return ans;
}

int main (int argc, char **argv) {
	string num = argv[1];
	int n = toInt(num);
	srand(time(NULL));
	vector <Segment*> segments;

	// freopen("test_4.in", "w+", stdout);
	
	cout << n << "\n";

	// int x1, y1, x2, y2;
	// cin >> x1 >> y1 >> x2 >> y2;
	// Segment *seg1 = new Segment(Point(x1, y1), Point(x2, y2));
	// cin >> x1 >> y1 >> x2 >> y2;
	// Segment *seg2 = new Segment(Point(x1, y1), Point(x2, y2));

	// cout << seg1->isIntersecting(seg2) << "\n";

	for (int i = 0; i < n; ++i) {
		int x1 = rand() % 4000 - 2000, y1 = rand() % 4000 - 2000;
		int x2 = rand() % 4000 - 2000, y2 = rand() % 4000 - 2000;
		Point p1 = Point(x1, y1);
		Point p2 = Point(x2, y2);
		Segment *seg = new Segment(p1, p2);

		while (true) {
			bool noIntersect = true;
			for (int j = 0; j < segments.size(); ++j)
				if (segments[j]->isIntersecting(seg))
					noIntersect = false;

			if (noIntersect)
				break ;

			int replace = rand() % 4;
			if (replace == 0)
				seg->pLeft.x = rand() % 4000 - 2000;
			else if (replace == 1)
				seg->pLeft.y = rand() % 4000 - 2000;
			else if (replace == 2)
				seg->pRight.x = rand() % 4000 - 2000;
			else
				seg->pRight.y = rand() % 4000 - 2000;
		}

		segments.push_back(seg);

		cout << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
	}

	// for (int i = 0; i < segments.size(); ++i)
	// 	for (int j = i+1; j < segments.size(); ++j) {
	// 		if (segments[i]->isIntersecting(segments[j])) {
	// 			cout << i << " intersects " << j << "\n"; 
	// 		}
	// 	}

	TrapezoidalMap tMap = TrapezoidalMap();
	tMap.buildMap(segments);

	while (true) {
		int x, y;
		cin >> x >> y;
		Trapezoid *trape = tMap.findFace(Point(x, y));
		cout << "Point is below segment: [(" << trape->top->pLeft.x << ", " << trape->top->pLeft.y << "), (" << trape->top->pRight.x << ", " << trape->top->pRight.y << ")]";
		cout << " and above segment: [(" << trape->bot->pLeft.x << ", " << trape->bot->pLeft.y << "), (" << trape->bot->pRight.x << ", " << trape->bot->pRight.y << ")]";
		cout << "\n";
	}
}

/*
734 -1937 -1470 239
-1773 1661 1851 937
179 -243 1131 -1966
*/