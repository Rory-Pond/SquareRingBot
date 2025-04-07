0	Rotate	0	0
1	Move_Forwards	1	2
2	Move_Forwards-Right	2	3
3	Move_Right	3	4
4	Move_Backwards-Right	4	5
5	Move_Backwards	5	6
6	Move_Backwards-Left	6	7
7	Move_Left	7	8
8	Move_Forwards-Left	8	1
9	Attack_Forwards	1	2
10	Attack_Left	8,1	1,2
11	Attack_Right	1,2	2,3
12	Attack_All	8,1,2	1,2,3
13	Attack_Far-Left	10,11	12,13
14	Attack_Far-Right	9,10	11,12
15	Attack_Far-All	9,10,11	11,12,13
16	Attack_Double-forwards	1,10	2,12
17	Attack_Double-Left	2,12	3,14

int function(int value) {
    if (value == 0)
        return 0;                
    if (value <= 8)
        return (value % 8) + 1;  
    else
        return value + 2;        
}

direction/2 ==> movement choice
direction%2 ==> isDiagonl?

+-----------+
| . . . . . |
| . . . A . |
| . . . . . |
| . B . . . |
| . . . . . |
+-----------+
