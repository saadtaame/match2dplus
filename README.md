# match2dplus
*match2dplus* is a generalization of *match2d* to allow for arbitrary ASCII characters. The program locates occurrences of square matrices in larger square matrices. The program reads two integers n and m (m <= n), the dimensions of the matrices, then it reads n lines each containing a string of characters of length n, then it reads m lines each containing a string of characters of length m. The program outputs coordinates where the mxm matrix occurs as a sub-matrix of the nxn matrix.

Sample input/output:

input:
<pre>
5 2
aACDe
FDhiJ
KlmnO
pqRDh
uvWlm
Dh
lm
</pre>

output
<pre>
match at (1,1)
match at (3,3)
</pre>
