## Solve system of equations:
<img src="https://render.githubusercontent.com/render/math?math=Au = f; \\ A = a_{ij} = \begin{cases} 1 , \ \ \ \ \ \ i = j,\\\frac{1}{i^2 + j}, \ i \neq j;\end{cases}\\ f_i = 1/i \\ \ \ \ \ \ i,j = 1 .. 12; \end{comment}">

### Gauss elimination:
$$
u = 
\begin{bmatrix}
0.75537
\\
0.2388
\\
0.178903
\\
0.15087
\\
0.131709
\\
0.117033
\\
0.105271
\\
0.0956054
\\
0.0875217
\\
0.0806659
\\
0.074783
\\
0.0696832
\end{bmatrix};
\\
residual = ||Au - f|| = 1.11022e-16;
$$

### Simple iterations:
$$
u = 
\begin{bmatrix}
0.75537
\\
0.2388
\\
0.178903
\\
0.15087
\\
0.131709
\\
0.117033
\\
0.105271
\\
0.0956054
\\
0.0875217
\\
0.0806659
\\
0.074783
\\
0.0696832
\end{bmatrix};
\\
residual = ||Au - f|| = 5.55112e-17;
\\
iterations = 117;
$$
