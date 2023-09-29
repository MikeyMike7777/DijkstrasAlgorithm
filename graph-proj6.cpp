/*
* file: graph-proj6-cpp
* author: Michael Mathews
* course: CSI 3334
* assignment: project 6
* due date: April 29, 2022
* version: 11.7
 
 This file implements the Graph class while using an ArrayHeap and Edge class
 for implementation. This Graph helps organize all connections between vertices
*/

#include "graph-proj6.h"
#include "arrayheap-student-proj6.h"

/* addEdge
*  parameters:
*      from - index of vertex that is start of edge
*      to - index of vertex that is end of edge
*      cost - value assigned to the edge line itself
*  return value: none
*
* This puts another edge on a Graph
*/
void Graph::addEdge(int from, int to, int cost){
    assert(inVertexRange(to));
    assert(inVertexRange(from));
    Edge newEdge(to, cost);
    adjacencyList[from].push_back(newEdge);
}

/* dijkstra
*  parameters:
*      source - index of a starting vertex to explore from
*  return value: none
*
* This function implements dijkstra's algorithm and finds the least cost
* from the source vertex to every other vertex on the Graph. This cost is
* "infinite" if there is no way to get from the source to a certain vertex
*/
vector<int> Graph::dijkstra(int source) const{
    assert(inVertexRange(source));
    //init priority queue - pair<cost, vtx>
    ArrayHeap<pair<int, int>> q;
    //init keys
    int key[this->adjacencyList.size()];
    //init costs from source to vert
    vector<int> DIST;
    for(int i = 0; i < this->adjacencyList.size(); i++){
        DIST.push_back(INFINITE_COST);
        key[i] = -1;
    }
    DIST[source] = 0;
    
    //place (0,s) on priority queue
    pair<int, int> curr(DIST[source], source), next;
    key[source] = q.insert(curr);
    
    //while queue is not empty
    while(q.getNumItems() > 0){
        curr = q.getMinItem();
        q.removeMinItem();
        key[curr.second] = -1;
        //iterate through list of edges connected to current vertex
        for(auto i = adjacencyList[curr.second].begin();
            i != adjacencyList[curr.second].end(); i++){
            //set neighbor vertex
            next.first = i->cost;
            next.second = i->to;
            //if src to neighbor vertex cost is less than current estimation
            //relaxation
            if(DIST[curr.second]+next.first < DIST[next.second]){
                //change estimation
                DIST[next.second] = DIST[curr.second]+next.first;
                //if it is on heap, change, else, insert
                if(q.isOnHeap(key[next.second])){
                    q.changeItemAtKey(key[next.second], next);
                }
                else{
                    key[next.second] = q.insert(next);
                }
            }
        }
    }
    return DIST;
}
