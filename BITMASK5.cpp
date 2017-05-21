//============================================================================
// Name        : SegmentTrees.cpp
// Author      : Siddhartha Sahani
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

/*
 * This class represents the Segment Tree DS. And specifically in this program Segment tree is used to solve the bitmask problem
 * Question source :- https://www.codechef.com/problems/BITMASK5/
 */
class SegmentTrees {
private:
	int* st;
	bool* lazyArr; // This additional array is use to store the flip status used in lazy propagation.
	int len;

	int getMid(int ss, int se) {
		return ss + (se - ss)/2;
	}

	int getLeftChildIndex(int si) {
		return 2 * si + 1;
	}

	int getRightChildIndex(int si) {
		return 2 * si + 2;
	}

	void intializeSegmentTree(int stringLen, bool initializeWithZero) {
		len = stringLen;
		int height = (int)ceil(log2(len));
		int stSize = (int) (2 * pow(2, height)) - 1;
		st = new int[stSize];
		lazyArr = new bool[stSize];
		createSegmentTreeUtil(0, len-1, 0, initializeWithZero);
	}

	int createSegmentTreeUtil(int ss, int se, int si, bool initializeWithZero) {

		int initialBits = initializeWithZero ? 0 : 1;

		// If its a leaf node, then we need to set the bits to 0 if initializeWithZero is set.
		if (ss == se) {
			st[si] = initialBits;
		} else {
			int mid = getMid(ss, se);
			st[si] = createSegmentTreeUtil(ss, mid, getLeftChildIndex(si), initializeWithZero)
					+ createSegmentTreeUtil(mid+1, se, getRightChildIndex(si), initializeWithZero);
		}
		return st[si];
	}

	void updateSegmentTreeUtil(int ss, int se, int us, int ue, int si) {

		// If the current node has a update pending, then first perform the pending update.
		if (lazyArr[si]) {
			int N = (se - ss) + 1;
			int K = st[si];
			st[si] = N - K;  // we need to flip the existing count. For ex. if the no of elements in the given range is
							 // N and the current number of 1's in the given range is K, then the new count for 1's after flip
							 // will be N - K.

			if (ss != se) {
				// delay the updates on the child nodes by setting the flag in the lazy array.
				int leftChildIndex = getLeftChildIndex(si);
				int rightChildIndex = getRightChildIndex(si);

				lazyArr[leftChildIndex] = !lazyArr[leftChildIndex];
				lazyArr[rightChildIndex] = !lazyArr[rightChildIndex];
			}

			lazyArr[si] = false;  // Reset the lazy flag as pending update on this node is completed.
		}

		// if the current node is completely out of range, then do nothing.
		if (ss > se || ss > ue || se < us) {
			return;
		}


		// If the current node is completely in range, then update the current node and set the lazy flags for its child nodes.
		if (us <= ss && ue >= se) {
			st[si] = (se - ss) + 1 - st[si];

			if (ss != se) {
				int leftChildIndex = getLeftChildIndex(si);
				int rightChildIndex = getRightChildIndex(si);

				lazyArr[leftChildIndex] = !lazyArr[leftChildIndex];
				lazyArr[rightChildIndex] = !lazyArr[rightChildIndex];
			}
			return;
		}

		int mid = getMid(ss, se);
		updateSegmentTreeUtil(ss, mid, us, ue, getLeftChildIndex(si));
		updateSegmentTreeUtil(mid+1, se, us, ue, getRightChildIndex(si));
		st[si] = st[getLeftChildIndex(si)] + st[getRightChildIndex(si)];
	}

	int findUtil(int ss, int se, int qs, int qe, int si) {

		// If the current node has a update pending, then first perform the pending update.
		if (lazyArr[si]) {
			int N = (se - ss) + 1;
			int K = st[si];
			st[si] = N - K;  // we just need to flip the existing count. For ex. if the no of elements in the given range is
							 // N and the current number of 1's in the given range is K, then the new count for 1's after flip
							 // will be N - K.

			if (ss != se) {
				// delay the updates on the child nodes by setting the flag in the lazy array.
				int leftChildIndex = getLeftChildIndex(si);
				int rightChildIndex = getRightChildIndex(si);

				lazyArr[leftChildIndex] = !lazyArr[leftChildIndex];
				lazyArr[rightChildIndex] = !lazyArr[rightChildIndex];
			}

			lazyArr[si] = false;
		}

		// if the current node is completely out of range, then do nothing.
		if (ss > se || ss > qe || se < qs) {
			return 0;
		}


		if (qs <= ss && qe >= se) {
			return st[si];
		}

		int mid = getMid(ss, se);
		return findUtil(ss, mid, qs, qe, getLeftChildIndex(si)) + findUtil(mid+1, se, qs, qe, getRightChildIndex(si));
	}

public:
	SegmentTrees(int stringLen, bool initializeWithZero = true) {
		intializeSegmentTree(stringLen, initializeWithZero);
	}

	void flipBitsInRange(int l, int r) {
		updateSegmentTreeUtil(0, len-1, l, r, 0);
	}

	int countOnBitsInRange(int l, int r) {
		return findUtil(0, len-1, l, r, 0);
	}

};

int main() {
	int N, Q;
	cin>>N>>Q;
	SegmentTrees st(N);
	while (Q--) {
		int f, a, b;
		cin>>f>>a>>b;
		if (a > b) {
			int temp = a;
			a = b;
			b = temp;
		}
		if (f == 1) {
			cout<<st.countOnBitsInRange(a, b)<<"\n";
		} else {
			st.flipBitsInRange(a, b);
		}
	}
	return 0;
}
