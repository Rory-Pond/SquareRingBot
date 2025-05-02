# Dev Notes

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

# Direction Symbols
↖ ↑ ↗
← · →
↙ ↓ ↘

⬁ ⇧ ⬀
⇦ · ⇨
⬃ ⇩ ⬂

↖ ↗ ↘ ↙← → ↑ ↓
⬉ ⬈ ⬊ ⬋⬅➡ ⬆ ⬇

// 0: ↑
// 1: ↗
// 2: →
// 3: ↘
// 4: ↓
// 5: ↙
// 6: ←
// 7: ↖

# Character Notes

## Oscar
Rotate_90
Move_Forwards_Rotate
Move_Right
Move_Backwards
Move_Left
Move_Forwards_Left

Cannonita 
	- Attack_Double_Forwards
	- 5 Damage
	- Round 2 -1
	- Round 3 -3

Cajones
	- 0 Damage
	- opponent deals +2 Damage
	- rotate 45
	- if you take damage free burst next turn

Left Haymaker
	- 4 Damage
	- Attack_Left
	- Round 2 -2
	- Round 3 -4
	- 

El Nuke
	- 8 Damage 
	- Round 2 -2
	- Round 3 -4
	- Attack_Right
	- if miss move forwards 1

## Fredo
Rotate_90
Move_Forwards_Rotate
Move_Forwards_Right_Rotate
Move_Right
Move_Backwards
Move_Left
Move_Forwards_Left_Rotate

Left Hook
	- 3 damage
	- attack left
	- +1 if last attack was grill guard
Right Hook
	- 3 damage
	- attack left
	- +1 if last attack was grill guard

Grill Guard
	- -2 damage reduction
	- prevent daze
	- rotate

Uppercut
	- 5 damage
	- daze
	- +3 if directly after grill guard


## Inga
Rotate_90
Move_Forwards
Move_Right
Move_Backwards_Right_Rotate
Move_Backwards
Move_Left
Move_Forwards_Left_Rotate

Left Jab
	- far left attack
	- 1 damage
	- attack move left, up_left_Rotate, down_left_Rotate

Right Hook
	- attack_right, 
	- 2 damage
	- attack move right

Svallin
	- -3 damage reduction
	- cannot be dazed

Mjolnir
	- attack double forwards
	- 4 damage
	- Round 2 +1
	- Round 3 +2
	- Daze
	- cooldown

## Andrey
Rotate_90
Move_Forwards
Move_Right_Rotate
Move_Backwards_Right
Move_Backwards
Move_Left_Rotate
Move_Forwards_Left

Collider
	- attack double forwards
	- attack move: right_Rotate
	- 2 damage
	- +2 if last action was quantum jab
  
Quantum Jab
	- attack all far
	- 1 damage
	- counter
	- attack move left_Rotate right_Rotate

Paradox
	- swap for collider or left_Rotate

Entangle
	- attack forwards
	- if lands no damage dealt by either character till movement

## Ellie
	- Max 2 Burst, Max 
Rotate_90
Move_Forwards_Rotate
Move_Forwards_Right_Rotate
Move_Right_Rotate
Move_Backwards_Right_Rotate
Move_Backwards_Rotate
Move_Backwards_Left_Rotate
Move_Left_Rotate
Move_Forwards_Left_Rotate

Left Jab
	- far left attack
	- 1 damage
	- attack move every direction with rotation

Godiva Strafe
	- double left attack
	- 2 damage
	- attack move up left
	- counter if react count == 2

Groove
	- if you don't get hit +1 burst or react
	- double damage multiplier from opponent if hit

Kook Hook
	- attack left
	- 3 damage
	- attack move left
	- +1 damage if you have 2 bursts

## Awan
Rotate_90
Move_Forwards
Move_Right_Rotate
Move_Backwards_Right
Move_Backwards
Move_Left
Move_Forwards_Left_Rotate

Bolo Punch
	- 4 damage
	- attack forwards
	- daze
	- +1 damage and counter if in exchange 5/6

Flurry
	- attack all
	- 1 damage
	- attack move right_Rotate
	- +1 damage and counter if in exchange 5/6

Left Jab
	- attack far left
	- 1 damage
	- attack move up_left_Rotate, right_Rotate
	- +1 damage and counter if in exchange 5/6

Rush Guard
	- -1 damage
	- Cannot be dazed
	- attack move forawards_Rotate
	- cooldown


## Roderik
Rotate_90
Move_Forwards
Move_Right
Move_Backwards_Right
Move_Backwards_Rotate
Move_Backwards_Left
Move_Left_Rotate

Check Hook
	- attack_left
	- 1 damage
	- +2 damage if counter
	- Attack move left
	- cooldown attack

Cobra Cross
	- attack double forwards
	- 1 damage
	- +1 damage if counter
	- counter
	- cooldown attack

Left Jab
	- attack far left
	- counter
	- 1 damage
	- attack move: left_Rotate, right, back_Rotate

Left Jab2
	- attack far left
	- counter
	- 1 damage
	- attack move: left_Rotate, right, back_Rotate

Coil Up
	- prevent all damage
	- cool down
	- if an attack would of landed then +2 damage and daze next attack


## Nigel
Rotate_90
Move_Forwards_Rotate_Damage_Daze
Move_Forwards_Right
Move_Right
Move_Backwards_Rotate_Counter
Move_Backwards_Left_Rotate
Move_Left_Rotate

Straight
	- attack double forwards
	- 3 damage
	- attack move: forwards backwards
Wallop
	- attack left
	- 3 damage
	- if hits opponent moves backwards 1
	- if hits cooldown 
Sway
	- cooldown
	- dodge
	- next action must be attack or backwards_Rotate

Right Jolt
	- attack far right 
	- damage 2
	- attack move up left