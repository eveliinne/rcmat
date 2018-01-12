#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class IndexOutOfRange {};
class WrongDim {};


// po to jest klasa w klasie zeby byla mozliwosc uzywania referencji w programie
// struktura to klasa publiczna

class Matrix
{
    struct matrix;
    matrix *mat;

public:
    class Cref;    
    class Macierz;
    Matrix(fstream& f);
    Matrix(int r, int c, double val1, double val2 = 0);
    Matrix(const Matrix& x);
    ~Matrix();

    Matrix & operator= (const Matrix & m); // matrix=matrix
    Matrix operator*(const Matrix& x) const;
    double & operator() ( int row,  int col) const;
    friend ostream& operator<<(ostream&, const Matrix&);
    double read(int rowss, int colss) const;
    void write(double number, int rowss, int colss);
    Cref operator() (const int row, const int col);
};

struct Matrix::matrix
{
    double **data;
    int rows;
    int columns;
    int odwolanie;      //licznik odwolan
    
    // Konstruktor kopiuj¹cy jest u¿ywany do stworzenia nowego obiektu.
	// Jest prostszy od operatora przypisania - nie musi sprawdzaæ przypisania do samego siebie i zwalniaæ poprzedniej zawartoœci.

    matrix(int r, int c, double diag, double fill = 0) // konstruktor tak jakby wewnetrzny 
    {
        odwolanie= 1;
        rows = r;
        columns = c;

        // tworzenie dwuwymiarowej tablicy dynamicznej
        data = new double*[rows];
        for(int i = 0; i < rows; i++)
            data[i] = new double[columns];

        // wypelnianie calej tablicy wartosciami
        for(int i = 0; i < rows; i++)
        {
            for(int k = 0; k < columns; k++)
            {
                if(i == k)
                    data[i][k] = diag;
                else
                    data[i][k] = fill;
            }
        }
    };

    matrix(int r, int c, double** array) // konstruktor kopiujacy tez bardziej wewnetrzny
    {
        odwolanie= 1;
        rows = r;
        columns = c;

        // tworzenie dwuwymiarowej tablicy dynamicznej
        data = new double*[rows];
        for(int i = 0; i < rows; i++)
            data[i] = new double[columns];

        // wypelnianie calej tablicy wartosciami
        for(int i = 0; i < rows; i++)
        {
            for(int k = 0; k < columns; k++)
            {
                data[i][k] = array[i][k];
            }
        }
    };

    ~matrix() //destruktor struktury matrix
    {
        for(int i = 0; i < rows; i++)
            delete [] data[i];

        delete [] data;
    };

    matrix* detach() //detach sprawdza czy licznik odwolan jest rowny 1, jesli tak to  nie ma potrzeba alokowac nowej pamieci bo tylko ten obiekt korzysta z tego obszaru pamieci
    {
        if(odwolanie == 1)
            return this;
        matrix* t = new matrix(rows, columns, data); // tworzy nowy wskaznik, do ktorego przypisywany jest nowy obiekt-struktura
        odwolanie--; //zmniejsz licznik odwolan bo ten obiekt juz nie bedzie korzystac z tego obszaru pamieci 
        return t;
    };

   /* void assign(int r, int c,double **array) // mam jakas tablice, tworze nowy obiekt i wartosci z niej sa kopiowane do tego obiektu
    {
        if(rows != r || columns != c) // modyfikacja obiektu
        {
            // tworzenie dwuwymiarowej tablicy dynamicznej
            double** tmp = new double*[r];
            for(int i = 0; i < r; i++)
                tmp[i] = new double[c];

            // wypelnianie calej tablicy wartosciami
            for(int i = 0; i < r; i++)
            {
                for(int k = 0; k < c; k++)
                {
                    tmp[i][k] = array[i][k];
                }
            }

            delete [] data;
            data = tmp;
        } 
        else
            for(int i = 0; i < rows; i++)
            {
                for(int k = 0; k < columns; k++)
                {
                    data[i][k] = array[i][k];
                }
            }
    }*/
}; 

/*inline*/ void Matrix::write(double nr, int rows, int columns)
{cout << "void operator = (double number)"<<endl;
	mat = mat->detach(); // sa jakies wskazniki do obiektu i jesli modyfikujemy ktorys z nich, to tworzy sie nowy obiekt zeby nie modyfikowac tamtych pozostalych 
	mat->data[rows][columns] = nr;
}

/*inline*/ double Matrix::read(int rows, int columns) const // jest const, bo nic nie zmieniam, tylko czytam
{
	return mat->data[rows][columns]; //indeksy od 0
}

class Matrix::Cref // klasa Cref umozliwia rozroznianie czytania i pisania
//Przeci¹¿enie - istnienie wielu definicji tej samej nazwy
{
	friend class Matrix;
	Matrix& s;
	int rows;
	int col;
	Cref(Matrix& ss, int rowsx, int colx): s(ss), rows(rowsx), col(colx) {};

public:
	operator double() const // po prostu czyta
	{	
		cout << "operator double() const"<<endl;
		return s.read(rows, col); // 
	}
	Matrix::Cref& operator= (double number) // operator przypisania dwoch wartosci
	{
		cout << "void operator = (double number)"<<endl;
		s.write(number, rows, col);
		return *this;
	}		
/*	Matrix::Cref& operator= (const Cref& ref) // specjalny przypadek (cos z rzutowaniem)
	{
		return operator= ((double)ref);
	} */
};

Matrix & Matrix::operator=(const Matrix & m) // operator przypisania 
/* np. jak jest matrix B=A to uruchamia sie konstruktor kopiujacy, 
a jak jest np. matrix B;
			   B=A;
			   to nastepuje wtedy przypisanie */
{
  		m.mat->odwolanie++;
		cout << "m.data->rCount(=):" << m.mat->odwolanie << endl;
		cout << "data->rCount(=):" << mat->odwolanie-1 << endl;
  		if(--mat->odwolanie == 0)
			delete mat; 
		mat=m.mat;
	return *this;
}

//Wczytanie z pliku
// odwoluje sie do mojej glownej klasy::konstruktor matrix 
Matrix::Matrix(fstream& f)
{
    int r;
    int c;

    f >> r;
    f >> c;

    if(r <= 0 || c <= 0)
        throw IndexOutOfRange();	// Wyjatek
    // tworzenie dwuwymiarowej pomocniczej tablicy dynamicznej
    double **tmp;
    tmp = new double*[r];
        for(int i = 0; i < r; i++)
    	{
            	tmp[i] = new double[c];        	
    	}
    // wypelnienie pomocniczej tablicy wartosciami z pliku
    for(int i = 0; i < r; i++)
    {
        for(int k = 0; k < c; k++)
        {
            f >> tmp[i][k];
        }
    }
        mat = new matrix(r, c, tmp); // konstruktor tworzy nowy obiekt i przypisuje go do struktury mat / tworzy nowy obiekt za pomoca konst. kopiujacego
    for(int i = 0; i < r; i++)
        delete [] tmp[i];
    delete [] tmp;
}

Matrix::Matrix(int r, int c, double w1, double w2) // konstruktor (tworzy nowy obiekt), ten glowny
{
    mat = new matrix(r, c, w1, w2);
}

Matrix::Matrix(const Matrix& x) // nie tworze niczego nowego / obie macierze beda wskazywaly na to samo
// on tylko "kopiuje" (przypisanie wskaznika), wiec dane bylyby te same; zmienia sie tylko odwolanie no i jest przypisanie tego wskaznika wlasnie
{
    x.mat->odwolanie++;
    mat = x.mat; // przekazuje wskaznik na cala jego strukture
}

Matrix::~Matrix() // destruktor klasy
{
    if(--(mat->odwolanie) == 0)
        delete mat;
}


//operator *
Matrix Matrix::operator*(const Matrix& x) const
{
    if(mat->columns != x.mat->rows)	// Wyjatek ?????????????
        throw WrongDim();

    //czysta macierz z zerami
    Matrix tmp(mat->rows, x.mat->columns, 0, 0);

    for(int r = 0; r < mat->rows; r++)
    {
        for(int c = 0; c < x.mat->columns; c++)
        {
            for(int i = r; i < mat->columns; i++)
            {
                tmp.mat->data[r][c] += mat->data[r][i]* x.mat->data[i][c];
            }
        }
    }
    return tmp;
}

//operator ()
/*double& Matrix::operator() (int row, int col) const
{
    if ( (row < 0 || row > mat->rows) || (col < 0 || col > mat->columns) )
	throw IndexOutOfRange();
    return mat->data[row][col];
} */

Matrix::Cref Matrix::operator()(int rowss, int colss) // to dziala przy kazdym zapisywaniu i odczytywaniu - zostaje wywolane
{
//	cout << 
    if ( (rowss < 0 || rowss > mat->rows) || (colss < 0 || colss > mat->columns) )
	throw IndexOutOfRange();
    return Cref(*this, rowss, colss);
}

//operator <<
ostream& operator<<(ostream& out, const Matrix& m)
{
    for(int i = 0; i < m.mat->rows; i++)
    {
        for(int k = 0; k < m.mat->columns; k++)
        {
            out << setw(5) << m.mat->data[i][k] << " ";
        }
        out << endl;
    }
    return out;
}

int main()
{
    try
    {
        cout << endl;

	  	Matrix A1(2,5,1.0);			//Create matrix A1 = [	1.0  0.0  0.0  0.0  0.0
							//			0.0  1.0  0.0  0.0  0.0  ]
		cout << "A1=\n"<< A1 << endl;
	
		Matrix A2(5,3,0.0,6.3);			//Create matrix A1 = [	0.0  6.3  6.3 
							//			6.3  0.0  6.3 
							//			6.3  6.3  0.0
							//			6.3  6.3  6.3
							//			6.3  6.3  6.3  ]
		cout << "A2=\n"<< A2 << endl;
		
		Matrix S = A1 * A2;			//Multiply A1 by A2
		cout <<"Wynik mnozenia A1*A2\nS=\n"<< S << endl;
	
		fstream f1;
		f1.open("matrix.dat", fstream::in);
		Matrix B(f1);			//Read the matrix data from file matrix.dat
		f1.close();			//First two values in this file specify the matrix dimensions
		cout <<"Macierz wczytana z pliku\nB=\n"<< B << endl;
		
		S = B;				//Assign B to S

			cout <<"S "<<S(0,0)<<endl;
			cout <<"B "<<B(0,0)<<endl;
		S(0,0) = 1.4;					//Modify S
		cout << "S[0][0]=" << S(0,0) << endl;		//Verify S
		cout << "B[0][0]=" << B(0,0) << endl;		//Verify B
    					//cout <<"Macierz wczytana z pliku\nB=\n"<< B << endl;
}
    catch(IndexOutOfRange&)
    {
        cout << "IndexOutOfRange" << endl; 		//Index poza zakresem
    }
    catch(WrongDim&)
    {
        cout << "Wrong Matrix Dimensions" << endl; 	//Zle wymiary macierzy
    }
    catch(bad_alloc)
    {
        cout << "Out of Memory" << endl; 		//Blad pamieci
    }

    return 0;
}
