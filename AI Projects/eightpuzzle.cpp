/*
A. Navarez's Submission for CMSC 170 MP-1
Eight puzzle solver: an A* search via manhattan heuristic approach

Boilerplate code from https://www.geeksforgeeks.org/8-puzzle-problem-using-branch-and-bound/
*/
#include <bits/stdc++.h>
using namespace std;
#define N 3
 
// state space tree nodes
struct Node
{
    // stores the parent node of the current node
    // helps in tracing path when the answer is found
    Node* parent;
 
    // stores matrix
    int mat[N][N];
 
    // stores blank tile coordinates
    int x, y;
 
    // stores the number of misplaced tiles
    int cost;
 
    // stores the number of moves so far
    int level;

	// stores the manhattan distance value;
	int distance;
};
 
// Function to print N x N matrix
void printMatrix(int mat[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++){
			if(mat[i][j] != 0)
				cout <<  mat[i][j] << " ";
			else
				cout << "  ";
		}
        cout << endl;
    }
}
 
// Function to allocate a new node
Node* newNode(int mat[N][N], int x, int y, int newX,
              int newY, int level, Node* parent)
{
    Node* node = new Node;
 
    // set pointer for path to root
    node->parent = parent;
 
    // copy data from parent node to current node
    memcpy(node->mat, mat, sizeof node->mat);
 
    // move tile by 1 position
    swap(node->mat[x][y], node->mat[newX][newY]);
 
    // set number of misplaced tiles
    node->cost = INT_MAX;
 
    // set number of moves so far
    node->level = level;
 
    // update new blank tile cordinates
    node->x = newX;
    node->y = newY;
 
	node->distance = INT_MAX;
    return node;
}
 
// bottom, left, top, right
int row[] = { 1, 0, -1, 0 };
int col[] = { 0, -1, 0, 1 };
 
// Function to calculate the number of misplaced tiles
// ie. number of non-blank tiles not in their goal position
int calculateCost(int initial[N][N], int final[N][N])
{
    int count = 0;
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (initial[i][j] && initial[i][j] != final[i][j])
           count++;
    return count;
}

// Function to generate the positions of the goal state
multimap<int, pair<int, int>> goalValue(int mat[N][N]) 
{ 
    multimap<int, pair<int, int>> val; 
    for (int i = 0; i < N; i++) 
      for (int j = 0; j < N; j++) 
            val.insert(make_pair(mat[i][j],make_pair(i,j))); 
 
    return val; 
} 
 
// Function to calculate the manhattan distance between the present state and the goal state 
int manhattan_distance(int initial[N][N], int final[N][N]) 
{ 
    multimap<int, pair<int,int>> finalvals = goalValue(final); 
    int sum = 0;  
    for (int i = 0; i < N; i++) 
      for (int j = 0; j < N; j++) 
        sum += abs(i-finalvals.find(initial[i][j])->second.first) + abs(j-finalvals.find(initial[i][j])->second.second);
    return sum; 
} 
 
// Function to calculate the euclidean distance between the present state and the goal state 
int euclidean_distance(int initial[N][N], int final[N][N]) 
{ 
    multimap<int, pair<int,int>> finalvals = goalValue(final); 
    int sum = 0;  
    for (int i = 0; i < N; i++) 
      for (int j = 0; j < N; j++) 
        sum += sqrt(pow((i-finalvals.find(initial[i][j])->second.first), 2) + pow((j-finalvals.find(initial[i][j])->second.second),2));
    return sum; 
} 

// Function to check if (x, y) is a valid matrix cordinate
int isSafe(int x, int y)
{
    return (x >= 0 && x < N && y >= 0 && y < N);
}
 
// print path from root node to destination node
int printPath(Node* root)
{
    if (root == NULL)
        return 0;

    int n = 1 + printPath(root->parent);
	system("clear");
	cout << n << ". ";
	if(root->cost == 0)
		cout << "Goal ";
	else if(root->parent == NULL)
		cout << "Start ";
	else
		cout << "Next ";
	cout << "State\n";

    printMatrix(root->mat);
    printf("\n");

	if(!(root->cost == 0)){
		cout << "Press enter to continue to next step.\n";
		cin.ignore();
	}

	return n;
 
}
 
// Comparison object to be used to order the heap based on the manhattan distance + level heurisitic
struct heuristic_comp
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return (lhs->level + lhs->distance) > (rhs->level + rhs->distance);
    }
};
 
 
// implemented A* algorithm using manhattan distance heuristic
void astar(int initial[N][N], int x, int y, int final[N][N]){
    // Create a priority queue to store live nodes of
    // search tree;
    priority_queue<Node*, std::vector<Node*>, heuristic_comp> frontier;

    // create a root node and calculate its cost
    Node* root = newNode(initial, x, y, x, y, 0, NULL);
    root->cost = calculateCost(initial, final);
    root->distance= manhattan_distance(initial, final);

	// capping the number of iterations
    int iterations = 0;

    // Add root to list of live nodes;
    frontier.push(root);

	// creates a vector of explored states
    vector<array <int, N*N>> explored;

    // Finds a live node with least cost,
    // add its childrens to list of live nodes and
    // finally deletes it from the list.
    while (!frontier.empty() && iterations <= 100000)
    {
        // Find a live node with least estimated cost
        Node* min = frontier.top();

        // The found node is deleted from the list of
        // live nodes
        frontier.pop();

        // if min is an answer node
        if (min->cost == 0)
        {
            // print the path from root to destination;
            printPath(min);
            return;
        }

		// setting current state into an array
        array<int, N*N> state;
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++)
                state[3*i+j] = min->mat[i][j];
        }

		// check if current state has been explored
        bool found = false;
        for(auto it=explored.begin(); !found && it!=explored.end(); it++){
            array<int, N*N> comp = *it;
            if(comp == state){
                found = true;
            }
        }

		if(!found){
			// add state to explored
			explored.push_back(state);

	        // check for valid moves within the children of min
		    for (int i = 0; i < 4; i++)
			{
				if (isSafe(min->x + row[i], min->y + col[i]))
				{
					// create a child node and calculate
					Node* child = newNode(min->mat, min->x,
				                  min->y, min->x + row[i],
					              min->y + col[i],
						          min->level + 1, min);
	                child->cost = calculateCost(child->mat, final);
		            child->distance = manhattan_distance(child->mat, final);
			        // Add child to list of live nodes
			        frontier.push(child);
				}
			}
			iterations++;
		}
    }
	
	// capping iterations
	if(iterations > 100000)	
		cout << "Solution not found\n";
}

// Driver code
int main()
{
    // Initial configuration
    // Value 0 is used for empty space
    int initial[N][N] =
    {
        {7, 2, 4},
        {5, 0, 6},
        {8, 3, 1}
    };
 
    // Solvable Final configuration
    // Value 0 is used for empty space
    int final[N][N] =
    {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}
    };
 
    // Blank tile coordinates in initial
    // configuration
    int x = 1, y = 1;
 
    astar(initial, x, y, final);
 
    return 0;
}
