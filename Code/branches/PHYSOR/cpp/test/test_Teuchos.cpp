#include "Teuchos_LAPACK.hpp"
#include "Teuchos_SerialDenseMatrix.hpp"
#include "Teuchos_SerialDenseVector.hpp"
#include "Teuchos_Version.hpp"
#include <cstdlib>

void calcEigenPairs(int);

typedef  Teuchos::SerialDenseMatrix<int, double> SDM;

void subMatrix(SDM&, int);

int main(int argc, char* argv[])
{
    std::cout << "argc: " << argc << ", argv: \n";
    for( int i  = 0; i < argc; ++i ){
        std::cout << "\t" << i << ", " << argv[i] << "\n";
    }
    int size = std::atoi(argv[1]); //Size of matrix
    std::cout << "size = " << size << "\n";


    std::cout << Teuchos::Teuchos_Version() << std::endl << std::endl;

    // Creating an instance of the LAPACK class for double-precision routines looks like:
    Teuchos::LAPACK<int, double> lapack;

    // This instance provides the access to all the LAPACK routines.
    Teuchos::SerialDenseMatrix<int, double> My_Matrix(size,size);
    Teuchos::SerialDenseVector<int, double> My_Vector(size);
    
    My_Matrix.random();
    My_Vector.random();

    // Perform an LU factorization of this matrix. 
    int ipiv[size], info;
    char TRANS = 'N';
    lapack.GETRF( size, size, My_Matrix.values(), My_Matrix.stride(), ipiv, &info ); 

    // Solve the linear system.
    lapack.GETRS( TRANS, size, 1, My_Matrix.values(), My_Matrix.stride(), 
        ipiv, My_Vector.values(), My_Vector.stride(), &info );  

    // Print out the solution.
    std::cout << My_Vector << std::endl;

    calcEigenPairs(size);
    return 0;
}

void calcEigenPairs(int N) {
    Teuchos::LAPACK<int, double> lapack;

    Teuchos::SerialDenseMatrix<int, double> A(N,N);
    Teuchos::SerialDenseVector<int, double> EigValuesReal(N);
    Teuchos::SerialDenseVector<int, double> EigValuesImag(N);
    Teuchos::SerialDenseMatrix<int, double> EigVectors(N,N);
    Teuchos::SerialDenseVector<int, double> Work(4*N);

    std::cout << "Before setting matrix elements: \n" << A << "\n\n";
    for( int i = 0; i < N; ++i ){
        A(i,i) = i + 1;
    }
    A(2,1) = 1.0;
    std::cout << "After setting matrix elements: \n" << A << "\n\n";

    std::cout << "-----------------------------------------\n";
    for( int n = 1; n < 4; ++n) subMatrix(A, n);
    std::cout << "-----------------------------------------\n";

    std::cout << "\nGetting my eigenpairs with GEEV...\n";

    int info;               //Exit flag
    
    lapack.GEEV(
 /* IN */   'N',            /*Left eigenvectors ?
                              'N' --- do not calculate
                              'V' --- calculate*/
 /* IN */   'V',            /*Right eigenvectors ?
                              'N' --- do not calculate
                              'V' --- calculate*/
 /* IN */   N,              /*Order of matrix A. 
                              A.shape must be (NxN)*/
 /* IN */   A.values(),     /*Matrix
                              Must be 1D contiguous array in memory.
   OUT                        WARNING: Overwritten.*/
 /* IN */   A.stride(),     /*Leading dimension of A.
                              stride or number of columns of A.*/
 /* OUT*/   EigValuesReal.values(),  /*Real part of eigenvalues*/
 /* OUT*/   EigValuesImag.values(),  /*Imaginary part of eigenvalues*/
 /* OUT*/   EigVectors.values(),     /*Left eigenvectors (if computed)
                              not referenced if LDVL = 'N'*/
 /* IN */   1,              /*Leading dimension of Left eigenvectors
                              >=1,
                              >=N, if LDVL = 'V'.*/
 /* OUT*/   EigVectors.values(),     /*Right eigenvectors (if computed)
                              not referenced if LDVR = 'N'*/
 /* IN */   N,              /*Leading dimension of Right eigenvectors
                              >=1,
                              >=N, if LDVR = 'V'.*/
 /* OUT*/   Work.values(),  /*Not sure what this does.*/
 /* IN */   4*N,            /*Not sure what this does, complements Work.*/
 /* OUT*/   &info           /*Status of computation*/
            );

    std::cout << "The 'messed up' matrix A now looks like \n" << A << "\n";
    std::cout << "Real part of eigenvalues:\n" << EigValuesReal << "\n";
    std::cout << "Imaginary part of eigenvalues:\n" << EigValuesImag << "\n";
    std::cout << "Eigenvectors:\n" << EigVectors << "\n";
    std::cout << "Now this funny work thing:\n" << Work << "\n";
}

void subMatrix(SDM& A, int n){
    Teuchos::LAPACK<int, double> lapack;
    Teuchos::SerialDenseVector<int, double> EigValuesReal(n);
    Teuchos::SerialDenseVector<int, double> EigValuesImag(n);
    Teuchos::SerialDenseMatrix<int, double> EigVectors(n,n);
    Teuchos::SerialDenseVector<int, double> Work(4*n);
    int info;               //Exit flag

    lapack.GEEV(
 /* IN */   'N',            /*Left eigenvectors ?
                              'N' --- do not calculate
                              'V' --- calculate*/
 /* IN */   'V',            /*Right eigenvectors ?
                              'N' --- do not calculate
                              'V' --- calculate*/
 /* IN */   n,              /*Order of matrix A. 
                              A.shape must be (NxN)*/
 /* IN */   A.values(),     /*Matrix
                              Must be 1D contiguous array in memory.
   OUT                        WARNING: Overwritten.*/
 /* IN */   A.stride(),     /*Leading dimension of A.
                              stride or number of columns of A.*/
 /* OUT*/   EigValuesReal.values(),  /*Real part of eigenvalues*/
 /* OUT*/   EigValuesImag.values(),  /*Imaginary part of eigenvalues*/
 /* OUT*/   EigVectors.values(),     /*Left eigenvectors (if computed)
                              not referenced if LDVL = 'N'*/
 /* IN */   1,              /*Leading dimension of Left eigenvectors
                              >=1,
                              >=N, if LDVL = 'V'.*/
 /* OUT*/   EigVectors.values(),     /*Right eigenvectors (if computed)
                              not referenced if LDVR = 'N'*/
 /* IN */   n,              /*Leading dimension of Right eigenvectors
                              >=1,
                              >=N, if LDVR = 'V'.*/
 /* OUT*/   Work.values(),  /*Not sure what this does.*/
 /* IN */   4*n,            /*Not sure what this does, complements Work.*/
 /* OUT*/   &info           /*Status of computation*/
            );
    std::cout << "Real part of eigenvalues:\n" << EigValuesReal << "\n";
    std::cout << "Eigenvectors:\n" << EigVectors << "\n";
}
