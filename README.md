# ITSC 3145 Final Group Project

## Description

This repository contains the final code for our group project in ITSC 3145. We have implemented solutions for two separate problems using C programming language.

### Problem 1: MPI Program for Tree-Structured Global Sum

- **File:** `Problem-2.c`
- **Purpose:** Compute a tree-structured global sum using MPI. The program initially handles the case where the number of MPI processes (`comm_sz`) is a power of two and later extends to handle any number of processes. Each process computes a local sum, and then a tree reduction approach is used to calculate the global sum.

### Problem 2: Monte Carlo Estimation of Pi using OpenMP

- **File:** `Problem-1.c`
- **Purpose:** Estimate the value of pi using the Monte Carlo method, utilizing parallelization with OpenMP. Darts are thrown randomly at a square dartboard with an inscribed circle, and the ratio of darts landing inside the circle to the total darts thrown provides an estimate of pi.

