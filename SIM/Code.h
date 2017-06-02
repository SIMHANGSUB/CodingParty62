#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>
#define buf_size 1000

#define INFOMATION_GAIN true
#define GAIN_RATIO false
#define GINI_INDEX false

using namespace std;






class index_type
{
	private:
		
		class node
		{
		public:
			char str_value[buf_size];
			int int_value;
			node* next;

			node(int num, char* str)
			{
				int_value = num;
				strcpy_s(str_value,str);
			}


		};

		int* col_max;
		node** ptr_arr;

	public:

		int get_col_max(int i)
		{
			return col_max[i];
		}

		void set(int max)
		{
			col_max = new int[max];
			ptr_arr = new node*[max];
			for (int i = 0; i < max; i++)
			{
				ptr_arr[i] = NULL;
				col_max[i] = -1;
			}

		}

		int search(int col, char* string)
		{
			node* temp = ptr_arr[col];

			for (; temp; temp = temp->next)
			{
				//outFile << "compare" << temp->str_value << " " << string << endl;
				if (strcmp(temp->str_value, string) == 0)
					return temp->int_value;
			}
			// 원하는 값을 찾으면 해당 값을 리턴
			
			
			node* temp2 = ptr_arr[col];
			ptr_arr[col] = new node(++col_max[col], string);
			ptr_arr[col]->next = temp2;
			//outFile << "insert" << string << "index : "<<col_max[col]<< endl;
			return col_max[col];
		}

		const char* search_and_get_string(int col, int value)
		{
			node* temp = ptr_arr[col];

			for (; temp; temp = temp->next)
			{
				if (temp->int_value == value)
					return temp->str_value;
			}
		}

};

index_type index;

void parsing(int* arr, char* string)
{
	int col = 0;
	char temp[buf_size];
	char* start = string;

	for (int i = 0; string[i] ; i++)
	{
		if (string[i] == '\t' )
		{
			string[i] = 0;
			
			strcpy_s(temp, start);

			arr[col] = index.search(col, temp);
			col++;
			start = &string[i] + 1;

		}


	}

	strcpy_s(temp, start);
	arr[col] = index.search(col, temp);


}
class CALCUL
{


public:

		CALCUL()
		{
			max_row = -1;
		}

		class row
	{

	public:

		int* arr;
		row* next;

		row(int sizeof_col, char* string)
		{

			arr = new int[sizeof_col];
			for (int i = 0; i < sizeof_col; i++)
				arr[i] = -1;

			parsing(arr, string);


		}




	};

		int get_max_row()
		{
			return max_row;
		}

		int get_max_col()
		{
			return max_col;
		}

		row* get_head()
		{
			return head;
		}

		void set (int sizeof_col)
		{
			max_col = sizeof_col;
			head = NULL;

		}

		void insert_arr( char* string )
		{
			max_row++;
			if (!head) head = new row(max_col, string);
			else
			{
				row* temp = head;
				head = new row(max_col , string );
				head->next = temp;

			}

		}

		void ShowDate()
		{
			for (row* temp = head; temp; temp = temp->next)
			{
				for ( int i = 0 ; i < max_col ; i++ )
					cout << temp->arr[i] ;
				cout << endl;
			}
			

		}

		void get_bitmap(int* bitmap, char* string)
		{
			parsing(bitmap, string);

		}

		void create_decision_tree();
		void classification();

	private:

	int max_col;
	int max_row;
	row* head;
	
};

CALCUL calcul;

class tree_node
{

private:

	int* bitmap_row;
	int* bitmap_col;
	int base_attr;
	int result;
	tree_node** child;


public:


	tree_node() // root setting
	{
		bitmap_col = new int[calcul.get_max_col()];
		bitmap_row = new int[calcul.get_max_row()];

		for (int i = 0; i < calcul.get_max_col(); i++) bitmap_col[i] = 1;
		for (int i = 0; i < calcul.get_max_row(); i++) bitmap_row[i] = 1;
		
		result = -1;
	}

	tree_node(int result)
	{
		this->result = result;
	}

	tree_node(int base, int partition , int*bitmap_row, int*bitmap_col)
	{

		int row = 0;
		int num_row = 0;
		int num_col = 0;
		int max_col = calcul.get_max_col();
		int max_row = calcul.get_max_row();

		this->bitmap_col = new int[calcul.get_max_col()];
		this->bitmap_row = new int[calcul.get_max_row()];

		result = -1;


		for (int i = 0; i < max_row; i++)
		{
			this->bitmap_row[i] = bitmap_row[i];
			if (bitmap_row[i]) num_row++;
		}

		//cout << "num row : " << num_row << endl;

		for (int i = 0; i < max_col; i++)
		{
				this->bitmap_col[i] = bitmap_col[i];
				if (bitmap_col[i]) num_col++;
		}
		//cout << "num col : " << num_col << endl;

		this->bitmap_col[base] = 0; // 수정

		for (CALCUL::row* temp = calcul.get_head(); temp; temp = temp->next, row++)
		{
			if (bitmap_row[row] && temp->arr[base] == partition) this->bitmap_row[row] = 1;
			else this->bitmap_row[row] = 0;
		}
			
			
		

	}

	int check_terminal(int base, int partition, int*bitmap_row, int*bitmap_col)
	{
	
			int row = 0;
			int num_row = 0;
			int num_col = 0;
			int success = 0, failure = 0;
			int max_col = calcul.get_max_col() - 1;
			int max_row = calcul.get_max_row() + 1;

			int* temp_bitmap_row = new int[max_row];
			int* temp_bitmap_col = new int[max_col];

			for (int i = 0; i < max_row; i++) temp_bitmap_row[i] = bitmap_row[i];
			for (int i = 0; i < max_col; i++) temp_bitmap_col[i] = bitmap_col[i];

			temp_bitmap_col[base] = 0; // 수정
			for (CALCUL::row* temp = calcul.get_head(); temp; temp = temp->next, row++)
			{
				if (bitmap_row[row] && temp->arr[base] == partition) temp_bitmap_row[row] = 1;
				else temp_bitmap_row[row] = 0;
			}

			//////////////////////////////////////////

			for (int i = 0; i < max_row; i++)
				if (temp_bitmap_row[i]) num_row++;

			if (num_row == 0)
			{
				delete[] temp_bitmap_col;
				delete[] temp_bitmap_row;

				return 1;

			}
			//////////////////////////////////////////

			for (int i = 0; i < max_col; i++)
				if (temp_bitmap_col[i]) num_col++;

			if (num_col == 1)
			{
				delete[] temp_bitmap_col;
				delete[] temp_bitmap_row;
				return 2;

			}
			//////////////////////////////////////////
			int maximum_domain_value = index.get_col_max(max_col)+1;
			int* success_array = new int[maximum_domain_value];
			for (int k = 0; k < maximum_domain_value; k++) success_array[k] = 0;

			row = 0;
			for (CALCUL::row* temp = calcul.get_head(); temp; temp = temp->next, row++)
			{
				if (temp_bitmap_row[row])
				{
					success_array[temp->arr[max_col]]++;
				}
			

			}

			for (int k = 0; k < maximum_domain_value; k++)
			{
				//outFile << "col "<< k << "=> num " << success_array[k] << endl;
				if (success_array[k] == num_row)
				{
					//outFile << "choose because all class is same " << k << endl;
					delete[] success_array;
					delete[] temp_bitmap_col;
					delete[] temp_bitmap_row;
					return (-1)*(k + 1);

				}
			}
			
			/////////////////////////////////////////

		

			delete[] temp_bitmap_col;
			delete[] temp_bitmap_row;

			return 0;

		
		
		
	}

	
	void build_tree()
	{
		
		int result;
		int flag = 0;
		int max_col = calcul.get_max_col() - 1;
		int maximum_domain_value = index.get_col_max(max_col)+1;
		int* success_array = new int[maximum_domain_value];
		
		int row = 0;
		
		base_attr = select_base_info_gain(); // 어느 애트리뷰트가 기준이 될 것인가?

		int domain_max = index.get_col_max(base_attr)+1;
		if (domain_max == 0) return;

		child = new tree_node*[domain_max];

		for (int i = 0; i < domain_max; i++)
		{
			//outFile << "I choose base_col : " << base_attr << "value : " << i << endl;
			flag = check_terminal(base_attr, i, bitmap_row, bitmap_col);
			
			if (!flag)
			{
				child[i] = new tree_node(base_attr, i, bitmap_row, bitmap_col);
				
				child[i]->build_tree();
			}
			else
			{
				switch (flag)
				{
				case 1: // row가 아무것도 없는 경우
				case 2: // col이 아무것도 없는 경우

					//cout << "!!!!!!!!" << endl;
					
					for (int k = 0; k < maximum_domain_value; k++) success_array[k] = 0;

					row = 0;
					for (CALCUL::row* temp = calcul.get_head(); temp; temp = temp->next, row++)
					{
						if (bitmap_row[row])
							success_array[temp->arr[max_col]]++;
						
					}
					
					result = 0;
					for (int k = 0; k < maximum_domain_value; k++)
						if (success_array[k] > success_array[result]) result = k;

					child[i] = new tree_node(result);

					break;

				default: // 값이 결정난 경우 ( 모든 row의 class 값이 같은 경우 )
				
					result = (-1)*(flag+1);
					child[i] = new tree_node(result);

					break;


				}
			}
		}



	}

	int select_base_info_gain()
	{
		int max_col = calcul.get_max_col()-1; // 차원 애트리뷰트의 갯수
		float factor = 0;
		float total_row = 0;
		int row = 0;
		int min = 0;
		int col_max = 0;

		float* info_gain = new float[max_col];
		float** count_col = new float*[max_col];
		float** count_class_yes = new float*[max_col];

#if GAIN_RATIO
		float* split_info = new float[max_col];
		for (int i = 0; i < max_col; i++) split_info[i] = 0;
#endif

		for (int i = 0; i < max_col; i++)
		{
#if GAIN_RATIO
			info_gain[i] = -100;	
#else
			info_gain[i] = 100;
#endif
			//if (bitmap_col[i]) continue;

			col_max = index.get_col_max(i)+1; // i번째 애트리뷰트의 도메인 최댓값 획득

			count_col[i] = new float[col_max];
			count_class_yes[i] = new float[col_max];

			for (int k = 0; k < col_max; k++)
			{
				count_class_yes[i][k] = 0;
				count_col[i][k] = 0;
			}

			travel_row_and_count(i, count_col[i]);

		
		}

		for (CALCUL::row* temp = calcul.get_head(); temp; temp = temp->next, row++)
		{
			if (bitmap_row[row])
				total_row++;

		}
		
		


		for (int k = 0; k < max_col; k++) // k번째 애트리뷰트
		{
			if (!bitmap_col[k]) continue;
			else info_gain[k] = 0;

			int col_max = index.get_col_max(k)+1;

			for (int j = 0 ; j < col_max ; j++) // k번째 애트리뷰트에서의 j번째 도메인 값
			{
				//outFile << " " <<count_col[k][j] << " ";
				//outFile << total_row << endl;
				if (count_col[k][j] == 0) continue;
				factor = count_col[k][j] / total_row;
				info_gain[k] += factor*INFO(k,j,count_col[k][j]);

#if GAIN_RATIO
				split_info[k] += (-1)*factor*log2(factor);
#endif

				//outFile << "INFO : " << INFO(k,j ,count_col[k][j]) << endl;
			}

			//outFile << " infor_gain index : " << k << " =>> " << info_gain[k] << endl;

		}

		for (int k = 0; k < max_col; k++)
		{

			if (info_gain[min] > info_gain[k]) min = k;
		}


#if (GAIN_RATIO)

		float table_info = INFO(max_col, -1, total_row);

		for (int j = 0; j < col_max; j++)
		{
			if (!bitmap_col[j] || split_info[j] == 0) continue;
			else info_gain[j] = (table_info - info_gain[j])/split_info[j];

		}

		min = 0;
		for (int k = 0; k < max_col; k++)
		{
			if (!bitmap_col[k]) continue;
			if (info_gain[min] < info_gain[k]) min = k;
		}
		delete[] split_info;
#endif

		
		// 최소의 
		
		delete[] info_gain;
		for (int p = 0; p < max_col; p++)
		{
			delete[] count_class_yes[p];
			delete[] count_col[p];
		}
		

		return min;


	}

	float INFO(int col, int domain, float total_row_number ) 
	{
		float result;

#if (INFOMATION_GAIN || GAIN_RATIO)
		result = 0;
#elif GINI_INDEX
		result = 1;
#endif

		float temp = 0;
		int max_col = calcul.get_max_col() - 1;
		int maximum_domain_value = index.get_col_max(max_col)+1;
		int* success_array = new int[maximum_domain_value];
		int row = 0;

		for (int k = 0; k < maximum_domain_value; k++) success_array[k] = 0;

		
		for (CALCUL::row* temp = calcul.get_head(); temp; temp = temp->next, row++)
		{
			if (bitmap_row[row] && domain == -1)
			{
				success_array[temp->arr[max_col]]++;
			}
			else if (bitmap_row[row] && temp->arr[col] == domain )
				success_array[temp->arr[max_col]]++;

		}

		for (int k = 0; k < maximum_domain_value; k++)
		{
			//outFile << success_array[k] << "  ";
			if (success_array[k] == 0) continue;
			temp = success_array[k]/total_row_number;
#if (INFOMATION_GAIN || GAIN_RATIO)
			result += (-1)*temp*log2(temp);
#elif GINI_INDEX
			result -= temp*temp;
#endif
		}
		//outFile << endl;
		delete[] success_array;

		return result;
	
	}

	void travel_row_and_count(int col, float* array_count )
	{
		CALCUL::row* temp = calcul.get_head();
		int max_col = calcul.get_max_col();
		int row = 0;

		for (; temp ; temp = temp->next , row++ )
		{
			if (bitmap_row[row])
			{
				array_count[temp->arr[col]]++;
				
			}

		}


	}

	void get_result(int* bitmap)
	{
		if (this->result != -1)
		{
			bitmap[calcul.get_max_col() - 1] = result;
			return;
		}
		else
		{
			int go = bitmap[base_attr];
			child[go]->get_result(bitmap);
		}
	}



};

tree_node* root;

void CALCUL::create_decision_tree()
{
	
	root = new tree_node();
	root->build_tree();

	
}

void CALCUL::classification()
{

}

