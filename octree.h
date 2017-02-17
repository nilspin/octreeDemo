#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <memory>
#include <iostream>
#include <cstdint>
#include <bitset>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

static int numNodes=0;

class octreeNode;

static std::vector<octreeNode> table;

float minCellWidth = 0.0001;
class octreeNode : std::enable_shared_from_this<octreeNode>
{
private:
	
	int root;
    uint8_t children[8];
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
	
        for(uint8_t i = 0;i<=7;++i)
		{
			vec3 childCenter;
            uint8_t tempCode = i;
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
			octreeNode child(ID, level+1, childCenter, halfSize*0.5);
			child.code = i;
			table.push_back(child);
			//============
			//children[i] = new octreeNode(ID, level+1, childCenter, halfSize*0.5);
            children[i] = i;
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
		}
		else
		{
			//if leaf node
			if(!node.isDataSet)
			{
				node.setData(point);
			}
			else
			{
				//if leaf already contains data then split this node and
				//add insert recursively
				vec3 oldData = data;
				node.isDataSet = false;
				if(!node.split()) return;
				//ptr to node where oldData is to be inserted
				int child1 = node.children[getOctantContainingPoint(oldData)];
				octreeNode& childNode1 = table[child1];
				//ptr to node where new point is to be inserted
				int child2 = node.children[getOctantContainingPoint(point)];
				octreeNode& childNode2 = table[child2];
				
				std::cout<<"Restoring ("<<oldData.x<<" "<<oldData.y<<" "<<oldData.z<<") to node "<<std::bitset<4>(childNode1.code)<<"\n";
				childNode1.insert(child1, oldData);
				
				std::cout<<"Adding ("<<point.x<<" "<<point.y<<" "<<point.z<<") to node "<<std::bitset<4>(childNode2.code)<<"\n";
				childNode2.insert(child2, point);

			}
		}
	};

    uint8_t getOctantContainingPoint(vec3 point)
	{
        uint8_t code = 0;
		if(point.x > center.x) code|=4;
		if(point.y > center.y) code|=2;
		if(point.z > center.z) code|=1;
		return code;
	};

	octreeNode& self(){return *this;};

public:
	int ID;
	bool isLeaf = true;
	bool isDataSet = false;
    uint8_t code;
	octreeNode()
	{
		ID = numNodes;
		numNodes++;
		root = ID;
		parent = 0;
		code = 0;
		//center = vec3(0,0,0);
		//halfSize = 2.0;
		table.push_back(self());
	};

	octreeNode(int p, int l, vec3 c, float hs)
	{
		parent = p;
		center = c;
		halfSize = hs;
		level = l;
		isLeaf = true;
		ID = numNodes;
		numNodes++;
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
