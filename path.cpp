#include "path.h"
#include <queue>
#include <stack>
using namespace std;

path::path(const PNG & im, pair<int,int> s, pair<int,int> e)
   :start(s),end(e),image(im){
    BFS();
}

void path::BFS(){
	// initialize working vectors
	vector<vector<bool>> V(image.height(), vector<bool> (image.width(),false));
	vector<vector<pair<int,int>>> P(image.height(), vector<pair<int,int>> (image.width(),end));

    /* your code here */

	queue<pair<int, int>> queue;

	queue.push(start);
	
	while (!queue.empty()) {
		pair<int, int> curr = queue.front();
		queue.pop();

		if (V[curr.second][curr.first] == false) {
			V[curr.second][curr.first] = true;
			vector<pair<int, int>> listOfNeighbours = neighbors(curr);

			for (int i = 0; i < (int) listOfNeighbours.size(); i++) {
				if (good(V, curr, listOfNeighbours[i])) {					
					P[listOfNeighbours[i].second][listOfNeighbours[i].first] = curr;
					queue.push(listOfNeighbours[i]);
				}
			}

		}
	}

	P[start.second][start.first] = start;
	V[start.second][start.first] = true;


	pathPts = assemble(P,start,end);
}

PNG path::render(){

    /* your code here */

	PNG temp = image;

	for (int i = 0; i < (int) pathPts.size(); i++) {
		pair<int, int> currpos = pathPts[i];

		RGBAPixel* currpixel = temp.getPixel(currpos.first, currpos.second);

		currpixel->r = 255;
		currpixel->g = 0;
		currpixel->b = 0;
	}
	
	return temp;
}

vector<pair<int,int>> path::getPath() { return pathPts;}

int path::length() { return pathPts.size();}

bool path::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){

    /* your code here */

	if (next.first >= 0 && (unsigned int) next.first < image.width() && next.second >= 0 && (unsigned int) next.second < image.height()) {
		if (v[next.second][next.first] == false) {
			
			RGBAPixel* current = image.getPixel(curr.first, curr.second);
			RGBAPixel* neighbour = image.getPixel(next.first, next.second);

			if (closeEnough(*current, *neighbour)) {
				return true;
			}

		}
	}

	return false;

}

vector<pair<int,int>> path::neighbors(pair<int,int> curr) {
	vector<pair<int,int>> n;

    /* your code here */

	pair<int, int> up = make_pair(curr.first, curr.second-1);
	pair<int, int> down = make_pair(curr.first, curr.second + 1);
	pair<int, int> left = make_pair(curr.first - 1, curr.second);
	pair<int, int> right = make_pair(curr.first + 1, curr.second);


	n.push_back(up);
	n.push_back(down);
	n.push_back(left);
	n.push_back(right);

	return n;
}

vector<pair<int,int>> path::assemble(vector<vector<pair<int,int>>> & p,pair<int,int> s, pair<int,int> e) {

    /* hint, gold code contains the following line:
	stack<pair<int,int>> S; */

    /* your code here */


	vector<pair<int, int>> temp;

	if (p[end.second][end.first] == e) {
		temp.push_back(s);
		return temp;
	}

	stack<pair<int, int>> stack;
	stack.push(e);

	pair<int, int> curr = e;

	while (curr != s) {
		pair<int, int> pred = p[curr.second][curr.first];

		stack.push(pred);

		curr = pred;
	}

	while (!stack.empty()) {
		temp.push_back(stack.top());
		stack.pop();
	}

	return temp;

}

bool path::closeEnough(RGBAPixel p1, RGBAPixel p2){
   int dist = (p1.r-p2.r)*(p1.r-p2.r) + (p1.g-p2.g)*(p1.g-p2.g) +
               (p1.b-p2.b)*(p1.b-p2.b);

   return (dist <= 80);
}
