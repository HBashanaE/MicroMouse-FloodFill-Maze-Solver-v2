#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>

using namespace std;

// Define the directions for movement (up, down, left, right)
int dRow[] = {-1, 1, 0, 0};
int dCol[] = {0, 0, -1, 1};

// Helper function to check if the cell is valid for flood fill
bool isValid(int row, int col, vector<vector<int> > &maze, vector<vector<bool> > &visited) {
    int n = maze.size();
    int m = maze[0].size();
    return (row >= 0 && row < n && col >= 0 && col < m && maze[row][col] == 0 && !visited[row][col]);
}

// Function to solve the maze using flood fill algorithm (BFS)
// Now this function returns the path if one is found
vector<pair<int, int>> floodFill(vector<vector<int> > &maze, pair<int, int> start, pair<int, int> end) {
    int n = maze.size();
    int m = maze[0].size();
    
    // Create a visited matrix
    vector<vector<bool> > visited(n, vector<bool>(m, false));
    
    // Create a parent map to track the path (store parent cells)
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(m, {-1, -1}));
    
    // Queue for BFS
    queue<pair<int, int> > q;
    
    // Mark the starting point as visited and push it into the queue
    q.push(make_pair(start.first, start.second));
    visited[start.first][start.second] = true;
    
    // Perform BFS
    while (!q.empty()) {
        pair<int, int> cell = q.front();
        q.pop();
        
        int row = cell.first;
        int col = cell.second;
        
        // If we reach the destination, reconstruct the path
        if (row == end.first && col == end.second) {
            vector<pair<int, int>> path;
            pair<int, int> current = end;
            
            // Backtrack from end to start using the parent map
            while (current != make_pair(-1, -1)) {
                path.push_back(current);
                current = parent[current.first][current.second];
            }
            
            // Reverse the path since we backtracked
            reverse(path.begin(), path.end());
            return path;
        }
        
        // Explore all 4 possible directions
        for (int i = 0; i < 4; i++) {
            int newRow = row + dRow[i];
            int newCol = col + dCol[i];
            
            if (isValid(newRow, newCol, maze, visited)) {
                visited[newRow][newCol] = true;
                q.push(make_pair(newRow, newCol));
                
                // Store the parent for path reconstruction
                parent[newRow][newCol] = make_pair(row, col);
            }
        }
    }
    
    // If we exhaust the search without reaching the destination, return an empty path
    return {};
}

// Function to read the maze, start point, and end point from a file
bool readMazeFromFile(const string &filename, vector<vector<int> > &maze, pair<int, int> &start, pair<int, int> &end) {
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "Could not open the file." << endl;
        return false;
    }

    string line;
    int rowCount = 0;
    int n = 0; // Number of rows
    int m = 0; // Number of columns

    // Read the maze grid
    while (getline(file, line)) {
        cout << "Read line: " << line << endl;
        if (line.empty()) continue; // Skip any empty lines

        stringstream ss(line);
        vector<int> row;
        int value;

        // Read each value (0 or 1) in the current row
        while (ss >> value) {
            row.push_back(value);
        }

        // Check if we are done reading the maze
        if (row.size() == 0) break;

        // If this is the first row, set the number of columns
        if (rowCount == 0) {
            m = row.size();
        }

        maze.push_back(row);
        rowCount++;
    }

    // n = rowCount; // Number of rows

    // // Read start and end points
    // if (file >> start.first >> start.second) {
    //     if (file >> end.first >> end.second) {
    return true;
    //     }
    // }

    // return false;
}

int main() {
    vector<vector<int> > maze;
    pair<int, int> start = make_pair(0, 0);  // Start point at (0, 0)
    pair<int, int> end = make_pair(6, 6);    // End point at (4, 4)

    // File name for reading maze
    string filename = "maze.txt";
    
    // Read maze and start/end points from file
    if (!readMazeFromFile(filename, maze, start, end)) {
        cout << "Error reading maze file." << endl;
        return 1;
    }
    
    // Find the path using flood fill
    vector<pair<int, int>> path = floodFill(maze, start, end);
    
    // Print the result
    if (!path.empty()) {
        cout << "Path found from start to destination:" << endl;
        for (auto cell : path) {
            cout << "(" << cell.first << ", " << cell.second << ") -> ";
        }
        cout << "End" << endl;
    } else {
        cout << "No path found!" << endl;
    }
    
    return 0;
}
