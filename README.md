IRG
==

Interaktivna racunalna grafika - laboratorijske vjezbe

## 1. laboratorijska vjezba

### 1. zadatak

Napraviti biblioteku koja omogucava rad s vektorima i matricama kao potpora za
nadolazece laboratorijske vjezbe.
* IVector - sucelje prema AbstractVector
* AbstractVector - apstraktni razred vektora kojeg implementira razred Vector
* Vector - konkretni razred vektora

* IMatrix - sucelje prema MatrixTransposeView, MatrixSubMatrixView i
AbstractMatrix razredima
* MatrixTransposeView - transponirama matrica
* MatrixSubMatrixView - podmatrica
* AbstractMatrix - apstraktni razred matrice kojeg implementira razred Matrix
* Matrix - konrektni razred matrice

* MatrixVectorView - pogled na vektor preko matrice
* VectorMatrixView - pogled na matricu preko vektora

### 2. zadatak

Crtanje trokuta kojemu mozemo birati boju koristeci GLUT

### 3. zadatak

Crtanje linije pomocu Bresenhamovog algoritma te prikaz odsjecenih linija pomocu
Cohen Sutherlandovog algoritma
