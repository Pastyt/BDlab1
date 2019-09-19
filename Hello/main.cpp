#include <iostream>
#include <fstream>
using namespace std;

int block_size = 5;

struct student {
	char name[20];
	char sur[20];
	char twoname[30];
	int id;
	int group;
	bool del;
};

struct block {
	student stud[5];
	int block_id;
};


bool search(int id) {
	int ser = 0;
	/*int check=data.block_id,ser=0;
	for (int i=0; i<block_size;i++){
		if (id==data.stud[i].id) ser++;
	} */
	FILE* file;
	file = fopen("bd.txt", "r+");
	do {
		/*if (data.block_id==check)
		fseek(file, sizeof(struct block), SEEK_CUR);
		*/
		fread(&data, sizeof(struct block), 1, file);

		for (int i = 0; i < block_size; i++) {
			if (id == data.stud[i].id) ser++;
		}
	} while (!feof(file));
	if (ser > 1) return 1;
	return 0;
}
void showmydata()
{
	FILE* file;
	file = fopen("bd.txt", "r+");
	int prev = -1;
	while (!feof(file)) {
		fread(&data, sizeof(struct block), 1, file);
		if (prev == data.block_id) break;
		cout << "Block numb: " << data.block_id << endl;
		for (int i = 0; i < block_size; i++) {
			cout << data.stud[i].name << " " << data.stud[i].sur << " " << data.stud[i].twoname
				<< " " << data.stud[i].id << " " << data.stud[i].group << endl;
			prev = data.block_id;
		}
	}
	cout << "End" << endl;
}
int main() {
	FILE* file;
	file = fopen("bd.txt", "r+");
	data.block_id = 0;
	student c;
	int k = 0;
	cout << "1. New data" << endl;
	cout << "2. Search data" << endl;
	cout << "3. Show all" << endl;
	cout << "4. Change data" << endl;
	cout << "5. Delete data" << endl;
	cout << "6. Delete trash" << endl;
	cout << "0. Exit" << endl;
	int meme = 1;
	bool test;
	while (meme != 0) {

		cin >> meme;

		switch (meme) {
		case 1:
			cout << "Enter name, surname, otchestvo, id, group" << endl;
			data.stud[k].del = 0;
			cin >> data.stud[k].name >> data.stud[k].sur >> data.stud[k].twoname;
			cin >> data.stud[k].id >> data.stud[k].group;

			test = search(data.stud[k].id);
			if (test) {
				cout << "Data not unic" << endl;
				break;
			}
			k++;
			if (k == block_size) {
				k = 0;
				fwrite(&data, sizeof(struct block), 1, file);
				data.block_id++;
			}
			cout << "End" << endl;
			break;

		case 3:
			showmydata();
			break;
		}
		fclose(file);
	}
}
