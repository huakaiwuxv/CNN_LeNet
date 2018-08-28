#include <Array2D.h>
#include <float.h>
#include <algorithm>

template <typename T>
Array2D<T>::Array2D<T>(int col, int row, T value)
{
	if (col <= 0 || row <= 0)
	{
		cout << "size < 0 !" << endl << "Array2D init failed!" << endl;
	}

	vector<T> vector_temp;
	vector_temp.assign(row, value);
	_array2D.assign(col, vector_temp);
}


template <typename T>
void Array2D<T>::from_image_64FC1(const Mat &img)
{
	int row, col, i;
	row = img.rows;
	col = img.cols;

	// 初始化_array2D的大小尺寸
	_array2D.resize(col);
	for (i = 0; i < col; i++)
	{
		_array2D.at(i).resize(row);
	}

	for (int i = 0; i < row; i++)
	{
		const double* pData = img.ptr<double>(i);	//第i+1行的所有元素
		for (int j = 0; j < col; j++)
		{
			_array2D.at(j).at(i) = (T)pData[j];
			//这行也可以：array_ret.at(j).at(i) = img.at<double>(i, j);
		}
	}
}


template <typename T>
void Array2D<T>::create(int col, int row, T value)
{
	if (col <= 0 || row <= 0)
	{
		cout << "size < 0 !" << endl << "Array2D.create() stoped!" << endl;
	}

	vector<T> vector_temp;
	vector_temp.assign(row, value);

	_array2D.assign(col, vector_temp);
}


template <typename T>
void Array2D<T>::zeros(int col, int row)
{
	create(col, row, 0);
}


template <typename T>
vector<T>& Array2D<T>::at(int index)
{
	return _array2D.at(index);
}


template <typename T>
const vector<T>& Array2D<T>::at(int index) const
{
	return _array2D.at(index);
}


template <typename T>
void Array2D<T>::push_back(vector<T> val)
{
	_array2D.push_back(val);
}


template <typename T>
void Array2D<T>::get_specific_patch(const Array2D<T> &array2D, int size_col, int size_row, int pos_col, int pos_row)
{
	if (size_col + pos_col > array2D.size() || size_row + pos_row > array2D.at(0).size())
	{
		cout << "pos or size has exceed range!" << endl << "Array2D.get_specific_patch() failed!" << endl;
	}

	_array2D.resize(size_col);

	for (int i = 0; i < size_col; i++)
	{
		for (int j = 0; j < size_row; j++)
		{
			_array2D.at(i).push_back(array2D.at(pos_col + i).at(pos_row + j));
		}
	}
}


template <typename T>
void Array2D<T>::set_zero()
{
	int col = _array2D.size();

	if (col <= 0)
	{
		cout << "Array2D is empty!" << endl << "Array2D.set_zero() failed!" << endl;
		return;
	}

	int row = _array2D.at(0).size();

	if (row <= 0)
	{
		cout << "Array2D is empty!" << endl << "Array2D.set_zero() stoped!" << endl;
		return;
	}

	vector<T> vector_temp;
	vector_temp.assign(row, 0);

	_array2D.assign(col, vector_temp);
}


template <typename T>
void Array2D<T>::set_zero_same_size_as(const Array2D<T> &array2D)
{
	int i, j;

	_array2D.resize(array2D.size());

	for (i = 0; i < _array2D.size(); i++)
	{
		_array2D[i].assign(array2D.at(i).size(), 0);
	}
}


template <typename T>
void Array2D<T>::normalize()
{
	// find min and max;

	double min = DBL_MAX;
	double max = -DBL_MAX;
	double array_ij;

	for (int i = 0; i < _array2D.size(); i++)
	{
		for (int j = 0; j < _array2D.at(i).size(); j++)
		{
			array_ij = _array2D.at(i).at(j);
			if (array_ij > max)
			{
				max = array_ij;
			}
			if (array_ij < min)
			{
				min = array_ij;
			}
		}
	}

	// normalize
	for (int i = 0; i < _array2D.size(); i++)
	{
		for (int j = 0; j < _array2D.at(i).size(); j++)
		{
			array_ij = _array2D.at(i).at(j);
			array_ij -= min;
			array_ij /= (max - min);
			_array2D.at(i).at(j) = (T)array_ij;
		}
	}
}


template <typename T>
void Array2D<T>::flip_xy()
{
	for (int i = 0; i < _array2D.size(); i++)
	{
		reverse(_array2D.at(i).begin(), _array2D.at(i).end());
	}

	reverse(_array2D.begin(), _array2D.end());
}


template <typename T>
Mat Array2D<T>::to_Mat_64FC1() const
{
	if (_array2D.empty())
	{
		cout << "Array2D is empty!" << endl << "Array2D.to_Mat_64FC1() failed" << endl;
		Mat img;
		return img;
	}

	int row = _array2D.at(0).size();
	int col = _array2D.size();

	Mat img(row, col, CV_64FC1);
	double *ptmp = NULL;
	for (int i = 0; i < row; i++)
	{
		ptmp = img.ptr<double>(i);

		for (int j = 0; j < col; ++j)
		{
			ptmp[j] = _array2D.at(j).at(i);
		}
	}
	return img;
}


template <typename T>
Array2D<T> Array2D<T>::operator + (const Array2D<T> &array2D) const
{
	int col_A = _array2D.size();
	int row_A = _array2D.at(0).size();
	int col_B = array2D.size();
	int row_B = array2D.at(0).size();

	if (col_A != col_B || row_A != row_B)
	{
		cout << "array_A size is not same as arrow_B size!" << endl << "Array2D.operator + failed!" << endl;
		Array2D<T> temp;
		return temp;
	}

	Array2D<T> add_result(col_A, row_A, 0);
	add_result.set_zero();

	for (int i = 0; i < col_A; i++)
	{
		for (int j = 0; j < row_A; j++)
		{
			add_result.at(i).at(j) = _array2D.at(i).at(j) + array2D.at(i).at(j);
		}
	}

	return add_result;
}


template <typename T>
Array2D<T> Array2D<T>::operator * (const Array2D<T> &array2D) const
{
	int col_A = _array2D.size();
	int row_A = _array2D.at(0).size();
	int col_B = array2D.size();
	int row_B = array2D.at(0).size();

	if (col_A != col_B || row_A != row_B)
	{
		cout << "array_A size is not same as arrow_B size!" << endl << "Array2D.operator * failed!" << endl;
		Array2D<T> temp;
		return temp;
	}

	Array2D<T> dot_product_result(col_A, row_A, 0);
	dot_product_result.set_zero();

	for (int i = 0; i < col_A; i++)
	{
		for (int j = 0; j < row_A; j++)
		{
			dot_product_result.at(i).at(j) = _array2D.at(i).at(j) * array2D.at(i).at(j);
		}
	}

	return dot_product_result;
}


template <typename T>
T Array2D<T>::sum() const
{
	int col = _array2D.size();
	int row = _array2D.at(0).size();

	T sum_ret = 0;

	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			sum_ret += _array2D.at(i).at(j);
		}
	}

	return sum_ret;
}


template <typename T>
void Array2D<T>::dot_product(const Array2D<T> &array2D)
{
	int col_A = _array2D.size();
	int row_A = _array2D.at(0).size();
	int col_B = array2D.size();
	int row_B = array2D.at(0).size();

	if (col_A != col_B || row_A != row_B)
	{
		cout << "array_A size is not same as arrow_B size!" << endl << "Array2D.dot_product() failed!" << endl;
		return;
	}

	for (int i = 0; i < col_A; i++)
	{
		for (int j = 0; j < row_A; j++)
		{
			_array2D.at(i).at(j) = _array2D.at(i).at(j) * array2D.at(i).at(j);
		}
	}
}


template <typename T>
int Array2D<T>::size() const
{
	return _array2D.size();
}


template <typename T>
void Array2D<T>::print() const
{
	int i, j;

	for (i = 0; i < _array2D.at(0).size(); i++)
	{
		for (j = 0; j < _array2D.size(); j++)
		{
			cout << _array2D.at(j).at(i) << " ";
		}
		cout << endl;
	}
}


template <typename T>
void Array2D<T>::show_image_64FC1() const
{
	Mat image = to_Mat_64FC1();

	// 显示图片   
	imshow("img", image);
	// 等待time_msec后窗口自动关闭    
	waitKey(3000);

	destroyWindow("img");
}


template <typename T>
void Array2D<T>::show_image_64FC1(int time_msec) const
{
	Mat image = to_Mat_64FC1();

	// 显示图片   
	imshow("img", image);
	// 等待time_msec后窗口自动关闭    
	waitKey(time_msec);

	destroyWindow("img");
}









// only define for double
template Array2D<double>::Array2D<double>(int col, int row, double value);
template void Array2D<double>::from_image_64FC1(const Mat &img);
template void Array2D<double>::create(int col, int row, double value);
template void Array2D<double>::zeros(int col, int row);
template vector<double>& Array2D<double>::at(int index);
template const vector<double>& Array2D<double>::at(int index) const;
template void Array2D<double>::push_back(vector<double> val);
template void Array2D<double>::get_specific_patch(const Array2D<double> &array2D, int size_col, int size_row, int pos_col, int pos_row);
template void Array2D<double>::set_zero();
template void Array2D<double>::set_zero_same_size_as(const Array2D<double> &array2D);
template void Array2D<double>::normalize();
template void Array2D<double>::flip_xy();
template Mat Array2D<double>::to_Mat_64FC1() const;
template Array2D<double> Array2D<double>::operator + (const Array2D<double> &array2D) const;
template Array2D<double> Array2D<double>::operator * (const Array2D<double> &array2D) const;
template double Array2D<double>::sum() const;
template void Array2D<double>::dot_product(const Array2D<double> &array2D);
template int Array2D<double>::size() const;
template void Array2D<double>::print() const;
template void Array2D<double>::show_image_64FC1() const;
template void Array2D<double>::show_image_64FC1(int time_msec) const;




