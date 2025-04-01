from itertools import combinations

def rotate_90(x, y):
    """Rotate a point (x, y) by 90 degrees clockwise in a 5x5 grid."""
    return (y, 4 - x)

def generate_rotations(p1, p2):
    """Generate all 90-degree rotations of a given pair of points."""
    rotations = set()
    for _ in range(4):
        p1, p2 = rotate_90(*p1), rotate_90(*p2)
        rotations.add(tuple(sorted([p1, p2])))
    return rotations

def unique_positions(grid_size=5):
    """Find unique pairs of positions in a grid considering rotational symmetry."""
    all_pairs = list(combinations([(x, y) for x in range(grid_size) for y in range(grid_size)], 2))
    unique_sets = set()

    for p1, p2 in all_pairs:
        rotations = generate_rotations(p1, p2)
        # Only store one representative from the rotation set
        unique_sets.add(min(rotations))

    return sorted(unique_sets)

def print_grid(p1, p2, grid_size=5):
    """Print the grid with positions A and B marked."""
    grid = [['.' for _ in range(grid_size)] for _ in range(grid_size)]
    grid[p1[1]][p1[0]] = 'A'  # Note: row is y, column is x
    grid[p2[1]][p2[0]] = 'B'
    
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
