### USAGE OF COLORS IN NGPC

There is a maximum of 3 colors per Tile. The 4th color is represented as 0, and the framework does not evaluate it, treating it as direct transparency.

#### FORMAT

```
0x0000 = 0x[SUFFIX]
```

##### SUFFIX

Each value of the suffix represents two cells, resulting in 8 pixels for 4 suffix values multiplied by 2. Each position represents an odd or even value, depending on the sum of the cells it represents, resulting in a different value.

```
Color 1 = Odd 4, Even 1
Color 2 = Odd 8, Even 2
Color 3 = Odd c, Even 3
```

#### EXPLANATION

- 1st digit = Cells 1 and 2
- 2nd digit = Cells 3 and 4
- 3rd digit = Cells 5 and 6
- 4th digit = Cells 7 and 8
