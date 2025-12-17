#include <iostream>
using namespace std;

// Cities
const int RIVERSIDE = 0;
const int MOVAL = 1;
const int PERRIS = 2;
const int HEMET = 3;
const int NUM_CITIES = 4;

// adjacency list
struct City {
    int val, cost;
    City* next;
};

// edge
struct Edge {
    int src, dest, weight;
};

// City name
const char* cityName(int i){
    if (i == 0) return "Riverside";
    else if (i == 1) return "Moreno Valley";
    else if (i == 2) return "Perris";
    else if (i == 3) return "Hemet";
    else return "Unknown";    
}

// Graph
class Graph{
    City* getAdjCity(int value, int weight, City* head){
        City* newCity = new City;
        newCity->val = value;
        newCity->cost = weight;
        newCity->next = head;
        return newCity;
    }

public: 
    City** head;
    
    Graph(const Edge edges[], int edgeCount, int N){
        head = new City*[N]();        
        
        for (int i = 0; i < N; i++)
            head[i] = nullptr;
        
        // add directions
        for (int i = 0; i < edgeCount; i++){
            int src = edges[i].src;
            int dest = edges[i].dest;
            int w = edges[i].weight;
            
            head[src] = getAdjCity(dest, w, head[src]);
            head[dest] = getAdjCity(src, w, head[dest]);
           }
    }
    
    ~Graph() {
        delete[] head;
            }
    };
    
    // print the adjacencies 
    void printList(City* ptr, int from){
        cout << "[" << cityName(from) << "]: ";
        while (ptr != nullptr) {
            cout << "-> (" << cityName(ptr->val)
                 << ", " << ptr->cost << ") ";
            ptr = ptr->next;
        }
        cout << "\n";
    }
    
    // distance
    int getDistance(City* head, int dest) {
        City* cur = head;
        while (cur != nullptr) {
            if (cur->val == dest)
                return cur->cost; 
            cur = cur->next;
        }
        return -1; // no road, error
    }
    
    
    //print matrix using list
    void printAdjacencyMatrix(Graph* g) {
        cout << "\n *****Adjacency Matrix*****\n";
        
        //column
        cout << "      ";
        for (int col = 0; col < NUM_CITIES; col++)
            cout << col << "   ";
        cout << "\n";
        
        //Rows
        for (int row = 0; row < NUM_CITIES; row++){
            cout << row << ":   ";
            
            for (int col = 0; col < NUM_CITIES; col++) {
                if (row == col){
                    cout << "0  ";
                } else {
                    int w = getDistance(g->head[row], col);
                    if (w != -1) cout << w << "  ";
                    else cout << "--  ";
                }
            }
            
            cout << " (" << cityName(row) << ")\n";
        }
        
        cout << "\n";
    }
 
    
// build graph
    Graph* RouteTreeGraph() {
        Edge edges[] = {
            { RIVERSIDE, MOVAL,  16  },
            { RIVERSIDE, PERRIS, 24  },
            { RIVERSIDE, HEMET,  33  },
            { MOVAL,     PERRIS, 18  }, 
            { MOVAL,     HEMET,  26  },
            { PERRIS,    HEMET,  30  },
        };
        
        int edgeCount = sizeof(edges) / sizeof(edges[0]);
        return new Graph(edges, edgeCount, NUM_CITIES);        
    }
            
            
// show both
void showAdj(Graph* g) {
    cout << "Adjacency List:\n";
    for (int i = 0; i < NUM_CITIES; i++)
        printList(g->head[i], i);
    
    printAdjacencyMatrix(g);
}            

// Route options
// Start @ riverside and visit every city at least once
const int ROUTE_COUNT = 6;
const int ROUTE_LEN = 4;

int routes[ROUTE_COUNT][ROUTE_LEN] = {
    { RIVERSIDE, MOVAL,  PERRIS, HEMET  },
    { RIVERSIDE, MOVAL,  HEMET,  PERRIS },
    { RIVERSIDE, PERRIS, MOVAL,  HEMET  },
    { RIVERSIDE, PERRIS, HEMET,  MOVAL  },
    { RIVERSIDE, HEMET,  MOVAL,  PERRIS },
    { RIVERSIDE, HEMET,  PERRIS, MOVAL  }
};

// Print a route
void printRoute(int route[], int len) {
    for (int i = 0; i < len; i++) {
        cout << cityName(route[i]);
        if (i < len - 1) cout << " -> ";
    }
}

// Compute mileage
int routeMiles(Graph* g, int route[], int len) {
    int total = 0;
    
    for (int i = 0; i < len - 1; i++){
        int d = getDistance(g->head[route[i]], route[i +1]);
        if (d == -1) return -1; // invalid route
        total +=d;
    }
    return total;
}

// cost
double routeCost(int miles, double mpg, double gasPrice) {
    return (miles / mpg) * gasPrice;
}


int main() {
    Graph* g = RouteTreeGraph();

    int choice = -1;
    while (choice != 0) {
        cout << "==========================\n";
        cout << " Inland Empire Solar Route\n";
        cout << "==========================\n";
        cout << "1) Show Adjacency List\n";
        cout << "2) Show Adjacency Matrix\n";
        cout << "3) Show Both\n";
        cout << "4) Show Route Variations (all 6)\n";
        cout << "5) Show Shortest / Lowest-Cost Route\n";
        cout << "0) Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "\nAdjacency List:\n";
            for (int i = 0; i < NUM_CITIES; i++)
                printList(g->head[i], i);
            cout << "\n";
        }
        else if (choice == 2) {
            printAdjacencyMatrix(g);
        }
        else if (choice == 3) {
            showAdj(g);
        }
        else if (choice == 4) {
            cout << "\n===== Route Variations (3! = 6) =====\n";
            for (int i = 0; i < ROUTE_COUNT; i++) {
                int miles = routeMiles(g, routes[i], ROUTE_LEN);
                cout << (i + 1) << ") ";
                printRoute(routes[i], ROUTE_LEN);
                cout << " | Miles: " << miles << "\n";
            }
            cout << "\n";
        }
        else if (choice == 5) {
            double mpg, gasPrice;

            cout << "Enter average MPG: ";
            cin >> mpg;
            cout << "Enter gas price per gallon: ";
            cin >> gasPrice;

            int bestIndex = 0;
            int bestMiles = routeMiles(g, routes[0], ROUTE_LEN);

            for (int i = 1; i < ROUTE_COUNT; i++) {
                int miles = routeMiles(g, routes[i], ROUTE_LEN);
                if (miles < bestMiles) {
                    bestMiles = miles;
                    bestIndex = i;
                }
            }

            double cost = routeCost(bestMiles, mpg, gasPrice);

            cout << "\n===== Best Route (Shortest & Lowest Cost) =====\n";
            printRoute(routes[bestIndex], ROUTE_LEN);
            cout << "\nTotal Miles: " << bestMiles << "\n";
            cout << "Estimated Fuel Cost: $" << cost << "\n\n";
        }
        else if (choice == 0) {
            cout << "Goodbye!\n";
        }
        else {
            cout << "Invalid Choice. Try again.\n\n";
        }
    }

    delete g;
    return 0;
}