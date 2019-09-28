//Data is not completely overwritten if received through fread NEED FIX
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#define max_block 5
using namespace std;

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
int search_id(int, int, int,block,FILE*);
void writesomedata(int &casework, int &current_work,int &k,int &casek, FILE* base_data,block &data)
{
	if (casework != current_work) {
		current_work = casework;
		casek = k;
		fseek(base_data, sizeof(struct block) * casework, SEEK_SET);
		fread(&data, sizeof(struct block), 1, base_data);
		cout << "Write to mem" << endl;
	}
	cout << "Enter name, surname, otchestvo, id, group" << endl;
	cin >> data.stud[casek].name >> data.stud[casek].sur >> data.stud[casek].twoname;
	cin >> data.stud[casek].id >> data.stud[casek].group;

	if (search_id(casework, casek, data.stud[casek].id, data, base_data) != -1) {
		cout << "Data are repeated" << endl;
		return;
	}
	data.stud[casek].del = true;
	casek++;
	fseek(base_data, sizeof(struct block) * casework, SEEK_SET);
	fwrite(&data, sizeof(struct block), 1, base_data);
	cout << "Write to mem" << endl;
	if (casek == 5) {
		casework++;
		memset(&data, '\0', sizeof(block));
		casek = 0;
	}
	cout << "End" << endl;
	current_work = casework;
	k = casek;
}
void showmydata(FILE* base_data, int cur_work)
{
	block data;
	fseek(base_data, 0, SEEK_SET);
	while (!feof(base_data)) {
		cout << "------------------------------------------------"<< endl;
		memset(&data, '\0', sizeof(block));
		fread(&data, sizeof(struct block), 1, base_data);
		for (int i = 0; i < max_block; i++)
		{
			if (data.stud[i].del == true) {
				cout << data.stud[i].name << " " << data.stud[i].sur << " " << data.stud[i].twoname;
				cout << " " << data.stud[i].id << " " << data.stud[i].group << endl;
			}
		}
	}
}
int search_id(int cur_work, int k, int id, block data,FILE *base_data)
{
	for (int i = 0; i < max_block; i++)
		if (data.stud[i].id == id && i != k && data.stud[i].del) return i;
	fseek(base_data, 0, SEEK_SET);
	int now_work=-1;
	while (!feof(base_data)) {
		fread(&data, sizeof(struct block), 1, base_data);
		now_work++;
		for (int i = 0; i < max_block; i++)
			if (data.stud[i].id == id && 
				i != k && now_work!=cur_work && data.stud[i].del) return i;
	}
	return -1;
}
int search_student(int& cur_work,int id, block& data, FILE* base_data)
{
	for (int i = 0; i < max_block; i++)
		if (data.stud[i].id == id) return i;
	fseek(base_data, 0, SEEK_SET);
	memset(&data, '\0', sizeof(block));
	fread(&data, sizeof(struct block), 1, base_data);
	int now_work = 0;
	while (data.stud[0].del != false) {
		if (now_work == cur_work) continue;
		for (int i = 0; i < max_block; i++)
			if (data.stud[i].id == id) {
				cur_work = now_work;
				return i;
			}
		fread(&data, sizeof(struct block), 1, base_data);
		now_work++;
	}
	return -1;
}
int trashwork(int &casek, int &current_work,int &casework,int &k, FILE* base_data,block &data) {
	casek--;
	current_work = 0;
	block casedata;
	bool booldata, boolcase;
	memset(&data, '\0', sizeof(block));
	memset(&casedata, '\0', sizeof(block));
	fseek(base_data, sizeof(block) * casework, SEEK_SET);
	fread(&casedata, sizeof(struct block), 1, base_data);
	fseek(base_data, 0, SEEK_SET);
	fread(&data, sizeof(struct block), 1, base_data);
	while (true) {

		booldata = false; boolcase = false;
		while (data.stud[0].name[0] != '\0') {
			for (k = 0; k < 5; k++)
			{
				if (data.stud[k].del == false) {
					booldata = true;
					goto exit1;
				}
			}
			current_work++;
			memset(&data, '\0', sizeof(block));
			fseek(base_data, sizeof(block) * current_work, SEEK_SET);
			fread(&data, sizeof(struct block), 1, base_data);
		}
	exit1:
		while (true)
		{
			for (casek = 4; casek > -1; casek--)
			{
				if (casedata.stud[casek].del == true) {
					boolcase = true;
					goto exit2;
				}
			}
			casework--;
			if (casework == -1) break;
			memset(&casedata, '\0', sizeof(block));
			fseek(base_data, sizeof(block) * casework, SEEK_SET);
			fread(&casedata, sizeof(struct block), 1, base_data);
		}
	exit2:
		if (booldata && boolcase && current_work <= casework) {
			if (current_work == casework)
				if (k > casek) break;
			cout << "Move block " << casework << " num " << casek << endl;
			cout << "To " << current_work << " num " << k << endl;
			data.stud[k] = casedata.stud[casek];
			casedata.stud[casek].del = false;
			fseek(base_data, sizeof(block) * current_work, SEEK_SET);
			fwrite(&data, sizeof(struct block), 1, base_data);
			fseek(base_data, sizeof(block) * casework, SEEK_SET);
			fwrite(&casedata, sizeof(struct block), 1, base_data);
			if (casek == 0) {
				int cur_page = casework;
				FILE* temp_file;
				temp_file = fopen("temp_data.bin", "w+");
				block temp_data;
				fseek(base_data, 0, SEEK_SET);
				for (int i = 0; i < cur_page; i++)
				{
					memset(&data, '\0', sizeof(block));
					fread(&temp_data, sizeof(struct block), 1, base_data);
					fwrite(&temp_data, sizeof(struct block), 1, temp_file);
				}
				char newname[] = "temp_data.bin";
				char name[] = "base_data.bin";
				fclose(base_data);
				fclose(temp_file);
				if (remove(name)) {
					perror("Error: ");
					cout << endl;
					/*fclose(base_data);
					return 1; */
				}
				if (rename(newname, name)) {
					perror("Error: ");
					cout << endl;
					/*fclose(base_data);
					return 1; */
				}
				base_data = fopen("base_data.bin", "r+");
			}
		}
		else break;
	}

	current_work = 0; k = 0;
	fseek(base_data, 0, SEEK_SET);
	memset(&data, '\0', sizeof(block));
	fread(&data, sizeof(struct block), 1, base_data);
	while (data.stud[0].del != false) {
		current_work++;
		memset(&data, '\0', sizeof(block));
		fread(&data, sizeof(struct block), 1, base_data);
	}
	/*memset(&data, '\0', sizeof(block));
	fseek(base_data, sizeof(block)* current_work, SEEK_SET);
	fwrite(&data, sizeof(struct block), 1, base_data); */
	if (current_work != 0) current_work--;
	fseek(base_data, sizeof(struct block) * current_work, SEEK_SET);
	fread(&data, sizeof(struct block), 1, base_data);
	while (data.stud[k].del && k != 4) k++;

	if (k == 4) {
		current_work++;
		memset(&data, '\0', sizeof(block));
		k = 0;
	}
	casek = k;
	casework = current_work;
	cout << "k = " << k << endl;
	cout << "block = " << casework << endl;
	return 0;
}
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
