# sat-slover-zwang531
## DPLL 
  * Instruction: 
    * navigate to dpll folder
    * run make
    * run ./sat 'filename'
  * Results (timeout = 10sec): 
    * graph-ordering-#.cnf : got 'Unsatisfiable' for 2,3,4 and 'Timeout' for others
    * graph-ordering-alt-#.cnf : got 'Satisfiable' for 2,3,4 and 'Timeout' for others
    * order-principle-#.cnf : got 'Satisfiable' for 2,3,4 and 'Timeout' for others
    * total-order-alt-#.cnf : got 'Unsatisfiable' for 2,3,4 and 'Timeout' for others
    * total-order-#.cnf : got 'Unsatisfiable' for 2,3,4 and 'Timeout' for others
    
## DPLL-fast (improved DPLL with better structure)
  * Instruction: 
    * navigate to dpll-fast folder
    * run make and run ./test.sh
  * Results (timeout = 10sec): pre-run results are in output.txt

## Conflict analysis 
  * Instruction: 
    * navigate to conflict_analysis folder
    * run make and run ./test.sh
  * Results: pre-run results are in output.txt
    * output format:
      1. conflict @ level 'd'
      2. a clause of literals, each literal followed by its level in parentheses
      3. backtrack to level 'd'
    
## CDCL complete
  * Instruction:
    * navigate to cdcl folder
    * run make
    * run ./sat filename --verbose --time i
    * edit test.sh to run through cnf files in a specific folder with user-chosen mode
    * prerun results of ./petite in output.txt with both flags set and 10s timeout
    * use CRTL-C to interrupt the program
  * side talk:
    * tried 2 types of heuristics:
      1. pick decision literal based on their frequencies, the number of times they occur in the clauses
      2. sort the clauses with the number of literals of each, clauses with fewer number of literals get higher priority, and choose the decision literal in the highest priority clause
    * both worked poorly, so I decided to leave them out, and just pick an unassigned literal with respect to their ordering
