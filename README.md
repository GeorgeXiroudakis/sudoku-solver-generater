# sudoku-solver-generater
fourth semester assignment

## use

basicaly the program has 3 modes.

### 1)
runing ./sudoku by it self it reads a puzzle from the stdin (in the same format as the excamples given)
prints the puzzle it was given in stdout
prints a message for if the puzzle has one and only solution (is unique), if it has multiple solution, or if it has no solution.
finally if it has at least one solution it prints the solved puzzle, else it prints one of the failed attemts of solving it.

### 2)
with the c flag so ./sudoku -c you get the ckecking mode.
Again it reads a *solved* puzzle from the stdin (in the same format as the excamples given) and
prints the puzzle it was given in stdout
then it ckecks the puzzles and if it's correct it just prints a message saying it is and if it's not correctly solved
it prints a message that it is not correct and all the errors it foud.

## 3)
lastly with the -g flag you get generate mode where you can greate unsolved sudkoku puzzles after the g flag 
you specify the number of filled numbers want to have (ex ./sudoku -g 70 will generate a puzzles with 70 out of the 81 numbers filled).
you can also specify if you want the puzzles generated to be unique by adding the u flag (ex ./sudoku -g 70 -u).
