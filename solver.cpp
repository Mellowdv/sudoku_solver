// To solve a sudoku: find first free slot (first 0), replace with 1, check 3 options
// Horizontal, Vertical, Within the Box, if all 3 fail, increment by 1.
// If all numbers 1 to 9 fail, return to previous number and increment it by 1, try again

bool fillGrid(int grid[9][9], int n) {
    int x = 0;
    int y = 0;
    bool found = false;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] == 0) {
                x = i;
                y = j;
                found = true;
                break;
            }
        }
        if (found == true) break;
    }
    if (found == false) return true;

    grid[x][y] = n;
    bool valid = true;
    for (int k = 0; k < 9; k++) {
        if (k == y) continue;
        if (grid[x][k] == grid[x][y]) {
            grid[x][y] = 0;
            return false;
        }
    }
    for (int m = 0; m < 9; m++) {
        if (m == x) continue;
        if (grid[m][y] == grid[x][y]) {
            grid[x][y] = 0;
            return false;
        }

    }
    int verticalBox = x / 3;
    int horizontalBox = y / 3;
    for (int a = verticalBox * 3; a <= (verticalBox + 1) * 3 - 1; a++) {
        for (int b = horizontalBox * 3; b <= (horizontalBox + 1) * 3 - 1; b++) {
            if ((a == x) && (b == y)) continue;
            if (grid[a][b] == grid[x][y]) {
                grid[x][y] = 0;
                return false;
            }
        }
    }
    // In case any of the rules fails
    if ((grid[x][y] == 9) && (valid == false)) {
        grid[x][y] = 0;
        return false;
    }
    for (int z = 1; z < 10; z++) {
        if (fillGrid(grid, z))
            return true;
    }
    grid[x][y] = 0;
    return false;
}

void solve(int t[9][9]) {
  for (int i = 1; i < 10; i++) {
    if (fillGrid(t, i))
      return;
  }
}
