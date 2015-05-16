#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
//#define PRINT_TEMP_FIELD
using namespace std;

double maxTemp;
double minTemp;
double dx;
double dy;
double dt;
int x;
int y;
int size;
size_t iterationAmount;
double defTemp;

class Edge {
    public:
	int type;
	double temp;
};

class Node {
    public:
	Node() { id = -1; temp = defTemp; };
	int& operator () () { return id; };
	bool operator == (Node A) { return temp = A.temp; };
	bool operator < (Node A) { return temp < A.temp; };
	bool operator > (Node A) { return temp > A.temp; };
	void setTemp(double ttemp) { temp = ttemp; };
	double getTemp() { return temp; };
    private:
	int id;
	double temp;
};

vector < vector < Node > > index(0);
vector < vector < double > > AMatrix(0);
vector < double > B(0);
vector < Edge > firstLevel(4);

void initializing(int argc, char *argv[]) {
    if (argc != 7) {
	printf("Lack of arguments!\n");
	exit(-1);
    }
    double sizeX = atof(argv[1]);
    double sizeY = atof(argv[2]);

    x = atoi(argv[3]);
    y = atoi(argv[4]);
    size = x * y;
    dx = sizeX / (x - 1);
    dy = sizeY / (y - 1);

    int timeToCount = atoi(argv[5]);
    dt = atof(argv[6]);
    iterationAmount = timeToCount / dt + 1;
    minTemp = defTemp = 0;
    maxTemp = 100;
}

void getEdge(vector < Edge > &mas) {
    int i;
    FILE* fp = fopen("settings.txt", "r");
    float temp;
    for (i = 0; i < 4; ++i) {
	fscanf(fp, "%d %f\n", &(mas[i].type), &temp);
	mas[i].temp = temp;
	cout << mas[i].type << endl;
    }
    fclose(fp);
}

void createBMas() {
    size_t i, j;
    for (i = 1; i < x - 1; ++i) {
	for (j = 1; j < y - 1; ++j) {
	    B[index[i][j]()] = -1 * index[i][j].getTemp() * dx * dx * dy * dy;
	}
    }

    if (firstLevel[0].type == 1) {
	// Top
	for (i = 1; i < x - 1; ++i) {
	    B[index[i][y - 1]()] = firstLevel[0].temp;
	}
    } else {
	for (i = 1; i < x - 1; ++i) {
	    B[index[i][y - 1]()] = firstLevel[0].temp * dy;
	}
    }

    if (firstLevel[1].type == 1) {
	// Right
	for (i = 0; i < y; ++i) {
	    B[index[x - 1][i]()] = firstLevel[1].temp;
	}
    } else {
	for (i = 0; i < y; ++i) {
	    B[index[x - 1][i]()] = firstLevel[1].temp * dx;
	}
    }

    if (firstLevel[2].type == 1) {
	// Bottom
	for (i = 1; i < x - 1; ++i) {
	    B[index[i][0]()] = firstLevel[2].temp;
	}
    } else {
	for (i = 1; i < x - 1; ++i) {
	    B[index[i][0]()] = firstLevel[2].temp * dy;
	}
    }

    if (firstLevel[3].type == 1) {
	// Left
	for (i = 0; i < y; ++i) {
	    B[index[0][i]()] = firstLevel[3].temp;
	}
    } else {
	for (i = 0; i < y; ++i) {
	    B[index[0][i]()] = firstLevel[3].temp * dx;
	}
    }
}

void createAMas() {

    int i = 0, j = 0;

    AMatrix.resize(size);
    B.resize(size, 0);

    for (i = 0; i < size; ++i) {
	AMatrix[i].resize(size, 0);
    }
    // Make matrix for computations
    for (i = 1; i < x - 1; ++i) {
	for (j = 1; j < y - 1; ++j) {
	    AMatrix[index[i][j]()][index[i][j]()] = 
		- 2 * dx * dx * dt 
		- 2 * dy * dy * dt
		- dx * dx * dy * dy;
	    AMatrix[index[i][j]()][index[i - 1][j]()] = dy * dy * dt;
	    AMatrix[index[i][j]()][index[i + 1][j]()] = dy * dy * dt;
	    AMatrix[index[i][j]()][index[i][j + 1]()] = dx * dx * dt;
	    AMatrix[index[i][j]()][index[i][j - 1]()] = dx * dx * dt;
	}
    }

    getEdge(firstLevel);

    printf("Top:    type: %d, value: %6.2f\n", firstLevel[0].type, firstLevel[0].temp);
    printf("Right:  type: %d, value: %6.2f\n", firstLevel[1].type, firstLevel[1].temp);
    printf("Bottom: type: %d, value: %6.2f\n", firstLevel[2].type, firstLevel[2].temp);
    printf("Left:   type: %d, value: %6.2f\n", firstLevel[3].type, firstLevel[3].temp);

    if (firstLevel[0].type == 1) {
	// Top
	for (i = 1; i < x - 1; ++i) {
	    AMatrix[index[i][y - 1]()][index[i][y - 1]()] = 1;
	}
    } else {
	for (i = 1; i < x - 1; ++i) {
	    AMatrix[index[i][y - 1]()][index[i][y - 1]()] = 1;
	    AMatrix[index[i][y - 1]()][index[i][y - 2]()] = -1;
	}
    }

    if (firstLevel[1].type == 1) {
	// Right
	for (i = 0; i < y; ++i) {
	    AMatrix[index[x - 1][i]()][index[x - 1][i]()] = 1;
	}
    } else {
	for (i = 0; i < y; ++i) {
	    AMatrix[index[x - 1][i]()][index[x - 1][i]()] = 1;
	    AMatrix[index[x - 1][i]()][index[x - 2][i]()] = -1;
	}
    }

    if (firstLevel[2].type == 1) {
	// Bottom
	for (i = 1; i < x - 1; ++i) {
	    AMatrix[index[i][0]()][index[i][0]()] = 1;
	}
    } else {
	for (i = 1; i < x - 1; ++i) {
	    AMatrix[index[i][0]()][index[i][0]()] = 1;
	    AMatrix[index[i][0]()][index[i][1]()] = -1;
	}
    }

    if (firstLevel[3].type == 1) {
	// Left
	for (i = 0; i < y; ++i) {
	    AMatrix[index[0][i]()][index[0][i]()] = 1;
	}
    } else {
	for (i = 0; i < y; ++i) {
	    AMatrix[index[0][i]()][index[0][i]()] = 1;
	    AMatrix[index[0][i]()][index[1][i]()] = -1;
	}
    }
}

void createIndex() {
    index.resize(x);
    int count = 0;
    for (int i = 0; i < x; ++i) {
	index[i].resize(y);
	for (int j = 0; j < y; ++j) {
	    index[i][j]() = count++;
	}
    }
}


void toDefault(vector < double > &mas, size_t num, double &Bb) {
    double temp =  mas[num];
    if (temp == 0) {
	return;
    }
    size_t i;
    for (i = 0; i < mas.size(); ++i) {
	mas[i] = mas[i] / temp;
    }
    Bb /= temp;
}

void backStep(vector < vector < double > > &mas, vector < double > &b) {
    size_t i, z;
    for (i = mas.size() - 1; i > 0; --i)
	for (z = 0; z < i; ++z) {
	    b[z] -= mas[z][i] * b[i] / mas[i][i];
	    mas[z][i] = 0;
	}
}

void differ(const vector < double > &base,
	vector < double > &toEdit, 
	size_t num, double b, double &Bb) {
    double temp = toEdit[num];
    size_t i;
    for (i = num; i < toEdit.size(); ++i) {
	toEdit[i] -= base[i] * temp;
    }
    Bb -= b * temp;
}

void setTemprature() {
    for (int i = 0; i < index.size(); ++i) {
	for (int j = 0; j < index[i].size(); ++j) {
	    double temp = B[index[i][j]()];
	    if (minTemp > temp)
		minTemp = temp;
	    if (maxTemp < temp)
		maxTemp = temp;
	    index[i][j].setTemp(temp);
	}
    }
}

void printToFile(int countIteration) {
    stringstream stream;
    stream << "out/step" << countIteration;
    ofstream file;
    file.open(stream.str().c_str(), ios::out | ios::trunc);
    for (size_t i = 0; i < index.size(); ++i) {
	for (size_t j = 0; j < index[i].size(); ++j) {
	    file << i * dx << ' ' << j * dy << ' ' << index[i][j].getTemp() << endl;
	}
	file << endl;
    }
    file.close();
}

void printResultFile() {
    stringstream pause;
    pause << "pause " << dt;
    ofstream file;
    file.open("result", ios::out | ios::trunc);
    file << "set cbrange [" << (int)minTemp << ':' << (int)maxTemp + 1 << ']' << endl;
    file << "set view map" << endl;
    for (size_t i = 0; i < iterationAmount; ++i) {
	file << "splot 'out/step" << i << "' with pm3d" << endl << pause.str() << endl;
    }
    file << "pause -1" << endl;
    file.close();
}

void printTempField() {
    for (size_t j = index[0].size(); j > 0; --j) {
	for (size_t i = 0; i < index.size(); ++i) {
	    std::cout.width(4);
	    std::cout << (int)index[i][j - 1].getTemp();
	}
	cout << endl;
    }
}

void printAMasAndB(vector < vector < double > > &A, vector < double > &b) {
    for (size_t i = 0; i < A.size(); ++i) {
	cout.width(3);
	cout << i << ") ";
	for (size_t j = 0; j < A[i].size(); ++j) {
	    std::cout.width(3);
	    std::cout << A[i][j];
	}
	cout << "| " << setw(3) << b[i] << endl;
    }
}

void printIndexes() {
    for (size_t i = 0; i < index.size(); ++i) {
	for (size_t j = 0; j < index[i].size(); ++j) {
	    std::cout.width(3);
	    std::cout << index[i][j]();
	}
	cout << endl;
    }
}

void iteration(vector < vector < double > > A) {
    for (int i = 0; i < A.size(); ++i) {
	toDefault(A[i], i, B[i]);
	for (int j = i + 1; j < A.size(); ++j) {
	    differ(A[i], A[j], i, B[i], B[j]);
	}
    }
    backStep(A, B);
}

int main(int argc, char *argv[]) {

    initializing(argc, argv);

    printf("dx = %f; dy = %f;\n", dx, dy);


    createIndex();
    createAMas();
    createBMas();

#ifdef PRINT
    printIndexes();
    printAMasAndB(AMatrix, B);
#endif

    for (int iterNum = 0; iterNum < iterationAmount; ++iterNum) {
	cout << "Iteration num: " << iterNum << endl;
	createBMas();
	iteration(AMatrix);
	setTemprature();
#ifdef PRINT_TEMP_FIELD
	printTempField();
#endif
	printToFile(iterNum);
    }

    printResultFile();

    exit(0);
}
