constexpr auto max_block = 5;
struct student {
	char name[20];
	char sur[20];
	char twoname[30];
	int id;
	int group;
	bool del;
};

struct block {
	student stud[max_block];
};
int search_id(int, int, int, block, FILE*);
void writesomedata(int& casework, int& current_work, int& k, int& casek, FILE* base_data, block& data);
void showmydata(FILE* base_data, int cur_work);
int search_id(int cur_work, int k, int id, block data, FILE* base_data);
int search_student(int& cur_work, int id, block& data, FILE* base_data);
int trashwork(int& casek, int& current_work, int& casework, int& k, FILE* base_data, block& data);