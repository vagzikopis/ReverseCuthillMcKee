# ReverseCuthillMcKee

## Implemented for course 'Parallel & Distributed Systems' of Aristotle University of Thessaloniki

Authors: Gonidelis Ioannis & Zikopis Evangelos

## Description

Sparse graph matrix reordering using ReverseCuthillMcKee algorithm and OpenMP library.

## How to execute code

```bash
cd ReverseCuthillMckee
make
cd bin
./sequential or ./parallel
```

### Optional Functions

Our implementation offers the above optional functionalities:

- If you want to calculate the initial and final bandwidhts uncomment [these](https://github.com/vagzikopis/ReverseCuthillMcKee/blob/684cdf611732cd4f8d94b2bd009331eee37d8fd2/src/main.c#L21-L22) and [these](https://github.com/vagzikopis/ReverseCuthillMcKee/blob/684cdf611732cd4f8d94b2bd009331eee37d8fd2/src/main.c#L72-L73) lines.
- If you want to save the input sparse matrix uncomment [these](https://github.com/vagzikopis/ReverseCuthillMcKee/blob/684cdf611732cd4f8d94b2bd009331eee37d8fd2/src/main.c#L29-L30) lines.
- If you want to generate and save the output sparse matrix uncomment [these](https://github.com/vagzikopis/ReverseCuthillMcKee/blob/684cdf611732cd4f8d94b2bd009331eee37d8fd2/src/main.c#L54-L75) lines. This step could be time consuming for big matrices.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
