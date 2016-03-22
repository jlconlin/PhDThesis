(* ::Package:: *)

(* :Title: Implicitly Restarted Arnoldi's Method *)

(* :Name: IRAM` *)

(* :Author: Jeremy L. Conlin *)

(* :Context: IRAM` *)

(* :Summary:
	This package was created as a prototype and comparision of a 
	similar code written in C++.  It is an implementation of 
	Implicitly Restarted Arnoldi's Method.
*)

BeginPackage["IRAM`"];

IRAM::usage = "IRAM[A, q, jI, kI, r] will perform IRAM on the matrix A with
starting vector q, for jI outer iterations and kI inner iterations\[Dash]or QR
iterations.  r is the number of restarts."

Begin["`Private`"]  (* Begin private context (implementation) *)

IRAM[M_,s_,jj_,kk_, rr_]:=Module[{},
	A = M;
	q = s;
	jI = jj; kI = kk; tI = jI + kI;
	Print["jI = ", jI, ", kI = ", kI, ", tI = ", tI];
	L = Length[q];
	Q = Table[Complex[0.0,0.],{i,L},{j,tI+1}];
	H = Table[Complex[0.0,0.0],{i,tI+1},{j,tI}];

	q = q/Norm[q,2];
	Q[[All, 1]] = q;
	Print["Q start", Q//MatrixForm];

	k = 0;
	For[i=0, i<kI, i++;k++,
		Iteration[];
	];(*End For loop*)

	For[R = 0, R < rr, R++, (* Restarts *)
		Restart[];
		If[ Abs[Re[ H[[k+1,k]] ] ] < 10^-10, 
			Print["Finishing, ", H[[k+1,k]] ];
			Break[];,
			Print["Continuing, ", H[[k+1,k]] ];
		];
		k = jI;		
	]; (* End restart For loop*)

	Return[{Q,H}];
]

Restart[] := Module[{},
	For[i=0, i < jI, i++; k++,
		Iteration[];
	]; (* End \.08inner iteration For loop *)
	
	Print["H before QR: ", H//MatrixForm];
	Print["Q before QR: ", Q//MatrixForm];

	(* Shifted QR *)
	Hs = H[[;;k,;;k]];
	SavedHs = Hs;
	Values = Eigenvalues[ Hs ];
	Shifts = Values[[-kI;;]];
	(*Shifts = {0,0,0,0};*)
	(*Print["Shifts: ", Shifts];*)
	n = Length[Hs];
	V = IdentityMatrix[n];
	Id = IdentityMatrix[n];
	For[i = 0, i < kI, i++,
		shift = Shifts[[-(i+1)]];
		(*Print["shift = ", shift];*)
		Hs = Hs - shift*Id;
		Print["Hs: ", Hs//MatrixForm];
		{v,r} = QRDecomposition[Hs];
		Hs = r.ConjugateTranspose[v] + shift*Id;
		V = V.ConjugateTranspose[v];
		(*Print["i = ", i, ", v = ", v//MatrixForm ];
		Print["Hs: ", Hs//MatrixForm];*)
	]; (* End shift For loop *)

	(* Update H, Q, and k *)	
	Q[[All,;;kI]] = (Q[[All,;;k]].V)[[All,;;kI]];
	H[[;;kI,;;kI]] = Hs[[;;kI,;;kI]];
	(*Print["V: ", V//MatrixForm];
	Print["Q: ", Q//MatrixForm];
	Print["H: ", H//MatrixForm];*)
	Print["Restart #: ", R, ", Eigenvalue = ", Values[[1]]];
] (* End Restart *)

Iteration[] := Module[{},
	Print["    Iteration #: ", k];
	q = Q[[All,k+1]];
	(*Print["q: ", q];*)
	q = A.q;
	Orthogonalization[];
	Normalization[];
	Q[[All, k+2]] = q;	
] (*End of Iteration*)

Orthogonalization[] := Module[{},
	For[j = 0, j <= k, j++,
		qj = Q[[All, j+1]];
		OF = q.qj;
		H[[j+1,k+1]] = OF;
		q = q-OF*qj;
	]; (* End of For loop *)
] (* End of Orthogonalization *)

Normalization[] := Module[{},
	NF = Norm[q,2];
	H[[k+2,k+1]] = NF;
	q = q/NF;
] (* End of Normalization *)



End[]  (* End the private contxt. *)

EndPackage[];



