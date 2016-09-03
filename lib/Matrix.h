#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable : 4996)

#include <cassert>
#include <iostream>
#include <algorithm>
#include <chrono>

using namespace std::chrono;

template <class T>
class Matrix {
public:
	Matrix() : Height(0), Width(0), data(nullptr) {}
	Matrix(int h, int w, T value) : data(nullptr) { AllocateAndFill(h, w, value); }
	Matrix(int h, int w, int windowSize, bool vertical) : data(nullptr) {
		AllocateRandomized(h, w, windowSize, vertical);
	}

	// Copy constructor.
	Matrix(const Matrix<T> &other) : Height(other.Height), Width(other.Width), data(nullptr) {
		AllocateAndFill(other);
	}
	// Move constructor.
	Matrix(Matrix<T> &&other) : Matrix<T>() {
		swap(*this, other);
	}

	~Matrix() {
		Destroy();
	}

	// Cast.
	template<class U>
	Matrix<U> castTo() const {
		Matrix<U> result(Height, Width, 0);

		for (int i = 0; i < Height; ++i)
			for (int j = 0; j < Width; ++j)
				result[i][j] = static_cast<U>(data[i][j]);
		
		return result;
	}

	inline int getHeight() const { return Height; }
	inline int getWidth() const { return Width; }

	// Operators.
	Matrix<T> &operator=(Matrix<T> other) {
		swap(*this, other);
		return *this;
	}

	bool operator==(const Matrix<T> &other) const {
		if (Height != other.getHeight() || Width != other.getWidth())
			return false;
		for (int i = 0; i < Height; ++i)
			for (int j = 0; j < Width; ++j)
				if (data[i][j] != other[i][j])
					return false;
		return true;
	}

	bool operator!=(const Matrix<T> &other) const { return !(*this == other); }

	Matrix<T> &operator+=(const Matrix<T> &other) {
		assert(Height == other.getHeight() && "Matrices have different heights!");
		assert(Width == other.getWidth() && "Matrices have different widths!");
		for (int i = 0; i < Height; ++i)
			for (int j = 0; j < Width; ++j)
				data[i][j] += other[i][j];
		return *this;
	}

	Matrix<T> &operator-=(const Matrix<T> &other) {
		assert(Height == other.getHeight() && "Matrices have different heights!");
		assert(Width == other.getWidth() && "Matrices have different widths!");
		for (int i = 0; i < Height; ++i)
			for (int j = 0; j < Width; ++j)
				data[i][j] -= other[i][j];
		return *this;
	}

	friend Matrix<T> operator+(Matrix<T> lhs, const Matrix<T> &rhs) {
		lhs += rhs;
		return lhs;
	}

	friend Matrix<T> operator-(Matrix<T> lhs, const Matrix<T> &rhs) {
		lhs -= rhs;
		return lhs;
	}

	T *&operator[](int i) {
		assert(i < Height && "Row index out of bounds!");
		assert(data && "Subscript operation on null data!");
		return data[i];
	}

	const T *operator[](int i) const {
		assert(i < Height && "Row index out of bounds!");
		assert(data && "Subscript operation on null data!");
		return data[i];
	}

	// Common mathematical functions.

	// SSD between whole matrices.
	T SSD(const Matrix<T> &other) {
		assert(Height == other.getHeight() && "Matrices have different heights!");
		assert(Width == other.getWidth() && "Matrices have different widths!");
		T result = 0;
		for (unsigned int i = 0; i < Height; ++i)
			for (unsigned int j = 0; j < Width; ++j) {
				T diff = data[i][j] - other[i][j];
				result += diff * diff;
			}
		return result;
	}

	// SSD between patches.
	T SSD(const Matrix<T> &other, unsigned i1, unsigned j1, unsigned i2, unsigned j2,
		  int patchRadX, int patchRadY = 0) {
		if (!patchRadY)
			patchRadY = patchRadX;
		assert(i1 >= patchRadY && i2 >= patchRadY &&
			i1 + patchRadY < Height && i2 + patchRadY < other.getHeight());
		assert(j1 >= patchRadX && j2 >= patchRadX &&
			j1 + patchRadX < Width && j2 + patchRadX < other.getWidth());
		assert(patchRadX > 0 && patchRadY > 0);
		T result = 0;
		for (int dy = -1 * patchRadY; dy <= patchRadY; ++dy)
			for (int dx = -1 * patchRadX; dx <= patchRadX; ++dx) {
				T diff = data[i1 + dy][j1 + dx] - other[i2 + dy][j2 + dx];
				result += diff * diff;
			}
		return result;
	}

	// Returns known SSD shifted 1 pixel right.
	T SSD_ShiftedRight(const Matrix<T> &other, unsigned i1, unsigned j1, unsigned i2, unsigned j2,
					   int patchRadX, int patchRadY, T knownSSD) {
		assert(i1 >= patchRadY && i2 >= patchRadY &&
			i1 + patchRadY < Height && i2 + patchRadY < other.getHeight());
		assert(j1 > patchRadX && j2 > patchRadX &&
			j1 + patchRadX < Width && j2 + patchRadX < other.getWidth());
		assert(patchRadX > 0 && patchRadY > 0);
		T result = knownSSD;
		for (int dy = -1 * patchRadY; dy <= patchRadY; ++dy) {
			// Positive.
			T A = data[i1 + dy][j1 + patchRadX];
			T B = other[i2 + dy][j2 + patchRadX];
			T C = data[i1 + dy][j1 - patchRadX - 1];
			T D = other[i2 + dy][j2 - patchRadX - 1];
			T diff1 = data[i1 + dy][j1 + patchRadX] - other[i2 + dy][j2 + patchRadX];
			// Negative.
			T diff2 = data[i1 + dy][j1 - patchRadX - 1] - other[i2 + dy][j2 - patchRadX - 1];
			result = result + diff1 * diff1 - diff2 * diff2;
		}
		return result;
	}

	// Returns known SSD shifted 1 pixel left.
	T SSD_ShiftedLeft(const Matrix<T> &other, unsigned i1, unsigned j1, unsigned i2, unsigned j2,
		int patchRadX, int patchRadY, T knownSSD) {
		assert(i1 >= patchRadY && i2 >= patchRadY &&
			i1 + patchRadY < Height && i2 + patchRadY < other.getHeight());
		assert(j1 >= patchRadX && j2 >= patchRadX &&
			j1 + patchRadX + 1 < Width && j2 + patchRadX + 1 < other.getWidth());
		assert(patchRadX > 0 && patchRadY > 0);
		T result = knownSSD;
		for (int dy = -1 * patchRadY; dy <= patchRadY; ++dy) {
			// Positive.
			T diff1 = data[i1 + dy][j1 - patchRadX] - other[i2 + dy][j2 - patchRadX];
			// Negative.
			T diff2 = data[i1 + dy][j1 + patchRadX + 1] - other[i2 + dy][j2 + patchRadX + 1];
			result = result + diff1 * diff1 - diff2 * diff2;
		}
		return result;
	}

	// Returns known SSD shifted 1 pixel up.
	T SSD_ShiftedUp(const Matrix<T> &other, unsigned i1, unsigned j1, unsigned i2, unsigned j2,
		int patchRadX, int patchRadY, T knownSSD) {
		assert(i1 > patchRadY && i2 > patchRadY &&
			i1 + patchRadY < Height && i2 + patchRadY < other.getHeight());
		assert(j1 >= patchRadX && j2 >= patchRadX &&
			j1 + patchRadX < Width && j2 + patchRadX < other.getWidth());
		assert(patchRadX > 0 && patchRadY > 0);
		T result = knownSSD;
		for (int dx = -1 * patchRadX; dx <= patchRadX; ++dx) {
			// Positive.
			T diff1 = data[i1 - patchRadY][j1 + dx] - other[i2 - patchRadY][j2 + dx];
			// Negative.
			T diff2 = data[i1 + patchRadY + 1][j2 + dx] - other[i2 + patchRadY + 1][j2 + dx];
			result = result + diff1 * diff1 - diff2 * diff2;
		}
		return result;
	}

	// Returns known SSD shifted 1 pixel down.
	T SSD_ShiftedDown(const Matrix<T> &other, unsigned i1, unsigned j1, unsigned i2, unsigned j2,
		int patchRadX, int patchRadY, T knownSSD) {
		assert(i1 >= patchRadY && i2 >= patchRadY &&
			i1 + patchRadY + 1 < Height && i2 + patchRadY + 1 < other.getHeight());
		assert(j1 >= patchRadX && j2 >= patchRadX &&
			j1 + patchRadX < Width && j2 + patchRadX < other.getWidth());
		assert(patchRadX > 0 && patchRadY > 0);
		T result = knownSSD;
		for (int dx = -1 * patchRadX; dx <= patchRadX; ++dx) {
			// Positive.
			T diff1 = data[i1 + patchRadY][j1 + dx] - other[i2 + patchRadY][j2 + dx];
			// Negative.
			T diff2 = data[i1 - patchRadY - 1][j2 + dx] - other[i2 - patchRadY - 1][j2 + dx];
			result = result + diff1 * diff1 - diff2 * diff2;
		}
		return result;
	}

	friend std::istream &operator>>(std::istream &is, Matrix<T> &m) {
		int h, w;
		is >> h >> w;
		m.AllocateAndFill(h, w, 0);
		for (int i = 0; i < h; ++i)
			for (int j = 0; j < w; ++j)
				is >> m[i][j];
		return is;
	}

	friend std::ostream &operator<<(std::ostream &os, const Matrix<T> &m) {
		for (int i = 0; i < m.getHeight(); ++i) {
			for (int j = 0; j < m.getWidth(); ++j) {
				os.width(3);
				os << m[i][j];
			}
			os << std::endl;
		}
		return os;
	}

	friend void swap(Matrix<T> &first, Matrix<T> &second) {
		using std::swap;
		swap(first.Height, second.Height);
		swap(first.Width, second.Width);
		swap(first.data, second.data);
	}

	void AllocateAndFill(int h, int w, T value) {
		Height = h;
		Width = w;

		Destroy();

		data = new (std::nothrow) T*[Height];
		if (!data)
			return;
		for (int i = 0; i < Height; ++i) {
			data[i] = new (std::nothrow) T[Width];
			for (int j = 0; j < Width; ++j)
				data[i][j] = value;
		}
	}

	void AllocateRandomized(int h, int w, int windowSize, bool vertical) {
		Height = h;
		Width = w;

		Destroy();

		time_point<system_clock, microseconds> seed = time_point_cast<microseconds>(system_clock::now());
		srand(seed.time_since_epoch().count());
		if (vertical)
			AllocateRandomizedVertical(windowSize);
		else
			AllocateRandomizedHorizontal(windowSize);
	}

	virtual void ReadFromFile(const std::string &fileName) {
		FILE *f = fopen(fileName.c_str(), "r");
		fscanf(f, "%d %d", &Height, &Width);
		AllocateAndFill(Height, Width, 0);
		for (int i = 0; i < Height; ++i)
			for (int j = 0; j < Width; ++j)
				fscanf(f, "%d", &data[i][j]);
		fclose(f);
	}

	virtual void SaveToFile(const std::string &fileName) const {
		FILE *f = fopen(fileName.c_str(), "w");
		fprintf(f, "%d %d\n", Height, Width);
		for (int i = 0; i < Height; ++i) {
			for (int j = 0; j < Width - 1; ++j)
				fprintf(f, "%d ", data[i][j]);
			fprintf(f, "%d\n", data[i][Width - 1]);
		}
	}


	void PRINT_SSD(const Matrix<T> &other, unsigned i1, unsigned j1, unsigned i2, unsigned j2,
		int patchRadX, int patchRadY = 0) {
		if (!patchRadY)
			patchRadY = patchRadX;
		assert(i1 >= patchRadY && i2 >= patchRadY &&
			i1 + patchRadY < Height && i2 + patchRadY < other.getHeight());
		assert(j1 >= patchRadX && j2 >= patchRadX &&
			j1 + patchRadX < Width && j2 + patchRadX < other.getWidth());
		assert(patchRadX > 0 && patchRadY > 0);
		T result = 0;
		for (int dy = -1 * patchRadY; dy <= patchRadY; ++dy) {
			for (int dx = -1 * patchRadX; dx <= patchRadX; ++dx) {
				std::cout.width(4);
				std::cout << other[i2 + dy][j2 + dx] << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}

private:
	void AllocateAndFill(const Matrix<T> &other) {
		Destroy();

		data = new (std::nothrow) T*[Height];
		if (!data)
			return;
		for (int i = 0; i < Height; ++i) {
			data[i] = new (std::nothrow) T[Width];
			for (int j = 0; j < Width; ++j)
				data[i][j] = other[i][j];
		}
	}

	void AllocateRandomizedVertical(int windowSize) {
		int windowWidth = 2 * windowSize + 1;
		data = new (std::nothrow) T*[Height];
		if (!data)
			return;
		for (int i = 0; i < Height; ++i) {
			data[i] = new (std::nothrow) T[Width];
			for (int j = 0; j < Width; ++j) {
				T value = i + rand() % windowWidth - windowSize;
				if (value < 0)
					value = 0;
				if (value >= Height)
					value = Height - 1;
				data[i][j] = value - i;
			}
		}
	}

	void AllocateRandomizedHorizontal(int windowSize) {
		int windowWidth = 2 * windowSize + 1;
		data = new (std::nothrow) T*[Height];
		if (!data)
			return;
		for (int i = 0; i < Height; ++i) {
			data[i] = new (std::nothrow) T[Width];
			for (int j = 0; j < Width; ++j) {
				T value = j + rand() % windowWidth - windowSize;
				if (value < 0)
					value = 0;
				if (value >= Width)
					value = Width - 1;
				data[i][j] = value - j;
			}
		}
	}

	void Destroy() {
		if (data) {
			for (int i = 0; i < Height; ++i)
				if (data[i])
					delete[] data[i];
			delete[] data;
		}
		data = nullptr;
	}

	int Height;
	int Width;
	T **data;
};





// Specification.
template<>
void Matrix<double>::ReadFromFile(const std::string &fileName) {
	FILE *f = fopen(fileName.c_str(), "r");
	fscanf(f, "%d %d", &Height, &Width);
	AllocateAndFill(Height, Width, 0);
	for (int i = 0; i < Height; ++i)
		for (int j = 0; j < Width; ++j)
			fscanf(f, "%lf", &data[i][j]);
	fclose(f);
}

template<>
void Matrix<double>::SaveToFile(const std::string &fileName) const {
	FILE *f = fopen(fileName.c_str(), "w");
	fprintf(f, "%d %d\n", Height, Width);
	for (int i = 0; i < Height; ++i) {
		for (int j = 0; j < Width - 1; ++j)
			fprintf(f, "%lf ", data[i][j]);
		fprintf(f, "%lf\n", data[i][Width - 1]);
	}
}