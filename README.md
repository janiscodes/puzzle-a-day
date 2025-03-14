# Puzzle-a-Day Solver

The **Puzzle-a-Day Solver** is a C-based program that solves the daily puzzle from DragonFjord. It uses two different algorithms: a recursive depth-first search with backtracking and an enhanced flood-fill pruning algorithm. The program compares the performance of both approaches and provides detailed analyses of runtime and recursion count.

## Features

- Two algorithms for solving the puzzle:
  - Standard backtracking depth-first search (DFS)
  - Backtracking with flood-fill pruning
- Analysis of computation time and recursion depth
- Solution calculation for specific days or date ranges within a month

## Usage

1. **Compile the program**:  
   ```sh
   make
   ```

2. **Run the program**:  
   ```sh
   make run
   ```
   The program will prompt you to enter a date in the format `DD.MM.` for which a solution should be computed. After input, two solution approaches will be executed:  
   - Standard DFS backtracking  
   - Optimized flood-fill pruning  

3. **Calculate solutions for a date range**:  
   Enter `-1` if you want to compute solutions for a date range. The program will then ask for the end date:  
   ```
   Please insert date you want to have a solution for (DD.MM.)
   -1
   Going from 1.1. to ...?
   ```
   You can now specify a range from January 1st to the desired date.

4. **Cleanup**:  
   To remove temporary files and clean up the program, run:  
   ```sh
   make clean
   ```

## License

This project is licensed under the MIT License.

## Authors

- Janis  
- Max  
- Nikita  
