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

class octreeNode : std::enable_shared_from_this<octreeNode>
{
private:
	int root;
    vector<int> children={-1,-1,-1,-1,-1,-1,-1,-1};
	int parent;
	vec3 center;
	float halfSize;
	vec3 data; //actual point stored at this grid cell
	
	int level = 0;
	int maxLevel = 10;
    float minCellWidth = 0.0001;

	vec3 getChildCenter(vec3 cen, int cod, float childWidth)
	{
		vec3 childCenter;
        switch(cod)
		{
			case 0: childCenter = cen + vec3(-childWidth, -childWidth, -childWidth);
					break;

			case 1: childCenter = cen + vec3(-childWidth, -childWidth, childWidth);
					break;

			case 2: childCenter = cen + vec3(-childWidth, childWidth, -childWidth);
					break;

			case 3: childCenter = cen + vec3(-childWidth, childWidth, childWidth);
					break;

			case 4: childCenter = cen + vec3(childWidth, -childWidth, -childWidth);
					break;

			case 5: childCenter = cen + vec3(childWidth, -childWidth, childWidth);
					break;

			case 6: childCenter = cen + vec3(childWidth, childWidth, -childWidth);
					break;

			case 7: childCenter = cen + vec3(childWidth, childWidth, childWidth);
					break;
		}
		return childCenter;
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

    const vector<int> getChildren()
    {
        vector<int> a = children;
        return a;
    };

	octreeNode()
	{
		ID = numNodes;
		numNodes++;
		root = ID;
		parent = 0;
        table.push_back(self());
	};

    octreeNode(int p, int l, vec3 c, float hs, int cod)
	{
		parent = p;
		center = c;
		halfSize = hs;
		level = l;
        code = cod;
        ID = numNodes;
		numNodes++;
        isDataSet = false;
        table.push_back(self());
	};

	void setData(vec3 p)
	{
		data = p;
		isDataSet = true;
	};

    void insert(vec3 point)
    {
        float childWidth = this->halfSize*0.5;
        octreeNode& self = table[this->ID];

        if(isLeaf)
        {
    		if(isDataSet)
    		{
                //create 2 child nodes and insert oldData and point

                //--------------------------------
    			vec3 oldData = data;

                //ptr to node where oldData is to be inserted
                int child1_number = getOctantContainingPoint(oldData);
                vec3 child1Center = getChildCenter(this->center, child1_number, childWidth);
                octreeNode child1Node(this->ID,this->level+1,child1Center, childWidth, child1_number);
                table[child1Node.ID].setData(oldData);
                table[this->ID].children[child1_number] = child1Node.ID;

                //ptr to node where new point is to be inserted
                int child2_number = getOctantContainingPoint(point);
                vec3 child2Center = getChildCenter(this->center, child2_number, childWidth);
                octreeNode child2Node(this->ID,this->level+1,child2Center, childWidth, child2_number);
                table[child2Node.ID].setData(point);
                table[this->ID].children[child2_number] = child2Node.ID;
                //--------------------------------


                isLeaf = false;//since we have children now
    		}
            else
            {
                setData(point);
            }
    	}
    	else
    	{
    		//means we have children where data needs to be placed
            int index = getOctantContainingPoint(point);
            vec3 childCenter = getChildCenter(this->center, index, childWidth);
            octreeNode child(this->ID, this->level+1, childCenter, childWidth, index);

            table[child.ID].insert(point);
            table[this->ID].children[index] = child.ID;

        }
    };
};

#endif
