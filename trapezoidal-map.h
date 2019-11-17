#include <bits/stdc++.h>
#include "def-class.h"
using namespace std;

class TrapezoidalMap {
public:
	Node *root; // root of DAG

	// function to build the trapezoidal map and DAG
	void buildMap(vector <Segment*> segments) {
		// figure out the bounding box first, this take linear time
		double minX = segments[0]->pLeft.x, maxX = segments[0]->pRight.x;
		double minY = segments[0]->pLeft.y, maxY = segments[0]->pLeft.y;

		for (int i = 0; i < segments.size(); ++i) {
			Segment *seg = segments[i];
			minX = min(minX, min(seg->pLeft.x, seg->pRight.x));
			maxX = max(maxX, max(seg->pLeft.x, seg->pRight.x));
			minY = min(minY, min(seg->pLeft.y, seg->pRight.y));
			maxY = max(maxY, max(seg->pLeft.y, seg->pRight.y));
		}

		minX -= 100.0; maxX += 100.0;
		minY -= 100.0; maxY += 100.0;

		// bounding box is a face node with top and bottom boundary
		Trapezoid *trapezoid = new Trapezoid();
		Segment *top = new Segment(Point(minX, maxY), Point(maxX, maxY));
		Segment *bot = new Segment(Point(minX, minY), Point(maxX, minY));
		Point left = Point(minX, maxY);
		Point right = Point(maxX, maxY);
		trapezoid->name = "bounding box";

		trapezoid->top = top;
		trapezoid->bot = bot;
		trapezoid->left = left;
		trapezoid->right = right;

		Node *initRoot = new SquareNode(trapezoid);
		root = initRoot;
		trapezoid->node = root;

		// // start by inserting the segment one by one in a random order
		// insert(top);
		// insert(bot);

		// permute the list of segments

		// insert the segments
		for (int i = 0; i < segments.size(); ++i)
			insert(segments[i]);
	}

	Trapezoid* findFace(Point target) {
		Node* cur = root;
		while (!cur->getTrapezoid()) {
			cur = cur->next(target);
		}

		return cur->getTrapezoid();
	}

	// function to insert a new segment into our trapezoidal map
	void insert(Segment *segment) {
		// find the trapezoid that contains the left endpoint first
		// find the trapezoid that contains the right endpoint
		Trapezoid* beginTrape = findFace(segment->pLeft);
		Trapezoid* endTrape = findFace(segment->pRight);

		// fix the trapezoid map and DAG
		// 2 case:
		// - both segment endpoint lies in the same face, this one is easy
		// - they lie in different faces, need more work.

		// CASE 1:
		if (beginTrape == endTrape) {
			// new trapezoid to insert
			Trapezoid* leftTrape = new Trapezoid();		// leftTrape->name = "left";
			Trapezoid* rightTrape = new Trapezoid();	// rightTrape->name = "right";
			Trapezoid* topTrape = new Trapezoid();		// topTrape->name = "top";
			Trapezoid* botTrape = new Trapezoid();		// botTrape->name = "bot";

			// properly setting up leftTrape
			leftTrape->top = beginTrape->top;
			leftTrape->bot = beginTrape->bot;

			leftTrape->upLeft = beginTrape->upLeft;
			leftTrape->lowLeft = beginTrape->lowLeft;
			leftTrape->upRight = topTrape;
			leftTrape->lowRight = botTrape;
			
			leftTrape->left = beginTrape->left;
			leftTrape->right = segment->pLeft;

			// properly setting up topTrape
			topTrape->top = beginTrape->top;
			topTrape->bot = segment;

			topTrape->upLeft = leftTrape;
			topTrape->upRight = rightTrape;

			topTrape->left = segment->pLeft;
			topTrape->right = segment->pRight;

			// properly setting up botTrape
			botTrape->top = segment;
			botTrape->bot = beginTrape->bot;

			botTrape->upLeft = leftTrape;
			botTrape->upRight = rightTrape;

			botTrape->left = segment->pLeft;
			botTrape->right = segment->pRight;

			// properly setting up rightTrape
			rightTrape->top = beginTrape->top;
			rightTrape->bot = beginTrape->bot;

			rightTrape->upLeft = topTrape;
			rightTrape->lowLeft = botTrape;
			rightTrape->upRight = beginTrape->upRight;
			rightTrape->lowRight = beginTrape->lowRight;
			
			rightTrape->left = segment->pRight;
			rightTrape->right = beginTrape->right;

			// fixing the neighbor of original trape
			if (beginTrape->upLeft != NULL) {
				if (beginTrape->upLeft->upRight == beginTrape)
					beginTrape->upLeft->upRight = leftTrape;
				else
					beginTrape->upLeft->lowRight = leftTrape;
			}

			if (beginTrape->lowLeft != NULL) {
				if (beginTrape->lowLeft->upRight == beginTrape)
					beginTrape->lowLeft->upRight = leftTrape;
				else
					beginTrape->lowLeft->lowRight = leftTrape;
			}

			if (beginTrape->upRight != NULL) {
				if (beginTrape->upRight->upLeft == beginTrape)
					beginTrape->upRight->upLeft = rightTrape;
				else
					beginTrape->upRight->lowLeft = rightTrape;
			}

			if (beginTrape->lowRight != NULL) {
				if (beginTrape->lowRight->upLeft == beginTrape)
					beginTrape->lowRight->upLeft = rightTrape;
				else
					beginTrape->lowRight->lowLeft = rightTrape;
			}

			// fixing DAG
			Node* leftPNode = new XNode(segment->pLeft);
			Node* rightPNode = new XNode(segment->pRight);
			Node* segNode = new YNode(segment);

			Node* oldSquareNode = beginTrape->node;
			for (int i = 0; i < oldSquareNode->parent.size(); ++i) {
				Node* curPar = oldSquareNode->parent[i];
				if (oldSquareNode == curPar->left) {
					curPar->left = leftPNode;
					leftPNode->parent.push_back(curPar);
				} else {
					curPar->right = leftPNode;
					leftPNode->parent.push_back(curPar);
				}
			}

			if (root == oldSquareNode) {
				root = leftPNode;
			}

			delete oldSquareNode;

			Node* leftTNode = new SquareNode(leftTrape);		leftTrape->node = leftTNode;
			Node* topTNode = new SquareNode(topTrape);			topTrape->node = topTNode;
			Node* botTNode = new SquareNode(botTrape);			botTrape->node = botTNode;
			Node* rightTNode = new SquareNode(rightTrape);		rightTrape->node = rightTNode;

			leftPNode->left = leftTNode;
			leftTNode->parent.push_back(leftPNode);

			leftPNode->right = rightPNode;
			rightPNode->parent.push_back(leftPNode);

			rightPNode->right = rightTNode;
			rightTNode->parent.push_back(rightPNode);

			rightPNode->left = segNode;
			segNode->parent.push_back(rightPNode);

			segNode->left = topTNode;
			topTNode->parent.push_back(segNode);

			segNode->right = botTNode;
			botTNode->parent.push_back(segNode);
		}

		// CASE 2:
		else {
			vector <Trapezoid*> deleted; 

			// fixing begin trapezoid
			Trapezoid* leftTrape = new Trapezoid(*beginTrape);
			leftTrape->right = segment->pLeft;

			// the after merged trapezoid on top of segment
			Trapezoid* mergeTopTrape = new Trapezoid(*beginTrape);
			mergeTopTrape->left = segment->pLeft;
			mergeTopTrape->bot = segment;
			Node* topTNode = new SquareNode(mergeTopTrape);

			// the after merged trapezoid below the segment
			Trapezoid* mergeBotTrape = new Trapezoid(*beginTrape);
			mergeBotTrape->left = segment->pLeft;
			mergeBotTrape->top = segment;
			Node* botTNode = new SquareNode(mergeBotTrape);

			// fixing left trape's neighbor
			leftTrape->upRight = mergeTopTrape;
			leftTrape->lowRight = mergeBotTrape;

			// neighbor of beginTrape is now leftTrape
			if (beginTrape->upLeft != NULL) {
				if (beginTrape->upLeft->upRight == beginTrape)
					beginTrape->upLeft->upRight = leftTrape;
				else
					beginTrape->upLeft->lowRight = leftTrape;
			}

			if (beginTrape->lowLeft != NULL) {
				if (beginTrape->lowLeft->upRight == beginTrape)
					beginTrape->lowLeft->upRight = leftTrape;
				else
					beginTrape->lowLeft->lowRight = leftTrape;
			}

			// neighbor of mergeTop and mergeBot
			mergeTopTrape->upLeft = leftTrape;
			mergeBotTrape->upLeft = leftTrape;

			// fixing the graph for now
			Node* leftPNode = new XNode(segment->pLeft);
			Node* segNode = new YNode(segment);
			Node* leftTNode = new SquareNode(leftTrape);
			leftPNode->left = leftTNode;
			leftPNode->right = segNode;
			segNode->left = topTNode;
			segNode->right = botTNode;

			Trapezoid* previousTrape = beginTrape;
			Trapezoid* currentTrape;
			
			// determining whether the next trape should be merge to top or bottom trape
			if (previousTrape->upRight) {
				if (previousTrape->lowRight) {
					if (previousTrape->upRight->bot->isAbove(segment->pLeft))
						currentTrape = previousTrape->upRight;
					else
						currentTrape = previousTrape->lowRight;
				} else {
					currentTrape = previousTrape->upRight;
				}
			} else {
				currentTrape = previousTrape->lowRight;
			}

			// square node corresponding to begin trapezoid is now gone
			Node* beginNode = beginTrape->node;
			for (int i = 0; i < beginNode->parent.size(); ++i) {
				Node* par = beginNode->parent[i];
				if (par->left == beginNode)
					par->left = leftPNode;
				else
					par->right = leftPNode;

				leftPNode->parent.push_back(par);
			}

			deleted.push_back(beginTrape);

			// keep merging the trapezoids in the intersection
			while (true) {
				bool fixTop = false;
				if (previousTrape->upRight && previousTrape->lowRight) {
					if (previousTrape->lowRight == currentTrape)
						fixTop = true;
				} else {
					if (currentTrape->lowLeft == previousTrape)
						fixTop = true;
				}

				if (fixTop) {
					// updating the merge top by migrating from that one to a new merge top node
					mergeTopTrape->right = currentTrape->left;
					Trapezoid* oldMergeTopTrape = mergeTopTrape;
					mergeTopTrape = new Trapezoid(*currentTrape);
					topTNode = new SquareNode(mergeTopTrape);
					mergeTopTrape->bot = segment;

					if (currentTrape->lowLeft && currentTrape->upLeft) {
						oldMergeTopTrape->upRight = mergeTopTrape;
						mergeTopTrape->lowLeft = oldMergeTopTrape;
						mergeTopTrape->upLeft = currentTrape->upLeft;
						currentTrape->upLeft->upRight = mergeTopTrape;
					} else {
						oldMergeTopTrape->lowRight = mergeTopTrape;
						oldMergeTopTrape->upRight = previousTrape->upRight;
						previousTrape->upRight->upLeft = oldMergeTopTrape;
						mergeTopTrape->upLeft = oldMergeTopTrape;
					}
				} else {
					// updating the merge bot by migrating from that one to a new merge bot node
					mergeBotTrape->right = currentTrape->left;
					Trapezoid* oldMergeBotTrape = mergeBotTrape;
					mergeBotTrape = new Trapezoid(*currentTrape);
					botTNode = new SquareNode(mergeBotTrape);
					mergeBotTrape->top = segment;

					if (currentTrape->lowLeft && currentTrape->upLeft) {
						oldMergeBotTrape->upRight = mergeBotTrape;
						mergeBotTrape->upLeft = oldMergeBotTrape;
						mergeBotTrape->lowLeft = currentTrape->lowLeft;
						currentTrape->lowLeft->upRight = mergeBotTrape;
					} else {
						oldMergeBotTrape->upRight = mergeBotTrape;
						oldMergeBotTrape->lowRight = previousTrape->lowRight;
						previousTrape->lowRight->upLeft = oldMergeBotTrape;
						mergeBotTrape->upLeft = oldMergeBotTrape;
					}
				}

				// don't need to get the next trapezoid anymore
				if (currentTrape == endTrape)
					break ;

				// fixing the graph
				segNode = new YNode(segment);
				segNode->left = topTNode;
				segNode->right = botTNode;
				Node* currentNode = currentTrape->node;

				for (int i = 0; i < currentNode->parent.size(); ++i) {
					Node* par = currentNode->parent[i];
					if (par->left == currentNode)
						par->left = segNode;
					else
						par->right = segNode;

					segNode->parent.push_back(par);
				}

				deleted.push_back(currentTrape);
				previousTrape = currentTrape;
				if (previousTrape->upRight) {
					if (previousTrape->lowRight) {
						if (previousTrape->upRight->bot->isAbove(segment->pLeft))
							currentTrape = previousTrape->upRight;
						else
							currentTrape = previousTrape->lowRight;
					} else {
						currentTrape = previousTrape->upRight;
					}
				} else {
					currentTrape = previousTrape->lowRight;
				}
			}

			// fixing end trapezoid
			mergeTopTrape->right = segment->pRight;
			mergeBotTrape->right = segment->pRight;

			Trapezoid* rightTrape = new Trapezoid(*endTrape);
			rightTrape->left = segment->pRight;

			mergeTopTrape->upRight = rightTrape;
			mergeBotTrape->upRight = rightTrape;
			rightTrape->upLeft = mergeTopTrape;
			rightTrape->lowLeft = mergeBotTrape;
			if (endTrape->upRight != NULL) {
				if (endTrape->upRight->upLeft == endTrape)
					endTrape->upRight->upLeft = rightTrape;
				else
					endTrape->upRight->lowLeft = rightTrape;
			}

			if (endTrape->lowRight != NULL) {
				if (endTrape->lowRight->upLeft == endTrape)
					endTrape->lowRight->upLeft = rightTrape;
				else
					endTrape->lowRight->lowLeft = rightTrape;
			}

			Node* rightPNode = new XNode(segment->pRight);
			segNode = new YNode(segment);
			Node* rightTNode = new SquareNode(rightTrape);
			rightPNode->right = rightTNode;
			rightPNode->left = segNode;
			segNode->left = topTNode;
			segNode->right = botTNode;

			Node* endNode = endTrape->node;
			for (int i = 0; i < endNode->parent.size(); ++i) {
				Node* par = endNode->parent[i];
				if (par->left == endNode)
					par->left = rightPNode;
				else
					par->right = rightPNode;

				rightPNode->parent.push_back(par);
			}

			deleted.push_back(endTrape);

			for (int i = 0; i < deleted.size(); ++i)
				delete deleted[i]->node;
		}
	}
};