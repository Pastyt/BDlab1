#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "functions.h";

using namespace std;

int main() 
{
	block data;//cleardata;
	int current_work = 0,casework,k=0,casek;
	FILE* base_data;
	base_data = fopen("base_data.bin", "r+");
	memset(&data, '\0', sizeof(block));
	fread(&data, sizeof(struct block), 1, base_data);
	while (data.stud[0].del != false) {
		current_work++;
		memset(&data, '\0', sizeof(block));
		fread(&data, sizeof(struct block), 1, base_data);
	}
	if(current_work!=0) current_work--;
	fseek(base_data, sizeof(struct block) * current_work, SEEK_SET);
	fread(&data, sizeof(struct block), 1, base_data);
	while (data.stud[k].del && k!=4) k++;
	if (k == 4) {
		current_work++;
		memset(&data, '\0', sizeof(block));
		k = 0;
	}
	cout << "1. New data" << endl;
	cout << "2. Search data" << endl;
	cout << "3. Show all" << endl;
	cout << "4. Change data" << endl;
	cout << "5. Delete data" << endl;
	cout << "6. Clear database trash" << endl;
	cout << "7. Delete database" << endl;
	cout << "0. Exit" << endl;
	int meme = 1;
	casek = k;
	casework = current_work;
	cout << "current work = " << current_work<< endl;
	cout << "k = " << k << endl;
	while (meme != 0) {
		cin >> meme;
		switch (meme) {
		case 1:
			writesomedata(casework, current_work, k, casek, base_data, data);
			break;
		case 2:
			cout << "Type ID" << endl;
			cin >> meme;
			k=search_student(current_work, meme, data, base_data);
			if (k == -1) {
				cout << "Data not found" << endl;
				break;
			}
			cout << data.stud[k].name << " " << data.stud[k].sur << " " << data.stud[k].twoname;
			cout << " " << data.stud[k].id << " " << data.stud[k].group << endl;
		break;

		case 3:
			showmydata(base_data,current_work);
			break;
		case 4: //Change data
			cout << "Type ID" << endl;
			cin >> meme;
			k = search_student(current_work, meme, data, base_data);
			if (k == -1) {
				cout << "Data not found" << endl;
				break;
			}
			cout << "Change data" << endl;
			cin >> data.stud[k].name >> data.stud[k].sur >> data.stud[k].twoname;
			cin >> data.stud[k].id >> data.stud[k].group;
			/*if (search_id(casework, k, data.stud[k].id, data, base_data) != -1) {
				cout << "Data are repeated" << endl;
				break;
			}*/
			fseek(base_data, sizeof(block) * current_work, SEEK_SET);
			fwrite(&data, sizeof(struct block), 1, base_data);
			break;
		case 5: //DELETE
			cout << "Type ID" << endl;
			cin >> meme;
			k = search_student(current_work, meme, data, base_data);
			if (k == -1) {
				cout << "Data not found" << endl;
				break;
			}
			data.stud[k].del = false;
			fseek(base_data, sizeof(block)* current_work, SEEK_SET);
			fwrite(&data, sizeof(struct block), 1, base_data);
			break;
		case 6:
			if (trashwork(casek,current_work,casework,k,base_data,data))
				return 0;
			break;
		case 7:
			base_data = fopen("base_data.bin", "w+");
			current_work = 0;
			k = 0;
			casework = 0;
			casek = 0;
			break;

		}
		
	}
	fclose(base_data);
	return 0;
}
