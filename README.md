# HMM Simulation (School Project Implemented in C)
Project in "Algorithm in molecular biology"

## Information:
The Dishonest Casino. A casino has two dices:
- Fair dice
  - P(1) = P(2) = P(3) = P(4) = P(5) = P(6) = 1/6
- Loaded dice
  - P(1) = P(2) = P(3) = P(4) = P(5) = 0.1
  - P(6) = 0.5
  
Casino player switches back & forth between fair and loaded die with the following probability :

![](https://github.com/bencoz/HMM-Simulation/blob/master/hmm_pic_1.jpg)
> state diagram.

### 3 C Programs :
- simulate.c : Simulate a casino day (300) tosses and produce 2 txt files :
  - TOSS_OUT.txt -  the tosses result in that day (i.e 66234666324515666..)
  - CUBES_OUT.txt - the cube use in that toss (F/U)
- hmm.c : Implemention of the foward-backward algorithm. Recives TOSS_OUT.txt and give a state probability (F/U) for each toss. Produces :
  - HMM_OUT.txt - A guess for each toss i.e in which state it was.
- analysis.c - Recives 2 files (CUBES_OUT.txt & HMM_OUT.txt) and analyses them. Produce 2 txt files :
  - COMPARE.txt - 3 rows file :
    1. is the (real) CUBES_OUT.txt
    2. is '+' if the algorithm was right. ' ' Otherwise. 
    3. is the HMM_OUT.txt
  - SUMMARY.txt - Presenting the sensitivity and specificity of the algorithm

## Made by Ben Cohen
