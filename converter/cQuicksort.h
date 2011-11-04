#pragma once


class cQuicksort {


public:
	void quicksort(int*, int, int);


private:
	int partition(int*, int, int, int);
	int median3(int*,int,int);
	void swap(int &, int &);

};
