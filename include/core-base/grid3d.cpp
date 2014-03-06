
template <class T> Grid3D<T>::Grid3D()
{
	m_rows = 0;
	m_cols = 0;
	m_data = NULL;
}

template <class T> Grid3D<T>::Grid3D(UINT rows, UINT cols)
{
	m_rows = rows;
	m_cols = cols;
	m_data = new T[rows * cols];
}

template <class T> Grid3D<T>::Grid3D(UINT rows, UINT cols, const T &clearValue)
{
	m_rows = rows;
	m_cols = cols;
	m_data = new T[rows * cols];
	clear(clearValue);
}

template <class T> Grid3D<T>::Grid3D(const Grid3D<T> &G)
{
	m_rows = G.m_rows;
	m_cols = G.m_cols;

	const UINT totalEntries = m_rows * m_cols;
	m_data = new T[totalEntries];
	for(UINT i = 0; i < totalEntries; i++)
	{
		m_data[i] = G.m_data[i];
	}
}

template <class T> Grid3D<T>::Grid3D(Grid3D<T> &&G)
{
	m_rows = G.m_rows;
	m_cols = G.m_cols;
	m_data = G.m_data;

	G.m_rows = 0;
	G.m_cols = 0;
	G.m_data = NULL;
}

template <class T> Grid3D<T>::~Grid3D()
{
	deleteMemory();
}

template <class T> void Grid3D<T>::deleteMemory()
{
	m_rows = 0;
	m_cols = 0;
	if(m_data != NULL)
	{
		delete[] m_data;
		m_data = NULL;
	}
}

template <class T> Grid3D<T>& Grid3D<T>::operator = (const Grid3D<T> &G)
{
	if(m_data) delete[] m_data;
	m_rows = G.m_rows;
	m_cols = G.m_cols;

	const UINT totalEntries = m_rows * m_cols;
	m_data = new T[totalEntries];
	for(UINT i = 0; i < totalEntries; i++) m_data[i] = G.m_data[i];

	return *this;
}

template <class T> Grid3D<T>& Grid3D<T>::operator = (Grid3D<T> &&G)
{
	std::swap(m_rows, G.m_rows);
	std::swap(m_cols, G.m_cols);
	std::swap(m_data, G.m_data);
	return *this;
}

template <class T> void Grid3D<T>::allocate(UINT rows, UINT cols)
{
	m_rows = rows;
	m_cols = cols;
	if(m_data) delete[] m_data;
	m_data = new T[rows * cols];
}

template <class T> void Grid3D<T>::allocate(UINT rows, UINT cols, const T &clearValue)
{
	allocate(rows, cols);
	clear(clearValue);
}

template <class T> void Grid3D<T>::clear(const T &clearValue)
{
	const UINT totalEntries = m_rows * m_cols;
	for(UINT i = 0; i < totalEntries; i++) m_data[i] = clearValue;
}

template <class T> std::pair<UINT, UINT> Grid3D<T>::maxIndex() const
{
	std::pair<UINT, UINT> maxIndex(0, 0);
	const T *maxValue = m_data;
	for(UINT rowIndex = 0; rowIndex < m_rows; rowIndex++)
		for(UINT colIndex = 0; colIndex < m_cols; colIndex++)
		{
			const T *curValue = &m_data[rowIndex * m_cols + colIndex];
			if(*curValue > *maxValue)
			{
				maxIndex = std::make_pair(rowIndex, colIndex);
				maxValue = curValue;
			}
		}
	return maxIndex;
}

template <class T> const T& Grid3D<T>::maxValue() const
{
	std::pair<UINT, UINT> index = maxIndex();
	return m_data[index.first * m_cols + index.second];
}

template <class T> std::pair<UINT, UINT> Grid3D<T>::minIndex() const
{
	std::pair<UINT, UINT> minIndex(0, 0);
	const T *minValue = &m_data[0];
	for(UINT rowIndex = 0; rowIndex < m_rows; rowIndex++)
	{
		for(UINT colIndex = 0; colIndex < m_cols; colIndex++)
		{
			const T *curValue = &m_data[rowIndex * m_cols + colIndex];
			if(*curValue < *minValue)
			{
				minIndex = std::make_pair(rowIndex, colIndex);
				minValue = curValue;
			}
		}
	}
	return minIndex;
}

template <class T> const T& Grid3D<T>::minValue() const
{
	std::pair<UINT, UINT> index = minIndex();
	return m_data[index.first * m_cols + index.second];
}
