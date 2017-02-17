#include "octree.h"
#include <iostream>
#include <random>
#include <cmath>

//using namespace std;
const int numPoints=2;

int main()
{
	std::random_device rd;
    std::mt19937 gen(rd());
 
    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    std::normal_distribution<> d(0,1);
//    std::vector<vec3> pointList;
//    for(int i=0;i<numPoints;++i)
//    {
//    	pointList.emplace_back(vec3(d(gen),d(gen),d(gen)));
//    	vec3 p = pointList[i];
//    	cout<<i<<") "<<p.x<<" "<<p.y<<" "<<p.z<<"\n";
//    }

	octreeNode rootNode(0,0, vec3(0,0,0), 2.0);
//	for(auto i=0;i<pointList.size();++i)
//	{
//		rootNode.insert(pointList[i]);
//	}
    rootNode.insert(vec3(0.4,0.12,-1.3));
    rootNode.insert(vec3(-0.4,0.12,-1.3));
    cout<<"====================================\n";
    for(auto &i : table)
    {
        cout<<"ID: "<<i.ID<<", numNodes"<<numNodes<<"\n";
    }
	return 0;
}
