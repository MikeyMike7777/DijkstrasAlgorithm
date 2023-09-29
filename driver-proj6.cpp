/*
* file: driver-proj6.cpp
* author: Michael Mathews
* course: CSI 3334
* assignment: project 6
* due date: April 29, 2022
* version: 11.7
 
 This file uses a Graph of Edges to find which computers are able to make
 the minimum total transmission delay between two computers. This is only if
 the source computer is a server (denoted by ending in _server) and can connect
 to every other computer. Input begins with the number of computers. It is
 followed by that number of lines contatining
<starting computer> <ending computer> <connection length>
*/

#include <iostream>
#include <string>
#include <map>
#include "arrayheap-student-proj6.h"
#include "graph-proj6.h"

using namespace std;

/* main
*  parameters:
*      argc -- the number of arguments from the command line
*      argv -- the command line argument values
*  return value: 0 (indicating a successful run)
*
* This function finds the minimum total transmission delay between a server
* and all other servers. If there is no server that can do this, it is stated
*/
int main(int argc, const char * argv[]) {
    const int NEWLINE = 1;
    const int NOT_SET = -1;
    const int INVALID = -2;
    int connections, cost, count = 0;
    
    cin >> connections;
    assert(connections > 0 and connections <= 4000);
    
    //find number of computers and create edges
    map<string, int> computers;
    vector<int> costs;
    vector<pair<pair<string, string>, int> > edges;
    pair<pair<string, string>, int> edge;
    vector<int> totals;
    string comp1, comp2, serv = "_server";
    
    for(int i = 0; i < connections; i++){
        cin.ignore(NEWLINE);
        
        //get computer
        getline(cin, comp1, ' ');
        //make sure computer is not already on map
        if(computers.find(comp1) == computers.end()){
            computers.insert(pair<string, int>(comp1,count));
            count++;
        }
        
        //get computer
        getline(cin, comp2, ' ');
        //make sure computer is not already on map
        if(computers.find(comp2) == computers.end()){
            computers.insert(pair<string, int>(comp2,count));
            count++;
        }
        
        cin >> cost;
        
        //store edges to be inserted to graph
        edge.first.first = comp1;
        edge.first.second = comp2;
        edge.second = cost;
        edges.push_back(edge);
    }
    //init graph with number of computers and list of which are servers
    Graph g(count);
    
    for(int i = 0; i < connections; i++){
        //add stored edge by referencing computers indexes stored in map
        g.addEdge(computers[edges[i].first.first],
                  computers[edges[i].first.second], edges[i].second);
    }
    
    //find which vertices are servers
    //get total transmission delay for all servers
    for(int i = 0; i < count; i++){
        costs = g.dijkstra(i);
        //add up all costs
        
        totals.push_back(0);
        for(int j = 0; j < costs.size(); j++){
            if(costs[j] == Graph::INFINITE_COST){
                totals[i] = INVALID;
                break;
            }
            totals[i] += costs[j];
        }
    }
    
    
//
    //find minimum total transmission delay
    //prepare initial cost
    cost = NOT_SET;
    //go through every computer
    for(auto i = computers.begin(); i != computers.end(); i++){
        //find if computer is a server
        if(i->first.find(serv) != string::npos){
            //update cost if it is the first computer
            if(cost == NOT_SET){
                if(totals[i->second] >= 0){
                    cost = totals[i->second];
                }
            }
            //change cost if lower
            else{
                if(totals[i->second] >= 0 and totals[i->second] < cost){
                    cost = totals[i->second];
                }
            }
        }
    }
    
    //init server count
    count = 0;
    //go through computers in alphabetical order
    for(auto i = computers.begin(); i != computers.end(); i++){
        //find if a computer is a server
        if(i->first.find(serv) != string::npos){
            //find if total at computer is the found cost
            if(totals[i->second] == cost){
                //if it is the first computer found to have cost, display
                if(count == 0){
                    cout << "total delay: " << cost << endl;
                    count = 1;
                }
                //display name
                cout << i->first << endl;
            }
        }
    }
    if(count == 0){
        cout << "no server can serve the whole network" << endl;
    }
    
    return 0;
}
