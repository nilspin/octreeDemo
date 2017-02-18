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
    int children[8]={-1,-1,-1,-1,-1,-1,-1,-1};
	int parent;
	vec3 center;
	float halfSize;
	vec3 data; //actual point stored at this grid cell
	
	int level = 0;
	int maxLevel = 10;
    float minCellWidth = 0.0001;

/*
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

        return true;
    };
*/

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
        vector<int> a = {children[0],children[1],children[2],
                    children[3],children[4],children[5],
                    children[6],children[7]};
//        int (&c)[8] = children;
        return a;
    };

	octreeNode()
	{
		ID = numNodes;
		numNodes++;
		root = ID;
		parent = 0;
        //center = vec3(0,0,0);
		//halfSize = 2.0;
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

//	void insert(vec3 point)
//	{
//		insert(ID, point);
//	};

	void setData(vec3 p)
	{
		data = p;
		isDataSet = true;
	};

    void insert(vec3 point)
    {
    	if(isLeaf)
        {
    		if(isDataSet)
    		{
    			//create 2 child nodes and insert oldData and point

                //--------------------------------
    			vec3 oldData = data;
                float childWidth = this->halfSize*0.5;

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
            int childID = table[this->ID].children[index];
            octreeNode& child = table[childID];
    		child.insert(point);
    	}

        /*if(!isDataSet)
        {
            if(isLeaf)
            {
                setData(point);
            }
            else
            {
                int child_number = getOctantContainingPoint(point);
                octreeNode& child = table[children[child_number]];
                child.insert(point);
            }
        }
        else
        {
            //find octants where old and new data will go
            //insert those points respectively
            float childWidth = 0.5*halfSize;
            vec3 oldData = data;

            if(childWidth < minCellWidth)
            {
                cout<<"Point could not be added, grid cell size is too small. skipping.\n";
                return;
            }

            int child1_number = getOctantContainingPoint(oldData);
            int child2_number = getOctantContainingPoint(point);
            vec3 child1_center = getChildCenter(center, child1_number, childWidth);
            vec3 child2_center = getChildCenter(center, child2_number, childWidth);
            octreeNode child1(ID, level+1, child1_center, halfSize*0.5,child2_number);
            child1.insert(oldData);
            table[this->ID].children[child1_number] = child1.ID;

            octreeNode child2(ID, level+1, child2_center, halfSize*0.5,child2_number);
            child2.insert(point);
            table[this->ID].children[child2_number] = child2.ID;

            isDataSet = false;
            isLeaf = false;
        }
        */
    };
};

#endif
