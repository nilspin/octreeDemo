#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <memory>
#include <iostream>
#include <cstdint>
#include <bitset>
#include <algorithm>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

static int numNodes=0;

class octreeNode;

static std::vector<octreeNode> table;

vector<int> childNodeIDs(8);

float minCellWidth = 0.0001;
class octreeNode : std::enable_shared_from_this<octreeNode>
{
private:
	
	int root;
    int children[8]={-1,-1,-1,-1,-1,-1,-1,-1};
	int parent;
	vec3 center;
	float halfSize;
	vec3 data; //actual point stored at this grid cell
	
	int level = 0;
	int maxLevel = 10;

	bool split()
	{
		float childWidth = 0.5*halfSize;

		if(childWidth < minCellWidth)
		{
			cout<<"Point could not be added, grid cell size is too small. skipping.\n";
			return false;
		}
	
        for(int i = 0;i<=7;++i)
		{
			vec3 childCenter;
            //uint8_t tempCode = i;
			switch(i)
			{
				case 0: childCenter = center + vec3(-childWidth, -childWidth, -childWidth);
						break;

				case 1: childCenter = center + vec3(-childWidth, -childWidth, childWidth);
						break;

				case 2: childCenter = center + vec3(-childWidth, childWidth, -childWidth);
						break;

				case 3: childCenter = center + vec3(-childWidth, childWidth, childWidth);
						break;

				case 4: childCenter = center + vec3(childWidth, -childWidth, -childWidth);
						break;

				case 5: childCenter = center + vec3(childWidth, -childWidth, childWidth);
						break;

				case 6: childCenter = center + vec3(childWidth, childWidth, -childWidth);
						break;

				case 7: childCenter = center + vec3(childWidth, childWidth, childWidth);
						break;
			}
			//============
            octreeNode child(ID, level+1, childCenter, halfSize*0.5,i);
            childNodeIDs[i] = child.ID;
//			child.code = i;
            //============
			//children[i] = new octreeNode(ID, level+1, childCenter, halfSize*0.5);
            //children[i] = child.ID;
		}

		isLeaf = false;
		return true;
    };

	void insert(int ptr,vec3 point)
	{
		octreeNode& node = table[ptr];
		if(!node.isLeaf)
		{
			int child = node.children[getOctantContainingPoint(point)];
			octreeNode& childNode = table[child];
			childNode.insert(child, point);
            isLeaf = false;
		}
		else
		{
			//if leaf node
            if(!isDataSet)
			{
                int child_number = getOctantContainingPoint(point);
                int childID = childNodeIDs[child_number];
                octreeNode& childNode = table[childID];
                childNode.setData(point);
                table[childNode.parent].children[child_number] = childID;
                setData(point);
                //isLeaf = false;
			}
			else
			{
                // If leaf already contains data then split this node and
                // insert recursively
				vec3 oldData = data;
				isDataSet = false;

                //childNodeIDs.erase(childNodeIDs.begin(), childNodeIDs.end());
                if(!split()) return;

                //ptr to node where oldData is to be inserted
                int child1_number = getOctantContainingPoint(oldData);
                int child1 = childNodeIDs[child1_number];
                octreeNode& child1Node = table[child1];
                child1Node.setData(oldData);
                table[child1Node.parent].children[child1_number] = child1;

                //ptr to node where new point is to be inserted
                int child2_number = getOctantContainingPoint(point);
                int child2 = childNodeIDs[child2_number];
                octreeNode& child2Node = table[child2];
                child2Node.setData(point);
                table[child2Node.parent].children[child2_number] = child2;
            }
		}
	};

    int getOctantContainingPoint(vec3 point)
	{
        int c = 0;
        if(point.x > center.x) c|=4;
        if(point.y > center.y) c|=2;
        if(point.z > center.z) c|=1;
        return c;
	};

	octreeNode& self(){return *this;};

public:
	int ID;
	bool isLeaf = true;
	bool isDataSet = false;
    int code = -1;
	octreeNode()
	{
		ID = numNodes;
		numNodes++;
		root = ID;
		parent = 0;
        isDataSet = false;
		//center = vec3(0,0,0);
		//halfSize = 2.0;
		table.push_back(self());
	};

    const int getChildren()
    {
        int a[8] = {children[0],children[1],children[2],
                    children[3],children[4],children[5],
                    children[6],children[7]};
//        int (&c)[8] = children;
        return *a;
    };

    octreeNode(int p, int l, vec3 c, float hs, int cod)
	{
		parent = p;
		center = c;
		halfSize = hs;
		level = l;
        code = cod;
		isLeaf = true;
		ID = numNodes;
		numNodes++;
        isDataSet = false;
		table.push_back(self());
	};

	void insert(vec3 point)
	{
		insert(ID, point);
	};

	void setData(vec3 p)
	{
		data = p;
		isDataSet = true;
	};
};

#endif
