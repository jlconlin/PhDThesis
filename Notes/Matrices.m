(* ::Package:: *)

(* :Title: Matrices *)

(* :Name: Matrices *)

(* :Author: Jeremy L. Conlin *)

(* :Context: Matricdes` *)

(* :Summary:
	This package was created to have a centralized location for all my 
	special matrix definitions. *)


BeginPackage["Matrices`"];


UHessenberg::usage = "UHessenberg[n] will create a square, n by n, upper 
Hessenberg matrix with increasing elements."


Begin["Private`"]


UHessenberg[n_] := Module[{H},
(* UHessenberg makes a square upper Hessenberg matrix with increasing 
	elements. *)
k=1.0;
H = Table[If[i<=j+1,k++,0],{i,n},{j,n}];
Return[H];
]


F[n_]:=Module[{A},
(* This creates a sqaure matrix whose elements are linearly increasing.   
	All elements are non-zero.*)
k = 1.0;
A = Table[k++,{i,n},{j,n}];
Return[A];
]


(* Standard creates a square matrix that is diagonal with linearly increasing 
	elements---except for the [[3,2]] entry which is 1.*)
Standard[n_] := Module[{S},
S = DiagonalMatrix[Range[1.0,n]];
S[[3,2]] = 1.0;
Return[S];
]


(* Simple matrix from Fundamentals of Matrix Computatations, by David S. Watkins
	pg. 357.*)
Watkins[] := Module[{A},
	A = {{8.,2.},{2.,5.}};
	Return[A];
]


End[]; (* End Private` context. *)


EndPackage[];
