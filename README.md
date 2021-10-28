# c-vectors
Custom vector implementation in c

This is a very (terrible) implementation of vectors and matrices in c. I will expand upon this repository as my skills get better - TODO'S include: 

* Hardware and memory optimizations.
* Eliminate all possible sources of memory leaks.
* Computational tricks and approximations.
* Support for more operations (curl, div etc.) 
* Flexibility (function overloading, generics, etc.)

Matrix example:
`
    mat* nmat = init_mat(5, 5); // declare matrix
    setm(nmat, 3, 1, 1); // set matrix entry (3,1) to 1
    resizem(nmat, 4, 4); // resize the matrix to size (4,4) from (5,5)
    to_string(nmat); // print the matrix
    delm(nmat); // delete the matrix
`

Vector example:
`
    vec* nvec = init_vec(3); // declare vector
    setv(nvec, 0, 1); // set vectory entry (0) to 1
    resizev(nvec, 2); // resize the vector to size (2) from (3)
    to_string(nvec); // print the vector
    delv(nvec); // delete the vector
`   
We can also use the auxillay functions for the vector and matrix class respectively to do some maths:
`

    vec* NVEC = init_vec(3);
    vec* NVEC2 = init_vec(3);
    vec* NVEC3 = init_vec(3);
    setv(NVEC, 0, 1);
    setv(NVEC, 1, 1);
    setv(NVEC, 2, 1);
    setv(NVEC2, 0, 1);
    setv(NVEC2, 1, 2);
    setv(NVEC2, 2, 3);
    setv(NVEC3, 0, 10);
    setv(NVEC3, 1, 5);
    setv(NVEC3, 2, 88);
    
    to_string(prod(3, NVEC, NVEC2, NVEC3));
    to_string(sum(3, NVEC, NVEC2, NVEC3));
    to_string(scale(NVEC3, 5));
    to_string(cross(NVEC2, NVEC3));
    printf("%f\n", *dot(2, NVEC, NVEC2, NVEC3));

`


