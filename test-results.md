# Performance Results

Performance can be measured by compiling with ```-pg```, and opening the executable with ```gprof silo.exe```

Seconds for first 100000 cycles

It should be noted this is CPU time, not wall time.

## OLD ALG

```txt
  %   cumulative   self              self     total
 time   seconds   seconds    calls  ms/call  ms/call  name
 97.86    221.47   221.47   100000     2.21     2.21  solve
  1.69    225.30     3.83 118752000     0.00     0.00  addToCLList
  0.28    225.93     0.63                             _mcount_private
  0.09    226.13     0.20                             __fentry__
  0.08    226.31     0.18                             main
  0.00    226.32     0.01   100000     0.00     0.00  initCLList
  0.00    226.32     0.00   100000     0.00     0.00  checkParticles
  0.00    226.32     0.00     2000     0.00     0.00  addParticle
  0.00    226.32     0.00      100     0.00     0.00  getFilename
  0.00    226.32     0.00      100     0.00     0.00  plot
  0.00    226.32     0.00      100     0.00     0.00  showInfo
  0.00    226.32     0.00        1     0.00     0.00  readInput
```

## NEW ALG NO OMP

```txt
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 91.68     61.39    61.39   100000   613.90   613.90  calcInteractionCL
  5.42     65.02     3.63 118752000     0.03     0.03  addToCLList
  1.12     65.77     0.75                             _mcount_private
  1.03     66.46     0.69   100000     6.90   620.80  solve
  0.30     66.66     0.20                             main
  0.24     66.82     0.16                             __fentry__
  0.21     66.96     0.14   100000     1.40     1.40  checkParticles
  0.00     66.96     0.00   100000     0.00     0.00  initCLList
  0.00     66.96     0.00     2000     0.00     0.00  addParticle
  0.00     66.96     0.00      100     0.00     0.00  getFilename
  0.00     66.96     0.00      100     0.00     0.00  plot
  0.00     66.96     0.00      100     0.00     0.00  showInfo
  0.00     66.96     0.00        1     0.00     0.00  readInput
```

## NEW ALP OMP 4 THREADS

```txt
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 90.89     45.90    45.90                             __gcc_deregister_frame
  6.22     49.04     3.14 118752000     0.03     0.03  addToCLList
  1.23     49.66     0.62                             _mcount_private
  0.93     50.13     0.47   100000     4.70     4.70  solve
  0.40     50.33     0.20                             __fentry__
  0.30     50.48     0.15                             main
  0.04     50.50     0.02   100000     0.20     0.20  checkParticles
  0.00     50.50     0.00   100000     0.00     0.00  initCLList
  0.00     50.50     0.00     2000     0.00     0.00  addParticle
  0.00     50.50     0.00      100     0.00     0.00  getFilename
  0.00     50.50     0.00      100     0.00     0.00  plot
  0.00     50.50     0.00      100     0.00     0.00  showInfo
  0.00     50.50     0.00        1     0.00     0.00  readInput
```

## NEW ALP OMP 6 THREADS

```txt
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 86.77     35.36    35.36                             __gcc_deregister_frame
  8.69     38.90     3.54 118752000     0.03     0.03  addToCLList
  1.77     39.62     0.72                             _mcount_private
  1.35     40.17     0.55   100000     5.50     5.50  solve
  0.96     40.56     0.39                             main
  0.37     40.71     0.15                             __fentry__
  0.10     40.75     0.04   100000     0.40     0.40  checkParticles
  0.00     40.75     0.00   100000     0.00     0.00  initCLList
  0.00     40.75     0.00     2000     0.00     0.00  addParticle
  0.00     40.75     0.00      100     0.00     0.00  getFilename
  0.00     40.75     0.00      100     0.00     0.00  plot
  0.00     40.75     0.00      100     0.00     0.00  showInfo
  0.00     40.75     0.00        1     0.00     0.00  readInput
```

## NEW ALP OMP 6 THREADS - LINKED LIST WITH TAIL

```txt
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 96.00     35.48    35.48                             __gcc_deregister_frame
  1.43     36.01     0.53   100000     5.30     5.30  solve
  1.33     36.50     0.49 118752000     0.00     0.00  addToCLList
  0.68     36.75     0.25                             _mcount_private
  0.22     36.83     0.08                             __fentry__
  0.22     36.91     0.08                             main
  0.14     36.96     0.05   100000     0.50     0.50  checkParticles
  0.00     36.96     0.00   100000     0.00     0.00  initCLList
  0.00     36.96     0.00     2000     0.00     0.00  addParticle
  0.00     36.96     0.00      100     0.00     0.00  getFilename
  0.00     36.96     0.00      100     0.00     0.00  plot
  0.00     36.96     0.00      100     0.00     0.00  showInfo
  0.00     36.96     0.00        1     0.00     0.00  readInput
```

## NEW ALG OMP 8 THREADS

```txt
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 86.65     37.27    37.27                             __gcc_deregister_frame
  8.67     41.00     3.73 118752000     0.03     0.03  addToCLList
  2.35     42.01     1.01                             _mcount_private
  1.23     42.54     0.53   100000     5.30     5.30  solve
  0.58     42.79     0.25                             __fentry__
  0.42     42.97     0.18                             main
  0.09     43.01     0.04   100000     0.40     0.40  checkParticles
  0.00     43.01     0.00   100000     0.00     0.00  initCLList
  0.00     43.01     0.00     2000     0.00     0.00  addParticle
  0.00     43.01     0.00      100     0.00     0.00  getFilename
  0.00     43.01     0.00      100     0.00     0.00  plot
  0.00     43.01     0.00      100     0.00     0.00  showInfo
  0.00     43.01     0.00        1     0.00     0.00  readInput
```

## OBSERVATIONS

- The new algorithm is faster than the old algorithm
- The new algorithm is fastest with 6 threads due to the overhead of threads for this number of particles and iterations
- While using OMP ```addToCLList``` is the most time consuming function (it is called a lot)
- Using the linked list with tail is faster than using the linked list without tail
