from itertools import combinations

def rotate_90(x, y):
    """Rotate a point (x, y) by 90 degrees clockwise in a 5x5 grid."""
    return (y, 4 - x)

def generate_rotations(pRed, pBlue):
    """Generate all 90-degree rotations of a given pair of points."""
    rotations = set()
    for _ in range(4):
        pRed, pBlue = rotate_90(*pRed), rotate_90(*pBlue)
        rotations.add(tuple(sorted([pRed, pBlue])))
    return rotations

def unique_positions(grid_size=5):
    """Find unique pairs of positions in a grid considering rotational symmetry."""
    all_pairs = list(combinations([(x, y) for x in range(grid_size) for y in range(grid_size)], 2))
    unique_sets = set()

    for pRed, pBlue in all_pairs:
        rotations = generate_rotations(pRed, pBlue)
        # Only store one representative from the rotation set
        unique_sets.add(min(rotations))

    return sorted(unique_sets)

def print_grid(pRed, pBlue, grid_size=5):
    """Print the grid with positions A and B marked."""
    grid = [['.' for _ in range(grid_size)] for _ in range(grid_size)]
    grid[pRed[1]][pRed[0]] = 'A'  # Note: row is y, column is x
    grid[pBlue[1]][pBlue[0]] = 'B'
    
    print("+" + "-" * (2 * grid_size - 1) + "+")
    for row in grid:
        print("| " + " ".join(row) + " |")
    print("+" + "-" * (2 * grid_size - 1) + "+")

# Compute unique placements
unique_pairs = unique_positions()
print(f"Number of unique placements: {len(unique_pairs)}")
for pair in unique_pairs:
    print(pair)
    print_grid(*pair)
    print()
