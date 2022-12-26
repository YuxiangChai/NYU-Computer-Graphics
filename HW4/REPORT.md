# Assignment 4 Report

## Environment

- MacOS Monterey 12.6
- Compiler: g++, clang version 11.0.0

## Part 1

### Implementation

- `Simulation.h`
    - change the number of point `noPt` to 10 to reduce the computation.
    - change the time step `timestep` to make the points move faster.
- `Simulation.cpp`
    - implement the Euler integration based on the equations.
    - implement the EulerCromer integration based on the equations.
    - implement the Verlet integration based on the equations.
        - first compute the old position at the initialization.
        - then after computing the new position and velocity, I need to reflex the old position based on the `worldColldConst` if the point collides with the walls.

### Usage

```bash
$ cd Part1
$ make clean; make
$ ./main
```

### Results

- All three integrations work fine.
- Euler will have small fluctuation on enery at the end. EulerCromer and Verlet will give stable energy at the end, but they both have negative potential energy, which might be caused by floating point errors.
    - Euler final energy: 30.64/30.65
    - EulerCromer final energy: 30.56
    - Verlet final energy: 30.56

## Part 2

### Implementation

- `Simulation.h`
    - First to apply gravity to every particle
    - Second, traverse the spring list and get the particles for each spring. Compute the spring force by using Hooke's law $F = k(L - l)$. The force direction can be computed by $p_1 - p_2$ and make it unit vector. Then multiply it by $F$ and convert it to negative if necessary.
    - To compute the damping force, I apply the equation $F = - (p_1 - p_2)\times$ `damping_constant` for $p_1$ and $F = - (p_2 - p_1)\times$ `damping_constant` for $p_2$.
- `Simulation.cpp`
    - Directly use the code in Part 1 for the three integration schemes
    - Use the same way as Part 1 to compute Kinetic Energy and Potential Energy
    - Compute the spring energy by applying the equation $E = 0.5\times k (L-l)^2$

### Usage

```bash
$ cd Part2
$ make clean; make
$ ./main
```

### Results

- Using the default settings, i.e. `spring_constant = 1000` and `damping_constant = 10`. Three schemes behave similarly and as in Part 1, Euler integration has the highest energy while EulerCromer and Verlet have similar energy.
- Change the settings to `spring_constant = 100`, the cube will be compressed when it hits the ground due to little force by spring, which equals the gravity of the top particles.
- Change the settings to `spring_constant = 5000`, the cube will bounce higher and deform less.
- Change the settings to `spring_constant = 10000`, the cube will deform in the air due to the large force by even a little spring deformation.
- Change the settings to `damping_constant = 1`, the cube will bounce and deform heavily due to little damping, which causes the spring to fluctuate heavily.
- Change the settings to `damping_constant = 100`, the system will explode because the damping force is too large for each particle. Screen Recorder can't catch the explode scene.