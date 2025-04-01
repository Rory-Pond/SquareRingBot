def grid_to_coordinates(grid):
    coordinates = {}
    for i in range(len(grid)):
        for j in range(len(grid[i])):
            coordinates[grid[i][j]] = (i, j)
    return coordinates

# Sample grid
grid = [
    [20, 21, 22, 23, 24],
    [15, 16, 17, 18, 19],
    [10, 11, 12, 13, 14],
    [5, 6, 7, 8, 9],
    [0, 1, 2, 3, 4]
]

coordinates = grid_to_coordinates(grid)

# Example of the result
for value, (x, y) in coordinates.items():
    print(f"Value {value} is at coordinates ({x}, {y})")
