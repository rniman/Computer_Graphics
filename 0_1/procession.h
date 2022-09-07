#pragma once
#include <vector>
#include <random>
#include <iostream>

class procession
{
private:
	std::vector<std::vector<int>> m_procession;
	int m_diterminant;
	int m_N;

public:
	procession()
		:m_N(3)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 2);

		m_procession.resize(m_N);

		for (unsigned i = 0; i < m_N; ++i)
		{
			m_procession[0].push_back(dis(gen));
			m_procession[1].push_back(dis(gen));
			m_procession[2].push_back(dis(gen));
		}

		m_diterminant = cal_diterminant();
	}

	procession(int col, int row, std::vector<std::vector<int>> ori,int size)
		:m_N(size-1)
	{
		m_procession.resize(m_N);
		unsigned temp = 0;
		for (unsigned i = 0; i <= m_N; ++i)
		{
			if (i == col)
				continue;
			for (unsigned j = 0; j <= m_N; ++j)
			{
				if (j == row)
					continue;
				m_procession[temp].push_back(ori[i][j]);
			}
			temp++;
		}
		m_diterminant = cal_diterminant();
	}

	void print() const;

	int getValue(const unsigned& col, const unsigned& row) const;
	int getDet() const;

	int cal_diterminant() const;
	void transpose();
	void expand();
	void reSet();

	friend void add(const procession& p1, const procession& p2);
	friend void sub(const procession& p1, const procession& p2);
	friend void mul(const procession& p1, const procession& p2);
};

void procession::print() const
{
	for (unsigned i = 0; i < m_N; ++i)
	{
		for (unsigned j = 0; j < m_N; ++j)
		{
			std::cout << getValue(i, j) << ' ';
		}
		std::cout << std::endl;
	}
}

int procession::getValue(const unsigned& col, const unsigned& row) const
{

	return m_procession[col][row];
}

int procession::getDet() const
{
	return m_diterminant;
}

void procession::transpose()
{
	int temp;
	this->print();
	for (unsigned i = 0; i < m_N - 1; ++i)
	{
		for (unsigned j = 0; j < m_N; ++j)
		{
			if (i > j)
				continue;
			temp = m_procession[i][j];
			m_procession[i][j] = m_procession[j][i];
			m_procession[j][i] = temp;
		}
	}
}

void procession::expand()
{
	m_N = 4;
	m_procession.resize(4);
	for (unsigned i = 0; i < m_N - 1; ++i)
	{
		m_procession[i].push_back(0);
		m_procession[m_N - 1].push_back(0);
	}
	m_procession[m_N - 1].push_back(1);
}

int procession::cal_diterminant() const
{
	if (m_N == 4)
	{
		return m_procession[0][0] * procession(0, 0,m_procession,m_N).cal_diterminant() + m_procession[0][1] * (-1) * procession(0, 1, m_procession, m_N).cal_diterminant()
			+ m_procession[0][2] * procession(0, 2, m_procession, m_N).cal_diterminant() + m_procession[0][3] * (-1) * procession(0, 3, m_procession, m_N).cal_diterminant();
	}
	//3X3에 대해서만 되어있음
	return(
		m_procession[0][0] * (m_procession[1][1] * m_procession[2][2] - m_procession[2][1] * m_procession[1][2]) +
		m_procession[0][1] * (-1) * (m_procession[1][0] * m_procession[2][2] - m_procession[1][2] * m_procession[2][0]) +
		m_procession[0][2] * (m_procession[1][0] * m_procession[2][1] - m_procession[2][0] * m_procession[1][1])
		);

	//일반적인 행렬식 구현해보기
}

void procession::reSet()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 2);

	for (unsigned i = 0; i < m_N; ++i)
	{
		for (unsigned j = 0; j < m_N; ++j)
		{
			m_procession[i][j] = dis(gen);
		}
	}

	m_diterminant = cal_diterminant();
}

void add(const procession& p1, const procession& p2)
{
	if (p1.m_N != p2.m_N)
	{
		std::cout << "Could not add" << std::endl;
		return;
	}

	std::cout << "행렬덧셈" << std::endl;
	for (unsigned i = 0; i < p1.m_N; ++i)
	{
		for (unsigned j = 0; j < p1.m_N; ++j)
		{
			std::cout << p1.getValue(i, j) + p2.getValue(i, j) << ' ';
		}
		std::cout << std::endl;
	}
}

void sub(const procession& p1, const procession& p2)
{
	if (p1.m_N != p2.m_N)
	{
		std::cout << "Could not subtract" << std::endl;
		return;
	}

	std::cout << "행렬뺄셈" << std::endl;
	for (unsigned i = 0; i < p1.m_N; ++i)
	{
		for (unsigned j = 0; j < p1.m_N; ++j)
		{
			std::cout << p1.getValue(i, j) - p2.getValue(i, j) << ' ';
		}
		std::cout << std::endl;
	}
}

void mul(const procession& p1, const procession& p2)
{
	if (p1.m_N != p2.m_N)
	{
		std::cout << "Could not multiply" << std::endl;
		return;
	}

	std::cout << "행렬곱셈" << std::endl;
	for (unsigned i = 0; i < p1.m_N; ++i)
	{
		for (unsigned j = 0; j < p1.m_N; ++j)
		{
			int out_num = 0;
			for (unsigned k = 0; k < p1.m_N; ++k)
			{
				out_num += p1.m_procession[i][k] * p2.m_procession[k][j];
			}
			std::cout << out_num << ' ';
		}
		std::cout << std::endl;
	}
}